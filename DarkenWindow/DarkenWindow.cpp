﻿#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//---------------------------------------------------------------------

// デバッグ用コールバック関数。デバッグメッセージを出力する
void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	::OutputDebugString(output);
}

//---------------------------------------------------------------------

AviUtlInternal g_auin;
HINSTANCE g_instance = 0;

inline static struct {
	ACTCTX ac = { sizeof(ac) };
	HANDLE hac = INVALID_HANDLE_VALUE;
	ULONG_PTR cookie = 0;

	//
	// アクティブ化コンテキストを作成します。
	//
	BOOL init() {
		// 初期化済みかどうかフラグをチェックします。
		static BOOL isInited = FALSE;
		if (isInited) return FALSE;
		isInited = TRUE;

		// ファイル名を取得します。
		TCHAR fileName[MAX_PATH] = {};
		::GetModuleFileName(g_instance, fileName, std::size(fileName));
		::PathRemoveFileSpec(fileName);
		::PathAppend(fileName, _T("cc6.manifest"));
		MY_TRACE_TSTR(fileName);

		// アクティブ化コンテキストを作成します。
		ac.lpSource = fileName;
		hac = ::CreateActCtx(&ac);
		BOOL result = ::ActivateActCtx(hac, &cookie);
		MY_TRACE_INT(result);

		return result;
	}

	//
	// アクティブ化コンテキストを削除します。
	//
	BOOL exit() {
		if (cookie) ::DeactivateActCtx(0, cookie), cookie = 0;
		if (hac != INVALID_HANDLE_VALUE) ::ReleaseActCtx(hac), hac = INVALID_HANDLE_VALUE;
		return TRUE;
	}
} actctx;

//---------------------------------------------------------------------
#if 1
__declspec(naked) LRESULT CALLBACK getCallWindowProcInternal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// リターン先のアドレスを返す。
	__asm
	{
		MOV EAX,[ESP]
		RETN
	}
}
#else
LRESULT CALLBACK getCallWindowProcInternal(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// リターン先のアドレスを返す。
	DWORD from = *((DWORD*)&hwnd - 1);
	return from;
}
#endif
void initHook()
{
	MY_TRACE(_T("initHook()\n"));

	HMODULE user32 = ::GetModuleHandle(_T("user32.dll"));
	GET_HOOK_PROC(user32, DrawFrame);

	// 2B = 7671339B - 76713370
	uintptr_t address1 = ::CallWindowProcW(getCallWindowProcInternal, 0, 0, 0, 0) - 0x2B;
	BYTE code[1] = {};
	::ReadProcessMemory(::GetCurrentProcess(), (LPCVOID)address1, code, sizeof(code), 0);
	if (code[0] == 0xCC) address1 += 0x01;
	uintptr_t address2 = (uintptr_t)user32 + 0x00043370;
	true_CallWindowProcInternal = (Type_CallWindowProcInternal)address1;

	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());

//	ATTACH_HOOK_PROC(DrawThemeParentBackground);
	ATTACH_HOOK_PROC(DrawThemeBackground);
	ATTACH_HOOK_PROC(DrawThemeBackgroundEx);
	ATTACH_HOOK_PROC(DrawThemeText);
	ATTACH_HOOK_PROC(DrawThemeTextEx);
//	ATTACH_HOOK_PROC(DrawThemeIcon);
//	ATTACH_HOOK_PROC(DrawThemeEdge);

	ATTACH_HOOK_PROC(FillRect);
//	ATTACH_HOOK_PROC(DrawFrame);
//	ATTACH_HOOK_PROC(DrawFrameControl);
//	ATTACH_HOOK_PROC(FrameRect);
	ATTACH_HOOK_PROC(DrawEdge);
//	ATTACH_HOOK_PROC(DrawFocusRect);
//	ATTACH_HOOK_PROC(DrawStateW);
	ATTACH_HOOK_PROC(ExtTextOutW);
//	ATTACH_HOOK_PROC(PatBlt);

	ATTACH_HOOK_PROC(CallWindowProcInternal);

//	ATTACH_HOOK_PROC(LoadIconA);
//	ATTACH_HOOK_PROC(LoadIconW);
	ATTACH_HOOK_PROC(LoadImageA);
//	ATTACH_HOOK_PROC(LoadImageW);
	ATTACH_HOOK_PROC(DrawIconEx);
	ATTACH_HOOK_PROC(LoadMenuA);
#if 0
	ATTACH_HOOK_PROC(OpenThemeData);
	ATTACH_HOOK_PROC(OpenThemeDataForDpi);
	ATTACH_HOOK_PROC(OpenThemeDataEx);
	ATTACH_HOOK_PROC(SetWindowTheme);
#endif
	if (DetourTransactionCommit() == NO_ERROR)
	{
		MY_TRACE(_T("API フックに成功しました\n"));
	}
	else
	{
		MY_TRACE(_T("API フックに失敗しました\n"));
	}
}

void termHook()
{
	MY_TRACE(_T("termHook()\n"));

	DETACH_HOOK_PROC2(DrawThemeParentBackground);
	DETACH_HOOK_PROC2(DrawThemeBackground);
	DETACH_HOOK_PROC2(DrawThemeBackgroundEx);
	DETACH_HOOK_PROC2(DrawThemeText);
	DETACH_HOOK_PROC2(DrawThemeTextEx);
	DETACH_HOOK_PROC2(DrawThemeIcon);
	DETACH_HOOK_PROC2(DrawThemeEdge);

	DETACH_HOOK_PROC2(FillRect);
	DETACH_HOOK_PROC2(DrawFrame);
	DETACH_HOOK_PROC2(DrawFrameControl);
	DETACH_HOOK_PROC2(FrameRect);
	DETACH_HOOK_PROC2(DrawEdge);
	DETACH_HOOK_PROC2(DrawFocusRect);
	DETACH_HOOK_PROC2(DrawStateW);
	DETACH_HOOK_PROC2(ExtTextOutW);
	DETACH_HOOK_PROC2(PatBlt);

	DETACH_HOOK_PROC2(CallWindowProcInternal);

	DETACH_HOOK_PROC2(LoadIconA);
	DETACH_HOOK_PROC2(LoadIconW);
	DETACH_HOOK_PROC2(LoadImageA);
	DETACH_HOOK_PROC2(LoadImageW);
	DETACH_HOOK_PROC2(DrawIconEx);
	DETACH_HOOK_PROC2(LoadMenuA);

	DETACH_HOOK_PROC2(OpenThemeData);
	DETACH_HOOK_PROC2(OpenThemeDataForDpi);
	DETACH_HOOK_PROC2(OpenThemeDataEx);
	DETACH_HOOK_PROC2(SetWindowTheme);
}

//---------------------------------------------------------------------

// 外部プロセスで使用する場合はこの関数をインポートして呼ぶ。
void WINAPI DarkenWindow_init(HWND hwnd)
{
	MY_TRACE(_T("DarkenWindow_init(0x%08X)\n"), hwnd);

	// テーマフックをセットする。
	initThemeHook(hwnd);

	g_skin.init(g_instance, hwnd);
	g_skin.reloadSettings(TRUE);
}

BOOL WINAPI checkPatch()
{
	MY_TRACE(_T("checkPatch()\n"));

	// システム情報を取得する。
	AviUtl::SysInfo si = {};
	g_auin.get_sys_info(0, &si);

	// バージョン文字列を取得する。
	if (!si.info)
	{
		::MessageBox(
			0,
			_T("バージョン文字列を取得できませんでした") _T("\n")
			_T("DarkenWindow は正常に動作することができません") _T("\n") _T(" "),
			_T("DarkenWindow"),
			MB_OK);

		return FALSE; // バージョン文字列を取得できなかった。
	}

	// バージョン文字列を見つける。
	LPCSTR p = strstr(si.info, "patched r");
	if (!p)
	{
		int retValue = ::MessageBox(
			0,
			_T("patch.aul のバージョン文字列が見つかりませんでした") _T("\n")
			_T("DarkenWindow は正常に動作することができません") _T("\n")
			_T("patch.aul を導入済みなのにこのメッセージが出ている場合は OK を押してください") _T("\n") _T(" "),
			_T("DarkenWindow"),
			MB_OKCANCEL | MB_ICONQUESTION);

		if (retValue == IDOK)
		{
			if (!::LoadLibrary(_T("MSVCP140_ATOMIC_WAIT.DLL")))
			{
				int retValue = ::MessageBox(
					0,
					_T("Visual Studio 2022 のランタイムライブラリがインストールされていない可能性があります") _T("\n")
					_T("ランタイムライブラリのダウンロードリンクをクリップボードにコピーする場合は OK を押してください") _T("\n") _T(" "),
					_T("DarkenWindow"),
					MB_OKCANCEL | MB_ICONQUESTION);

				if (retValue == IDOK)
				{
					::OpenClipboard(0);
					::EmptyClipboard();
					HANDLE handle = ::GlobalAlloc(GHND | GMEM_SHARE, MAX_PATH * sizeof(TCHAR));
					LPTSTR buffer = (LPTSTR)::GlobalLock(handle);
					::StringCchCopy(buffer, MAX_PATH, _T("https://aka.ms/vs/17/release/vc_redist.x86.exe"));
					::GlobalUnlock(handle);
					::SetClipboardData(CF_TEXT, handle);
					::CloseClipboard();

					::MessageBox(
						0,
						_T("https://aka.ms/vs/17/release/vc_redist.x86.exe") _T("\n")
						_T("上記のランタイムライブラリのダウンロードリンクをクリップボードにコピーしました") _T("\n")
						_T("ブラウザのアドレスバーに貼り付けてダウンロードしてください") _T("\n") _T(" "),
						_T("DarkenWindow"),
						MB_OK);
				}
			}
			else
			{
				int retValue = ::MessageBox(
					0,
					_T("patch.aul が AviUtl のフォルダの中にあるか確認してください") _T("\n") _T(" "),
					_T("DarkenWindow"),
					MB_OK);
			}
		}

		return FALSE; // バージョン文字列を見つけられなかった。
	}

	p += strlen("patched r");

	// バージョンを取得する。
	int version = atoi(p);
	if (version < 18)
	{
		::MessageBox(
			0,
			_T("patch.aul r18 以上が見つかりませんでした") _T("\n")
			_T("DarkenWindow は正常に動作することができません") _T("\n")
			_T("patch.aul の最新バージョンを導入してください") _T("\n") _T(" "),
			_T("DarkenWindow"),
			MB_OK);

		return FALSE; // バージョンが低すぎた。
	}

	return TRUE;
}

BOOL WINAPI init(HWND hwnd)
{
	MY_TRACE(_T("init(0x%08X)\n"), hwnd);

	static BOOL isInited = FALSE;
	if (!isInited)
	{
		isInited = TRUE;

		// AviUtl のアドレス情報を取得する。
		g_auin.initAviUtlAddress();

		if(!checkPatch()) // patch.aul のバージョンを確認する。
		{
			termHook();

			return FALSE;
		}

		DarkenWindow_init(hwnd);

		return TRUE;
	}

	return TRUE;
}

// 主に他プラグインに関する初期化処理を行う。
BOOL WINAPI postInit()
{
	MY_TRACE(_T("postInit()\n"));

	{
		// loudness.auf 内の ::TextOutA() をフックする。

		HMODULE loudness = ::GetModuleHandle(_T("loudness.auf"));
		MY_TRACE_HEX(loudness);

		if (loudness)
		{
			true_loudness_TextOutA = hookImportFunc(loudness, "TextOutA", hook_loudness_TextOutA);
			MY_TRACE_HEX(true_loudness_TextOutA);
		}
	}

	return TRUE;
}

void showSkinSelector(HWND hwnd)
{
	MY_TRACE(_T("showSkinSelector(0x%08X)\n"), hwnd);

	TCHAR origin[MAX_PATH] = {};
	::GetModuleFileName(g_instance, origin, MAX_PATH);
	::PathRemoveExtension(origin);
	::PathAppend(origin, _T("Skin"));
	MY_TRACE_TSTR(origin);

	std::vector<std::string> fileNames;
	{
		TCHAR folder[MAX_PATH] = {};
		::StringCbCopy(folder, sizeof(folder), origin);
		::PathAppend(folder, _T("*.xml"));
		MY_TRACE_TSTR(folder);

		WIN32_FIND_DATA ffd = {};
		HANDLE handle = ::FindFirstFile(folder, &ffd);

		if (handle == INVALID_HANDLE_VALUE)
			return;

		do
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				continue;

			fileNames.push_back(ffd.cFileName);
		}
		while (::FindNextFile(handle, &ffd));

		::FindClose(handle);

		std::sort(fileNames.begin(), fileNames.end());
	}

	TCHAR current[MAX_PATH] = {};
	::StringCbCopy(current, sizeof(current), g_skin.getSkinFileName());
	::PathStripPath(current);

	const int ID_SHADOW_MODE = 20000;
	const int ID_ROUND_MODE = 20001;
	const int ID_STATIC_EDGE_MODE = 20002;
	const int ID_USE_LAYER_COLOR_EX = 20003;

	HMENU menu = ::CreatePopupMenu();

	for (int i = 0; i < (int)fileNames.size(); i++)
	{
		TCHAR text[MAX_PATH] = {};
		::StringCbCopy(text, sizeof(text), fileNames[i].c_str());
		::PathRemoveExtension(text);

		::AppendMenu(menu, MF_STRING, i + 1, text);

		if (::lstrcmpi(fileNames[i].c_str(), current) == 0)
			::CheckMenuItem(menu, i + 1, MF_CHECKED);
	}

	::AppendMenu(menu, MF_SEPARATOR, -1, 0);
	::AppendMenu(menu, MF_STRING, ID_SHADOW_MODE, _T("影を付ける"));
	::AppendMenu(menu, MF_STRING, ID_ROUND_MODE, _T("丸くする"));
	::AppendMenu(menu, MF_STRING, ID_STATIC_EDGE_MODE, _T("ボタンにスタティックエッジを付ける"));
	::AppendMenu(menu, MF_STRING, ID_USE_LAYER_COLOR_EX, _T("複数行の色分け"));

	if (g_skin.getShadowMode() == Dark::SHADOW_MODE_ON)
		::CheckMenuItem(menu, ID_SHADOW_MODE, MF_CHECKED);

	if (g_skin.getRoundMode() == Dark::ROUND_MODE_ON)
		::CheckMenuItem(menu, ID_ROUND_MODE, MF_CHECKED);

	if (g_skin.getStaticEdgeMode() == Dark::STATIC_EDGE_MODE_ON)
		::CheckMenuItem(menu, ID_STATIC_EDGE_MODE, MF_CHECKED);

	if (g_skin.getUseLayerColorEx())
		::CheckMenuItem(menu, ID_USE_LAYER_COLOR_EX, MF_CHECKED);

	POINT pt; ::GetCursorPos(&pt);
	int id = ::TrackPopupMenu(menu, TPM_NONOTIFY | TPM_RETURNCMD, pt.x, pt.y, 0, hwnd, 0);

	::DestroyMenu(menu);

	if (id > 0 && id <= (int)fileNames.size())
	{
		TCHAR fileName[MAX_PATH] = {};
		::StringCbCopy(fileName, sizeof(fileName), _T("Skin"));
		::PathAppend(fileName, fileNames[id - 1].c_str());
		MY_TRACE_TSTR(fileName);

		g_skin.setSkinFileName(fileName);
		g_skin.saveSettings();
	}
	else if (id == ID_SHADOW_MODE)
	{
		if (g_skin.getShadowMode() == Dark::SHADOW_MODE_ON)
			g_skin.setShadowMode(Dark::SHADOW_MODE_OFF);
		else
			g_skin.setShadowMode(Dark::SHADOW_MODE_ON);

		g_skin.saveSettings();
	}
	else if (id == ID_ROUND_MODE)
	{
		if (g_skin.getRoundMode() == Dark::ROUND_MODE_ON)
			g_skin.setRoundMode(Dark::ROUND_MODE_OFF);
		else
			g_skin.setRoundMode(Dark::ROUND_MODE_ON);

		g_skin.saveSettings();
	}
	else if (id == ID_STATIC_EDGE_MODE)
	{
		if (g_skin.getStaticEdgeMode() == Dark::STATIC_EDGE_MODE_ON)
			g_skin.setStaticEdgeMode(Dark::STATIC_EDGE_MODE_OFF);
		else
			g_skin.setStaticEdgeMode(Dark::STATIC_EDGE_MODE_ON);

		g_skin.saveSettings();

		::MessageBox(hwnd, _T("このオプションは AviUtl を再起動したときに反映されます"), _T("DarkenWindow"), MB_OK);
	}
	else if (id == ID_USE_LAYER_COLOR_EX)
	{
		g_skin.setUseLayerColorEx(!g_skin.getUseLayerColorEx());

		g_skin.saveSettings();

		::InvalidateRect(g_auin.GetExEditWindow(), 0, TRUE);
	}
}

//---------------------------------------------------------------------

EXTERN_C BOOL APIENTRY DllMain(HINSTANCE instance, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
		{
			// ロケールを設定する。
			// これをやらないと日本語テキストが文字化けするので最初に実行する。
			_tsetlocale(LC_CTYPE, _T(""));

			MY_TRACE(_T("DLL_PROCESS_ATTACH\n"));

			// この DLL のハンドルをグローバル変数に保存しておく。
			g_instance = instance;
			MY_TRACE_HEX(g_instance);

			// この DLL の参照カウンタを増やしておく。
			WCHAR moduleFileName[MAX_PATH] = {};
			::GetModuleFileNameW(g_instance, moduleFileName, MAX_PATH);
			::LoadLibraryW(moduleFileName);

			initHook();

			break;
		}
	case DLL_PROCESS_DETACH:
		{
			MY_TRACE(_T("DLL_PROCESS_DETACH\n"));

//			termHook();

			break;
		}
	}

	return TRUE;
}

//---------------------------------------------------------------------

IMPLEMENT_HOOK_PROC_NULL(LRESULT, WINAPI, CallWindowProcInternal, (WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam))
{
#if 0
	DWORD from = *((DWORD*)&wndProc - 1);
	MY_TRACE(_T("0x%08X => CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), from, hwnd, message, wParam, lParam);
#endif
	switch (message)
	{
	case WM_NCCREATE:
		{
			initRenderer(hwnd);

			break;
		}
	case WM_NCDESTROY:
		{
			termRenderer(hwnd);

			break;
		}
	case WM_CREATE:
		{
			if (wndProc != (WNDPROC)::GetClassLongPtrA(hwnd, GCLP_WNDPROC) &&
				wndProc != (WNDPROC)::GetClassLongPtrW(hwnd, GCLP_WNDPROC))
			{
				break;
			}

			TCHAR className[MAX_PATH] = {};
			::GetClassName(hwnd, className, MAX_PATH);
			//MY_TRACE_TSTR(className);

			if (::lstrcmpi(className, _T("AviUtl")) == 0)
			{
				init(hwnd);

				g_skin.setDwm(hwnd, FALSE);
			}
			else if (::lstrcmpi(className, _T("ExtendedFilterClass")) == 0)
			{
				postInit();

				g_skin.setDwm(hwnd, FALSE);

				g_skin.reloadExEditSettings();
			}
			else if (getStyle(hwnd) & WS_CAPTION)
			{
				g_skin.setDwm(hwnd, FALSE);
			}
			else if (::lstrcmpi(className, WC_STATIC) == 0)
			{
				// ラウドネスメーター(全体) のスタティックテキストをちょっと変更する。

				HWND parent = ::GetParent(hwnd);
				TCHAR windowText[MAX_PATH] = {};
				::GetWindowText(parent, windowText, MAX_PATH);
//				MY_TRACE_TSTR(windowText);

				if (::lstrcmp(windowText, _T("ラウドネスメーター(全体)")) == 0)
					modifyStyle(hwnd, 0, SS_CENTERIMAGE);
			}

			break;
		}
	case WM_ACTIVATE:
		{
			int active = LOWORD(wParam);

//			MY_TRACE(_T("WM_ACTIVATE, %d\n"), active);

			g_skin.setDwm(hwnd, active);

			break;
		}
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSCROLLBAR:
		{
//			MY_TRACE(_T("WM_CTLCOLOR, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n"), hwnd, message, wParam, lParam);

			if (message == WM_CTLCOLOREDIT || message == WM_CTLCOLORLISTBOX)
			{
				if (wndProc != (WNDPROC)::GetClassLongPtrA(hwnd, GCLP_WNDPROC) &&
					wndProc != (WNDPROC)::GetClassLongPtrW(hwnd, GCLP_WNDPROC) &&
					wndProc != ::DefDlgProcA &&
					wndProc != ::DefDlgProcW)
				{
					break;
				}
			}

			HDC dc = (HDC)wParam;
			HWND control = (HWND)lParam;
			BOOL enable = ::IsWindowEnabled(control);
			HBRUSH brush = (HBRUSH)true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
//			HBRUSH dcBrush = (HBRUSH)::GetStockObject(DC_BRUSH);
			COLORREF bkColor = ::GetBkColor(dc);
//			COLORREF brushColor = my::getBrushColor(brush);
//			MY_TRACE(_T("brush = 0x%08X, dcBrush = 0x%08X, bkColor = 0x%08X, brushColor = 0x%08X\n"), brush, dcBrush, bkColor, brushColor);

			HTHEME theme = g_skin.getTheme(Dark::THEME_CTLCOLOR);
			int partId = g_skin.getCtlColorPartId(message);
			int stateId = 0;

			if (!enable)
			{
				stateId = Dark::CTLCOLOR_DISABLED;
			}
			else if (message == WM_CTLCOLOREDIT)
			{
				if (brush == (HBRUSH)(COLOR_BTNFACE + 1) || bkColor == ::GetSysColor(COLOR_BTNFACE))
				{
					// スタティックコントロールに似たエディットボックスだった。
					stateId = Dark::CTLCOLOR_STATICTEXT;
					::SetProp(control, _T("DarkenWindow.State"), (HANDLE)ETS_STATICTEXT);
//					MY_TRACE(_T("ETS_STATICTEXT 0x%08X\n"), control);
				}
				else
				{
					// 通常のエディットボックスだった。
					stateId = Dark::CTLCOLOR_NORMAL;
					::SetProp(control, _T("DarkenWindow.State"), (HANDLE)ETS_NORMAL);
//					MY_TRACE(_T("ETS_NORMAL 0x%08X\n"), control);
				}
			}
			else if (brush != (HBRUSH)::GetStockObject(DC_BRUSH))
			{
				if (brush == (HBRUSH)(COLOR_BTNFACE + 1) || bkColor == ::GetSysColor(COLOR_BTNFACE))
				{
					stateId = Dark::CTLCOLOR_NORMAL;
				}
				else if (brush == (HBRUSH)(COLOR_WINDOW + 1) || bkColor == ::GetSysColor(COLOR_WINDOW))
				{
					stateId = Dark::CTLCOLOR_NORMAL;
				}
			}

			Dark::StatePtr state = g_skin.getState(theme, partId, stateId);
			if (!state)
				return (LRESULT)brush;

			return (LRESULT)state->getFillBrush();
		}
	case WM_CTLCOLORSTATIC:
		{
//			MY_TRACE(_T("WM_CTLCOLORSTATIC, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n"), hwnd, message, wParam, lParam);

			HDC dc = (HDC)wParam;
			HWND control = (HWND)lParam;
			BOOL enable = ::IsWindowEnabled(control);
			HBRUSH brush = (HBRUSH)true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
//			COLORREF bkColor = ::GetBkColor(dc);
//			COLORREF brushColor = my::getBrushColor(brush);
//			MY_TRACE(_T("brush = 0x%08X, bkColor = 0x%08X, brushColor = 0x%08X\n"), brush, bkColor, brushColor);

			HTHEME theme = g_skin.getTheme(Dark::THEME_CTLCOLOR);
			int partId = g_skin.getCtlColorPartId(message);
			int stateId = 0;

			// 読み取り専用または無効状態のエディットボックスの場合は WM_CTLCOLORSTATIC が送られてくる。
			// しかし、エディットボックスのスキンは WM_CTLCOLOREDIT で処理したい。
			// したがって、コントロールがエディットボックスかどうか調べて、
			// エディットボックスだった場合は WM_CTLCOLOREDIT に変更する。

			TCHAR className[MAX_PATH] = {};
			::GetClassName(control, className, MAX_PATH);
			if (::lstrcmpi(className, WC_EDIT) == 0)
			{
				// エディットボックスだった。
				partId = g_skin.getCtlColorPartId(WM_CTLCOLOREDIT);

				if (enable)
				{
					// 読み取り専用のエディットボックスだった。
					stateId = Dark::CTLCOLOR_READONLY;
					::SetProp(control, _T("DarkenWindow.State"), (HANDLE)ETS_READONLY);
//					MY_TRACE(_T("ETS_READONLY 0x%08X\n"), control);
				}
				else
				{
					// 無効状態のエディットボックスだった。
					stateId = Dark::CTLCOLOR_DISABLED;
					::SetProp(control, _T("DarkenWindow.State"), (HANDLE)ETS_DISABLED);
//					MY_TRACE(_T("ETS_DISABLED 0x%08X\n"), control);
				}
			}
			else if (!enable)
			{
				// 無効状態のスタティックコントロールだった。
				stateId = Dark::CTLCOLOR_DISABLED;
			}
			else if (brush != (HBRUSH)::GetStockObject(DC_BRUSH))
			{
				if (::GetWindowLong(control, GWL_STYLE) & SS_SUNKEN && ::GetWindowTextLength(control) == 0)
				{
					// カラーダイアログのスタティックコントロールだった。
				}
				else
				{
					// 通常状態のスタティックコントロールだった。
					stateId = Dark::CTLCOLOR_NORMAL;
				}
			}

			Dark::StatePtr state = g_skin.getState(theme, partId, stateId);
			if (!state)
				return (LRESULT)brush;

			return (LRESULT)state->getFillBrush();
		}
	case WM_NOTIFY:
		{
			NMHDR* nm = (NMHDR*)lParam;

			TCHAR className[MAX_PATH] = {};
			::GetClassName(nm->hwndFrom, className, MAX_PATH);
//			MY_TRACE_TSTR(className);

			if (::lstrcmpi(className, WC_BUTTON) == 0)
				return 0; // ボタンの場合は何もしない。

			switch (nm->code)
			{
			case NM_CUSTOMDRAW:
				{
					Renderer* renderer = getRenderer(nm->hwndFrom);
					if (renderer)
						return renderer->CustomDraw(wndProc, hwnd, message, wParam, lParam);

					break;
				}
			}

			break;
		}
	case WM_NCRBUTTONUP:
		{
			MY_TRACE(_T("WM_NCRBUTTONUP, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n"), hwnd, message, wParam, lParam);

			if (wParam == HTMENU)
			{
				showSkinSelector(hwnd);

				return 0;
			}

			break;
		}
	case WM_CONTEXTMENU:
		{
			MY_TRACE(_T("WM_CONTEXTMENU, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n"), hwnd, message, wParam, lParam);

			UINT ht = (UINT)::SendMessage(hwnd, WM_NCHITTEST, 0, lParam);

			if (ht == HTCAPTION && ::GetKeyState(VK_CONTROL) < 0 && ::GetKeyState(VK_LWIN) < 0)
			{
				showSkinSelector(hwnd);

				return 0;
			}

			break;
		}
	}

	LRESULT result = 0;
	Dispatcher* dispatcher = getDispatcher();
	State oldState = dispatcher->m_currentState;
	dispatcher->setCurrentState(hwnd, message, wParam, lParam);
	Renderer* renderer = getRenderer(hwnd);
	if (renderer)
		result = renderer->CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
	else
		result = true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
	dispatcher->setCurrentState(oldState);
	return result;
}

//---------------------------------------------------------------------

IMPLEMENT_HOOK_PROC(HICON, WINAPI, LoadIconA, (HINSTANCE instance, LPCSTR iconName))
{
	if ((uintptr_t)iconName <= 0x0000FFFF || ::IsBadReadPtr(iconName, 1))
		MY_TRACE(_T("LoadIconA(0x%08X, %d)\n"), instance, iconName);
	else
		MY_TRACE(_T("LoadIconA(0x%08X, %hs)\n"), instance, iconName);

	return true_LoadIconA(instance, iconName);
}

IMPLEMENT_HOOK_PROC(HICON, WINAPI, LoadIconW, (HINSTANCE instance, LPCWSTR iconName))
{
	if ((uintptr_t)iconName <= 0x0000FFFF || ::IsBadReadPtr(iconName, 1))
		MY_TRACE(_T("LoadIconW(0x%08X, %d)\n"), instance, iconName);
	else
		MY_TRACE(_T("LoadIconW(0x%08X, %ws)\n"), instance, iconName);

	return true_LoadIconW(instance, iconName);
}

IMPLEMENT_HOOK_PROC(HANDLE, WINAPI, LoadImageA, (HINSTANCE instance, LPCSTR name, UINT type, int cx, int cy, UINT flags))
{
	if ((uintptr_t)name <= 0x0000FFFF || ::IsBadReadPtr(name, 1))
	{
		MY_TRACE(_T("LoadImageA(0x%08X, %d)\n"), instance, name);
	}
	else
	{
//		MY_TRACE(_T("LoadImageA(0x%08X, %hs)\n"), instance, name);

		if (instance == ::GetModuleHandle(0) && ::StrStrIA(name, "ICON_"))
		{
			MY_TRACE(_T("AviUtl のアイコンを書き換えます %hs, 0x%08X\n"), name, flags);

			// マニフェストを切り替えます。
			// なるべく早いタイミングで実行しないといけないので、
			// ここで処理しています。
			actctx.init();

			char name2[MAX_PATH] = {};
			::StringCbCopyA(name2, sizeof(name2), "AVIUTL_");
			::StringCbCatA(name2, sizeof(name2), name);

			HANDLE result = true_LoadImageA(g_instance, name2, type, cx, cy, flags);
			g_skin.addDrawIconData((HICON)result, (_bstr_t)name2);
			return result;
		}
		else if (instance == ::GetModuleHandle(_T("exedit.auf")) && ::StrStrIA(name, "ICON_"))
		{
			MY_TRACE(_T("拡張編集のアイコンを書き換えます %hs, 0x%08X\n"), name, flags);

			char name2[MAX_PATH] = {};
			::StringCbCopyA(name2, sizeof(name2), "EXEDIT_");
			::StringCbCatA(name2, sizeof(name2), name);

			HANDLE result = true_LoadImageA(g_instance, name2, type, cx, cy, flags);
			g_skin.addDrawIconData((HICON)result, (_bstr_t)name2);
			return result;
		}
	}

	return true_LoadImageA(instance, name, type, cx, cy, flags);
}

IMPLEMENT_HOOK_PROC(HANDLE, WINAPI, LoadImageW, (HINSTANCE instance, LPCWSTR name, UINT type, int cx, int cy, UINT flags))
{
	if ((uintptr_t)name <= 0x0000FFFF || ::IsBadReadPtr(name, 1))
		MY_TRACE(_T("LoadImageW(0x%08X, %d)\n"), instance, name);
	else
		MY_TRACE(_T("LoadImageW(0x%08X, %ws)\n"), instance, name);

	return true_LoadImageW(instance, name, type, cx, cy, flags);
}

IMPLEMENT_HOOK_PROC(BOOL, WINAPI, DrawIconEx, (HDC dc, int x, int y, HICON icon, int w, int h, UINT step, HBRUSH brush, UINT flags))
{
	icon = g_skin.getDrawIcon(icon);
	return true_DrawIconEx(dc, x, y, icon, w, h, step, brush, flags);
}

IMPLEMENT_HOOK_PROC(HMENU, WINAPI, LoadMenuA, (HINSTANCE instance, LPCSTR menuName))
{
	MY_TRACE(_T("LoadMenuA(0x%08X, 0x%08X)\n"), instance, menuName);

	if (::GetModuleHandle(_T("exedit.auf")))
	{
		DETACH_HOOK_PROC2(LoadMenuA);

		ExEdit::initExEdit();
	}

	return true_LoadMenuA(instance, menuName);
}

//---------------------------------------------------------------------

IMPLEMENT_HOOK_PROC(HTHEME, WINAPI, OpenThemeData, (HWND hwnd, LPCWSTR classList))
{
	HTHEME theme = true_OpenThemeData(hwnd, classList);
	MY_TRACE(_T("OpenThemeData(0x%08X, %ws) => 0x%08X\n"), hwnd, classList, theme);
	return theme;
}

IMPLEMENT_HOOK_PROC(HTHEME, WINAPI, OpenThemeDataForDpi, (HWND hwnd, LPCWSTR classList, UINT dpi))
{
	HTHEME theme = true_OpenThemeDataForDpi(hwnd, classList, dpi);
	MY_TRACE(_T("OpenThemeDataForDpi(0x%08X, %ws, %d) => 0x%08X\n"), hwnd, classList, dpi, theme);
	return theme;
}

IMPLEMENT_HOOK_PROC(HTHEME, WINAPI, OpenThemeDataEx, (HWND hwnd, LPCWSTR classList, DWORD flags))
{
	HTHEME theme = true_OpenThemeDataEx(hwnd, classList, flags);
	MY_TRACE(_T("OpenThemeDataEx(0x%08X, %ws, 0x%08X) => 0x%08X\n"), hwnd, classList, flags, theme);
	return theme;
}

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, SetWindowTheme, (HWND hwnd, LPCWSTR subAppName, LPCWSTR subIdList))
{
	MY_TRACE(_T("SetWindowTheme(0x%08X, %ws, %ws)\n"), hwnd, subAppName, subIdList);
	return true_SetWindowTheme(hwnd, subAppName, subIdList);
}

//---------------------------------------------------------------------

IMPLEMENT_HOOK_PROC_NULL(BOOL, WINAPI, loudness_TextOutA, (HDC dc, int x, int y, LPCSTR text, int c))
{
	HTHEME theme = g_skin.getTheme(Dark::THEME_STATIC);

	return g_skin.onExtTextOut(theme, dc, STAT_TEXT, 0, x, y, 0, 0, (_bstr_t)text, c, 0);
//	return true_loudness_TextOutA(dc, x, y, text, c);
}

//---------------------------------------------------------------------

LRESULT WINAPI onNcPaint(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);

//			HDC dc = ::GetDCEx(hwnd, 0, DCX_WINDOW | DCX_CLIPSIBLINGS | DCX_CACHE);
//			HDC dc = ::GetDCEx(hwnd, 0, DCX_WINDOW | DCX_CLIPSIBLINGS | DCX_PARENTCLIP);
			HDC dc = ::GetWindowDC(hwnd);
			RECT rc; ::GetWindowRect(hwnd, &rc);
			POINT origin = { rc.left, rc.top };
			::OffsetRect(&rc, -origin.x, -origin.y);
#if 0
			{
				HWND buddy = ::GetWindow(hwnd, GW_HWNDNEXT);
				if (hwnd == (HWND)::SendMessage(buddy, UDM_GETBUDDY, 0, 0))
				{
					RECT rc; ::GetWindowRect(buddy, &rc);
					::OffsetRect(&rc, -origin.x, -origin.y);
					HRGN rgn = ::CreateRectRgnIndirect(&rc);
					::ExtSelectClipRgn(dc, rgn, RGN_DIFF);
					::DeleteObject(rgn);
				}
			}
#endif
			DWORD style = GetWindowStyle(hwnd);
			DWORD exStyle = GetWindowExStyle(hwnd);
			HTHEME theme = g_skin.getTheme(Dark::THEME_WINDOW);

			if (exStyle & WS_EX_WINDOWEDGE)
			{
				int partId = Dark::WINDOW_WINDOWEDGE;
				int stateId = 0;
				g_skin.drawBackground(dc, theme, partId, stateId, &rc);
				::InflateRect(&rc, -2, -2);
			}
			else if (style & WS_BORDER)
			{
				int partId = Dark::WINDOW_BORDER;
				int stateId = 0;
				g_skin.drawBackground(dc, theme, partId, stateId, &rc);
				::InflateRect(&rc, -1, -1);
			}

			if (exStyle & WS_EX_STATICEDGE)
			{
				int partId = Dark::WINDOW_STATICEDGE;
				int stateId = 0;
				g_skin.drawBackground(dc, theme, partId, stateId, &rc);
				::InflateRect(&rc, -1, -1);
			}

			if (exStyle & WS_EX_CLIENTEDGE)
			{
				int partId = Dark::WINDOW_CLIENTEDGE;
				int stateId = 0;
				g_skin.drawBackground(dc, theme, partId, stateId, &rc);
				//my::frameRect(dc, &rc, RGB(255, 0, 0), 2);
				::InflateRect(&rc, -2, -2);
			}

			::ReleaseDC(hwnd, dc);

			return 0;
		}
	}

	return 1;
}

//---------------------------------------------------------------------
