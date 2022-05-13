#include "pch.h"
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
HWND g_inProcessWindow = 0;

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
	GET_HOOK_PROC(user32, DialogBoxParamA);
	GET_HOOK_PROC(user32, DialogBoxParamW);

	// 2B = 7671339B - 76713370
	DWORD address1 = ::CallWindowProcW(getCallWindowProcInternal, 0, 0, 0, 0) - 0x2B;
	BYTE code[1] = {};
	::ReadProcessMemory(::GetCurrentProcess(), (LPCVOID)address1, code, sizeof(code), 0);
	if (code[0] == 0xCC) address1 += 0x01;
	DWORD address2 = (DWORD)user32 + 0x00043370;
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
	ATTACH_HOOK_PROC(DrawFrame);
	ATTACH_HOOK_PROC(DrawFrameControl);
	ATTACH_HOOK_PROC(FrameRect);
	ATTACH_HOOK_PROC(DrawEdge);
	ATTACH_HOOK_PROC(DrawFocusRect);
	ATTACH_HOOK_PROC(DrawStateW);
	ATTACH_HOOK_PROC(ExtTextOutW);
	ATTACH_HOOK_PROC(PatBlt);

	ATTACH_HOOK_PROC(CallWindowProcInternal);

//	ATTACH_HOOK_PROC(LoadIconA);
//	ATTACH_HOOK_PROC(LoadIconW);
	ATTACH_HOOK_PROC(LoadImageA);
//	ATTACH_HOOK_PROC(LoadImageW);
#if 0
	ATTACH_HOOK_PROC(DialogBoxParamA);
#endif
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
}

//---------------------------------------------------------------------

HWND createInProcessWindow()
{
	MY_TRACE(_T("createInProcessWindow()\n"));

	WNDCLASS wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wc.hCursor = ::LoadCursor(0, IDC_ARROW);
	wc.lpfnWndProc = ::DefWindowProc;
	wc.hInstance = g_instance;
	wc.lpszClassName = _T("DarkenWindow");
	::RegisterClass(&wc);

	HWND hwnd = ::CreateWindowEx(
		0,
		_T("DarkenWindow"),
		_T("DarkenWindow"),
		WS_POPUP,
		0, 0, 0, 0,
		0, 0, g_instance, 0);

	return hwnd;
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
			_tsetlocale(LC_ALL, _T(""));

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

			termHook();

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
			if (wndProc != (WNDPROC)::GetClassLong(hwnd, GCL_WNDPROC))
				break;

			TCHAR className[MAX_PATH] = {};
			::GetClassName(hwnd, className, MAX_PATH);
			//MY_TRACE_TSTR(className);

			if (::lstrcmpi(className, _T("AviUtl")) == 0)
			{
				g_skin.setDwm(hwnd, FALSE);
			}
			else if (::lstrcmpi(className, _T("ExtendedFilterClass")) == 0)
			{
				MY_TRACE(_T("拡張編集をフックします\n"));

				g_skin.setDwm(hwnd, FALSE);

				g_auin.init();

				DWORD exedit = (DWORD)::GetModuleHandle(_T("exedit.auf"));

				Exedit::g_font = (HFONT*)(exedit + 0x00167D84);

				hookAbsoluteCall(exedit + 0x0003833E, Exedit::drawRootText);
				hookAbsoluteCall(exedit + 0x0003836A, Exedit::drawRootEdge);

				hookAbsoluteCall(exedit + 0x00037CFF, Exedit::drawLayerText);
				hookAbsoluteCall(exedit + 0x00037D46, Exedit::drawLayerEdge);

				hookCall(exedit + 0x000380DF, Exedit::drawTimelineLongGuage);
				hookCall(exedit + 0x000381D7, Exedit::drawTimelineShortGuage);
				hookCall(exedit + 0x000381A2, Exedit::drawTimelineTime);

				hookAbsoluteCall(exedit + 0x00038538, Exedit::fillLayerBackground);
				hookAbsoluteCall(exedit + 0x0003860E, Exedit::fillLayerBackground);
				hookAbsoluteCall(exedit + 0x000386E4, Exedit::fillGroupBackground);

				hookCall(exedit + 0x00038845, Exedit::drawLayerLeft);
				hookCall(exedit + 0x000388AA, Exedit::drawLayerRight);
				hookCall(exedit + 0x00038871, Exedit::drawLayerTop);
				hookCall(exedit + 0x000388DA, Exedit::drawLayerBottom);
				hookCall(exedit + 0x00037A1F, Exedit::drawLayerSeparator);
#if 0
				DetourTransactionBegin();
				DetourUpdateThread(::GetCurrentThread());

//				ATTACH_HOOK_PROC(exedit_00030500);
//				ATTACH_HOOK_PROC(exedit_000305E0);

				if (DetourTransactionCommit() == NO_ERROR)
				{
					MY_TRACE(_T("API フックに成功しました\n"));
				}
				else
				{
					MY_TRACE(_T("API フックに失敗しました\n"));
				}
#endif
				g_skin.reloadExeditSettings();
			}
			else if (::GetWindowLong(hwnd, GWL_STYLE) & WS_CAPTION)
			{
				g_skin.setDwm(hwnd, FALSE);
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
				if (wndProc != (WNDPROC)::GetClassLongA(hwnd, GCL_WNDPROC) &&
					wndProc != (WNDPROC)::GetClassLongW(hwnd, GCL_WNDPROC) &&
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
			::GetClassName(hwnd, className, MAX_PATH);
			if (::lstrcmpi(className, WC_EDIT) == 0)
			{
				// エディットボックスだった。
				partId = g_skin.getCtlColorPartId(WM_CTLCOLOREDIT);

				if (enable)
				{
					// 読み取り専用のエディットボックスだった。
					stateId = Dark::CTLCOLOR_READONLY;
					::SetProp(control, _T("DarkenWindow.State"), (HANDLE)Dark::CTLCOLOR_READONLY);
//					MY_TRACE(_T("ETS_READONLY 0x%08X\n"), control);
				}
				else
				{
					// 無効状態のエディットボックスだった。
					stateId = Dark::CTLCOLOR_DISABLED;
					::SetProp(control, _T("DarkenWindow.State"), (HANDLE)Dark::CTLCOLOR_DISABLED);
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
				// 通常状態のスタティックコントロールだった。
				stateId = Dark::CTLCOLOR_NORMAL;
			}

			Dark::StatePtr state = g_skin.getState(theme, partId, stateId);
			if (!state)
				return (LRESULT)brush;

			return (LRESULT)state->getFillBrush();
		}
	case WM_NOTIFY:
		{
			NMHDR* nm = (NMHDR*)lParam;

			switch (nm->code)
			{
			case NM_CUSTOMDRAW:
				{
#if 0
					TCHAR className[MAX_PATH] = {};
					::GetClassName(nm->hwndFrom, className, MAX_PATH);
					MY_TRACE_TSTR(className);
#endif
					Renderer* renderer = getRenderer(nm->hwndFrom);
					if (renderer)
						return renderer->CustomDraw(wndProc, hwnd, message, wParam, lParam);

					break;
				}
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
	if ((DWORD)iconName <= 0x0000FFFF || ::IsBadReadPtr(iconName, 1))
		MY_TRACE(_T("LoadIconA(0x%08X, %d)\n"), instance, iconName);
	else
		MY_TRACE(_T("LoadIconA(0x%08X, %hs)\n"), instance, iconName);

	return true_LoadIconA(instance, iconName);
}

IMPLEMENT_HOOK_PROC(HICON, WINAPI, LoadIconW, (HINSTANCE instance, LPCWSTR iconName))
{
	if ((DWORD)iconName <= 0x0000FFFF || ::IsBadReadPtr(iconName, 1))
		MY_TRACE(_T("LoadIconW(0x%08X, %d)\n"), instance, iconName);
	else
		MY_TRACE(_T("LoadIconW(0x%08X, %ws)\n"), instance, iconName);

	return true_LoadIconW(instance, iconName);
}

IMPLEMENT_HOOK_PROC(HANDLE, WINAPI, LoadImageA, (HINSTANCE instance, LPCSTR name, UINT type, int cx, int cy, UINT flags))
{
	if ((DWORD)name <= 0x0000FFFF || ::IsBadReadPtr(name, 1))
	{
		MY_TRACE(_T("LoadImageA(0x%08X, %d)\n"), instance, name);
	}
	else
	{
//		MY_TRACE(_T("LoadImageA(0x%08X, %hs)\n"), instance, name);

		if (instance == ::GetModuleHandle(0) && ::StrStrIA(name, "ICON_"))
		{
			static BOOL isInited = FALSE;
			if (!isInited)
			{
				isInited = TRUE;

				// ウィンドウを作成する。
				g_inProcessWindow = createInProcessWindow();
				MY_TRACE_HEX(g_inProcessWindow);

				// テーマフックをセットする。
				initThemeHook(g_inProcessWindow);

				// アイコンを書き換えないといけないのでこのタイミングでスキンを初期化する。
				g_skin.init(g_instance, g_inProcessWindow);
				g_skin.reloadSettings(TRUE);
			}

			MY_TRACE(_T("AviUtl のアイコンを書き換えます %hs, 0x%08X\n"), name, flags);

			char name2[MAX_PATH] = {};
			::StringCbCopyA(name2, sizeof(name2), "AVIUTL_");
			::StringCbCatA(name2, sizeof(name2), name);

			return g_skin.editIcon((HICON)true_LoadImageA(g_instance, name2, type, cx, cy, flags));
		}
		else if (instance == ::GetModuleHandle(_T("exedit.auf")) && ::StrStrIA(name, "ICON_"))
		{
			MY_TRACE(_T("拡張編集のアイコンを書き換えます %hs, 0x%08X\n"), name, flags);

			char name2[MAX_PATH] = {};
			::StringCbCopyA(name2, sizeof(name2), "EXEDIT_");
			::StringCbCatA(name2, sizeof(name2), name);

			return g_skin.editIcon((HICON)true_LoadImageA(g_instance, name2, type, cx, cy, flags));
		}
	}

	return true_LoadImageA(instance, name, type, cx, cy, flags);
}

IMPLEMENT_HOOK_PROC(HANDLE, WINAPI, LoadImageW, (HINSTANCE instance, LPCWSTR name, UINT type, int cx, int cy, UINT flags))
{
	if ((DWORD)name <= 0x0000FFFF || ::IsBadReadPtr(name, 1))
		MY_TRACE(_T("LoadImageW(0x%08X, %d)\n"), instance, name);
	else
		MY_TRACE(_T("LoadImageW(0x%08X, %ws)\n"), instance, name);

	return true_LoadImageW(instance, name, type, cx, cy, flags);
}

IMPLEMENT_HOOK_PROC_NULL(INT_PTR, WINAPI, DialogBoxParamA, (HINSTANCE instance, LPCSTR templateName, HWND parent, DLGPROC dialogFunc, LPARAM initParam))
{
	if ((DWORD)templateName <= 0x0000FFFF || ::IsBadReadPtr(templateName, 1))
	{
		MY_TRACE(_T("DialogBoxParamA(0x%08X, %d)\n"), instance, templateName);
	}
	else
	{
		MY_TRACE(_T("DialogBoxParamA(0x%08X, %hs)\n"), instance, templateName);
		if (instance == ::GetModuleHandle(_T("exedit.auf")))
		{
			MY_TRACE(_T("ダイアログを置き換えます\n"));
			instance = g_instance;
		}
	}

	return true_DialogBoxParamA(instance, templateName, parent, dialogFunc, initParam);
}

IMPLEMENT_HOOK_PROC_NULL(INT_PTR, WINAPI, DialogBoxParamW, (HINSTANCE instance, LPCWSTR templateName, HWND parent, DLGPROC dialogFunc, LPARAM initParam))
{
	if ((DWORD)templateName <= 0x0000FFFF || ::IsBadReadPtr(templateName, 1))
		MY_TRACE(_T("DialogBoxParamW(0x%08X, %d)\n"), instance, templateName);
	else
		MY_TRACE(_T("DialogBoxParamW(0x%08X, %ws)\n"), instance, templateName);

	return true_DialogBoxParamW(instance, templateName, parent, dialogFunc, initParam);
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
namespace Exedit {
//---------------------------------------------------------------------

HFONT* g_font = 0;

BOOL WINAPI drawRootText(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("drawRootText(0x%08X, %d, %d, 0x%08X)\n"), dc, x, y, options);

	WCHAR text2[MAX_PATH] = {};
	UINT c2 = ::MultiByteToWideChar(CP_ACP, 0, text, c, text2, MAX_PATH);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	if (g_skin.onExtTextOut(theme, dc, Dark::EXEDIT_ROOT, 0, x, y, options, rc, text2, c2, dx))
		return TRUE;

	return TRUE;
}

BOOL WINAPI drawRootEdge(HDC dc, LPRECT rc, UINT edge, UINT flags)
{
//	MY_TRACE(_T("drawRootEdge(0x%08X, 0x%08X, 0x%08X)\n"), dc, edge, flags);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	if (g_skin.onDrawThemeBackground(theme, dc, Dark::EXEDIT_ROOT, 0, rc))
		return TRUE;

	return TRUE;
}

BOOL WINAPI drawLayerText(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("drawLayerText(0x%08X, %d, %d, 0x%08X)\n"), dc, x, y, options);

	WCHAR text2[MAX_PATH] = {};
	UINT c2 = ::MultiByteToWideChar(CP_ACP, 0, text, c, text2, MAX_PATH);

	int stateId = Dark::EXEDIT_LAYER_ACTIVE;

	COLORREF bkColor = ::GetBkColor(dc);
//	MY_TRACE_HEX(bkColor);
	switch (bkColor)
	{
	case 0x00F0F0F0: stateId = Dark::EXEDIT_LAYER_ACTIVE; break;
	case 0x00CCCCCC: stateId = Dark::EXEDIT_LAYER_INACTIVE; break;
	}

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	g_skin.onDrawThemeBackground(theme, dc, Dark::EXEDIT_LAYER, stateId, rc);

	if (g_skin.onExtTextOut(theme, dc, Dark::EXEDIT_LAYER, stateId, x, y, options, rc, text2, c2, dx))
		return TRUE;

	return ::ExtTextOutA(dc, x, y, options, rc, text, c, dx);
}

BOOL WINAPI drawLayerEdge(HDC dc, LPRECT rc, UINT edge, UINT flags)
{
//	MY_TRACE(_T("drawLayerEdge(0x%08X, 0x%08X, 0x%08X)\n"), dc, edge, flags);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	if (g_skin.onDrawThemeBackground(theme, dc, Dark::EXEDIT_LAYEREDGE, 0, rc))
		return TRUE;

	return ::DrawEdge(dc, rc, edge, flags);
}

void drawTimelineLongGuage(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawTimelineLongGuage(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LONGGUAGE, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		RECT rc = { mx, my, mx + 1, ly };
		my::fillRect(dc, &rc, state->m_fillColor);
	}
}

void drawTimelineShortGuage(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawTimelineShortGuage(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_SHORTGUAGE, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		RECT rc = { mx, my, mx + 1, ly };
		my::fillRect(dc, &rc, state->m_fillColor);
	}
}

void drawTimelineTime(HDC dc, LPCSTR text, int x, int y, int w, int h, int scroll_x)
{
//	MY_TRACE(_T("drawTimelineTime(0x%08X, %hs, %d, %d, %d, %d, %d)\n"), dc, text, x, y, w, h, scroll_x);

	::SelectObject(dc, *g_font);

	TEXTMETRIC tm = {};
	::GetTextMetrics(dc, &tm);

	RECT rc = { x, y, x + w, y + h };
	::OffsetRect(&rc, tm.tmHeight / 4 + scroll_x, 0);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	g_skin.onDrawThemeText(theme, dc, Dark::EXEDIT_LONGGUAGE, 0,
		(_bstr_t)text, -1, DT_LEFT | DT_VCENTER | DT_SINGLELINE, &rc);
}

int WINAPI fillLayerBackground(HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("fillLayerBackground(0x%08X, 0x%08X)\n"), dc, brush);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	int stateId = 0;

	COLORREF color = my::getBrushColor(brush);
//	MY_TRACE_HEX(color);
	switch (color)
	{
	case 0x00CCCCCC: stateId = Dark::EXEDIT_LAYERBACKGROUND_INACTIVE; break;
	default: stateId = Dark::EXEDIT_LAYERBACKGROUND_ACTIVE; break;
	}

	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERBACKGROUND, stateId);
	if (state && state->m_fillColor != CLR_NONE)
		::SetDCBrushColor(dc, state->m_fillColor);

	return true_FillRect(dc, rc, (HBRUSH)::GetStockObject(DC_BRUSH));
}

int WINAPI fillGroupBackground(HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("fillGroupBackground(0x%08X, 0x%08X)\n"), dc, brush);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	int stateId = 0;

	COLORREF color = ::GetDCBrushColor(dc);
//	MY_TRACE_HEX(color);
	switch (color)
	{
	case 0x00DEDEDE: stateId = Dark::EXEDIT_GROUPBACKGROUND_ACTIVE_1; break;
	case 0x00CCCCCC: stateId = Dark::EXEDIT_GROUPBACKGROUND_ACTIVE_2; break;
	case 0x00BABABA: stateId = Dark::EXEDIT_GROUPBACKGROUND_ACTIVE_3; break;

	case 0x00BEBEBE: stateId = Dark::EXEDIT_GROUPBACKGROUND_INACTIVE_1; break;
	case 0x00B1B1B1: stateId = Dark::EXEDIT_GROUPBACKGROUND_INACTIVE_2; break;
	case 0x00A3A3A3: stateId = Dark::EXEDIT_GROUPBACKGROUND_INACTIVE_3; break;
	}

	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_GROUPBACKGROUND, stateId);
	if (state && state->m_fillColor != CLR_NONE)
		::SetDCBrushColor(dc, state->m_fillColor);

	return true_FillRect(dc, rc, brush);
//	return true_FillRect(dc, rc, (HBRUSH)::GetStockObject(DC_BRUSH));
}

void drawLayerLeft(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerLeft(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERLEFT, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, 1, ly - my, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

void drawLayerRight(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerRight(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERRIGHT, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, 1, ly - my, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

void drawLayerTop(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerTop(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERTOP, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, lx - mx, 1, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

void drawLayerBottom(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerBottom(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERBOTTOM, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, lx - mx, 1, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

void drawLayerSeparator(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerSeparator(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERSEPARATOR, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, 1, ly - my, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

//--------------------------------------------------------------------
} // namespace Exedit
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
