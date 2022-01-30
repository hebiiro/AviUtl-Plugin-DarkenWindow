#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//---------------------------------------------------------------------

// デバッグ用コールバック関数。デバッグメッセージを出力する
void ___outputLog(LPCTSTR text, LPCTSTR output)
{
	::OutputDebugString(output);
}

//---------------------------------------------------------------------

HINSTANCE g_instance = 0;
HWND g_exeditObjectDialog = 0;

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
	ATTACH_HOOK_PROC(DialogBoxParamA);
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
//	if (wndProc != (WNDPROC)::GetClassLong(hwnd, GCL_WNDPROC))
//		return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);

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
			TCHAR className[MAX_PATH] = {};
			::GetClassName(hwnd, className, MAX_PATH);
			//MY_TRACE_TSTR(className);

			if (::lstrcmpi(className, _T("AviUtl")) == 0)
			{
				static BOOL isInited = FALSE;
				if (!isInited)
				{
					isInited = TRUE;

					// 最初の AviUtl ウィンドウ作成時にテーマフックをセットする。
					initThemeHook(hwnd);
				}
			}
#if 0
			else if (::lstrcmpi(className, _T("ExtendedFilterClass")) == 0)
			{
				MY_TRACE(_T("拡張編集をフックします\n"));

				g_exeditObjectDialog = hwnd;
				HMODULE exedit_auf = ::GetModuleHandle(_T("exedit.auf"));
				true_exedit_00030500 = (Type_exedit_00030500)((DWORD)exedit_auf + 0x00030500);
				true_exedit_000305E0 = (Type_exedit_000305E0)((DWORD)exedit_auf + 0x000305E0);

				DetourTransactionBegin();
				DetourUpdateThread(::GetCurrentThread());

				ATTACH_HOOK_PROC(exedit_00030500);
				ATTACH_HOOK_PROC(exedit_000305E0);

				if (DetourTransactionCommit() == NO_ERROR)
				{
					MY_TRACE(_T("API フックに成功しました\n"));
				}
				else
				{
					MY_TRACE(_T("API フックに失敗しました\n"));
				}
			}
#endif
			break;
		}
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSTATIC:
		{
			//MY_TRACE(_T("WM_CTLCOLOR, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n"), hwnd, message, wParam, lParam);

			HDC dc = (HDC)wParam;
			HWND control = (HWND)lParam;
			//UINT id = ::GetDlgCtrlID(control);
			//MY_TRACE_HEX(id);
			DWORD exStyle = ::GetWindowLong(control, GWL_EXSTYLE);

			HBRUSH brush = (HBRUSH)true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
			COLORREF bkColor = ::GetBkColor(dc);
			//MY_TRACE(_T("brush = 0x%08X, bkColor = 0x%08X\n"), brush, bkColor);

			// 拡張編集の「色の選択」ダイアログのコントロールを塗りつぶさないようにするため、拡張スタイルをチェックする。
			if (bkColor == ::GetSysColor(COLOR_BTNFACE) || !(exStyle & (WS_EX_STATICEDGE | WS_EX_CLIENTEDGE)))
			{
				static HBRUSH g_brush = ::CreateSolidBrush(my::getFillColor_Dialog());
				::SetTextColor(dc, my::getForeTextColor_Dialog());
				::SetBkColor(dc, my::getFillColor_Dialog());
				return (LRESULT)g_brush;
			}

			return (LRESULT)brush;
		}
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
		{
//			MY_TRACE(_T("WM_CTLCOLOREDIT\n"));

			HDC dc = (HDC)wParam;

			HBRUSH brush = (HBRUSH)true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
//			COLORREF bkColor = ::GetBkColor(dc);
			BOOL enable = ::IsWindowEnabled((HWND)lParam);

			if (brush == (HBRUSH)(COLOR_BTNFACE + 1))
//			if (bkColor == ::GetSysColor(COLOR_BTNFACE))
			{
				static HBRUSH g_brush = ::CreateSolidBrush(my::getFillColor_Dialog());
				::SetTextColor(dc, enable ? my::getForeTextColor_Dialog() : my::getForeTextColor_Dialog_Disabled());
				::SetBkColor(dc, my::getFillColor_Dialog());
				return (LRESULT)g_brush;
			}
			else
			{
				static HBRUSH g_brush = ::CreateSolidBrush(my::getFillColor_Window());
				::SetTextColor(dc, enable ? my::getForeTextColor_Window() : my::getForeTextColor_Window_Disabled());
				::SetBkColor(dc, my::getFillColor_Window());
				return (LRESULT)g_brush;
			}
		}
	case WM_CTLCOLORSCROLLBAR:
		{
			//MY_TRACE(_T("WM_CTLCOLORSCROLLBAR, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n"), hwnd, message, wParam, lParam);

			HDC dc = (HDC)wParam;
			HWND control = (HWND)lParam;

			static HBRUSH g_brush = ::CreateSolidBrush(my::getFillColor_Separator());
			return (LRESULT)g_brush;
		}
	case WM_NOTIFY:
		{
			if (wndProc != (WNDPROC)::GetClassLong(hwnd, GCL_WNDPROC))
				break;

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
		if (instance == ::GetModuleHandle(_T("exedit.auf")) && ::StrStrIA(name, "ICON_"))
		{
//			MY_TRACE(_T("アイコンを置き換えます\n"));
			instance = g_instance;
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

IMPLEMENT_HOOK_PROC_NULL(void, CDECL, exedit_00030500, ())
{
	MY_TRACE(_T("exedit_00030500()\n"));

	true_exedit_00030500();
}

HWND getComboBox()
{
	for (UINT i = 8200; i >= 8100; i--)
	{
		// ウィンドウハンドルを取得する。
		HWND hwnd = ::GetDlgItem(g_exeditObjectDialog, i);

		// コンボボックスかどうかクラス名で調べる。
		TCHAR className[MAX_PATH] = {};
		::GetClassName(hwnd, className, MAX_PATH);
		if (::lstrcmpi(className, WC_COMBOBOX) != 0) continue;

		if (::IsWindowVisible(hwnd)) // ウィンドウが可視なら
		{
			// ID - 2 のウィンドウを返す。
			return ::GetDlgItem(g_exeditObjectDialog, i - 2);
		}
	}

	return 0;
}

IMPLEMENT_HOOK_PROC_NULL(BOOL, CDECL, exedit_000305E0, (int objectIndex))
{
	MY_TRACE(_T("exedit_000305E0(0x%08X)\n"), objectIndex);

//	::SendMessage(g_exeditObjectDialog, WM_SETREDRAW, FALSE, 0);
	BOOL result = true_exedit_000305E0(objectIndex);
//	::SendMessage(g_exeditObjectDialog, WM_SETREDRAW, TRUE, 0);
//	::RedrawWindow(g_exeditObjectDialog, NULL, NULL, RDW_ERASE | RDW_FRAME | RDW_INVALIDATE | RDW_ALLCHILDREN);

	HWND combobox = getComboBox();
	MY_TRACE_HEX(combobox);
	::SendMessage(g_exeditObjectDialog, WM_CTLCOLOREDIT, 0, (LPARAM)combobox);

	return result;
}

//---------------------------------------------------------------------
