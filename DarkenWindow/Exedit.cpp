#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//--------------------------------------------------------------------

LRESULT ExeditRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("ExeditRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);
#if 1
	if (wndProc != (WNDPROC)::GetClassLong(hwnd, GCL_WNDPROC))
		return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);

	switch (message)
	{
	case WM_ERASEBKGND:
		{
//			MY_TRACE(_T("WM_ERASEBKGND, 0x%08X, 0x%08X)\n"), wParam, lParam);

			// 「設定ダイアログ画面サイズ固定化プラグイン」を使用しているときはこの処理が必要。

			HDC dc = (HDC)wParam;
			RECT rc; ::GetClientRect(hwnd, &rc);

			HTHEME theme = g_skin.getTheme(Dark::THEME_WINDOW);
			if (g_skin.onDrawThemeBackground(theme, dc, Dark::WINDOW_DIALOGFACE, 0, &rc))
				return TRUE;

			break;
		}
	}
#endif
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int ExeditRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("ExeditRenderer::FillRect(0x%08X)\n"), brush);

	if (brush == (HBRUSH)(COLOR_BTNFACE + 1))
	{
		HTHEME theme = g_skin.getTheme(Dark::THEME_WINDOW);
		if (g_skin.onDrawThemeBackground(theme, dc, Dark::WINDOW_DIALOGFACE, 0, rc))
			return TRUE;
	}

	return true_FillRect(dc, rc, brush);
}

BOOL ExeditRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("ExeditRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL ExeditRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("ExeditRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL ExeditRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ExeditRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL ExeditRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("ExeditRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL ExeditRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ExeditRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL ExeditRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("ExeditRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL ExeditRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("ExeditRenderer::ExtTextOutW(%d, %d, 0x%08X, 0x%08X, 0x%08X, %d, 0x%08X)\n"), x, y, options, rc, text, c, ::GetBkColor(dc));

	// 「設定ダイアログ画面サイズ固定化プラグイン」が
	// FillSolidRect() を使用しているため以下の処理が必要。

	if (options == ETO_OPAQUE)
	{
		COLORREF color = ::GetBkColor(dc);
		MY_TRACE(_T("ExeditRenderer::ExtTextOutW(%d, %d, 0x%08X), 0x%08X\n"), x, y, options, color);

		if (color == ::GetSysColor(COLOR_BTNFACE))
		{
			HTHEME theme = g_skin.getTheme(Dark::THEME_WINDOW);
			if (g_skin.onDrawThemeBackground(theme, dc, Dark::WINDOW_DIALOGFACE, 0, rc))
				return TRUE;
		}
	}
	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ExeditRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ExeditRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
