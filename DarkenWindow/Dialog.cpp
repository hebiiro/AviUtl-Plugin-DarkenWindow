#include "pch.h"
#include "DarkenWindow.h"
#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//--------------------------------------------------------------------

LRESULT DialogRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("DialogRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), wndProc, hwnd, message, wParam, lParam);

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int DialogRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("DialogRenderer::FillRect(%d, %d, %d, %d)\n"), rc->left, rc->top, rc->right, rc->bottom);

	HINSTANCE instance = (HINSTANCE)::GetWindowLong(currentState->m_hwnd, GWL_HINSTANCE);
	DWORD style = ::GetWindowLong(currentState->m_hwnd, GWL_STYLE);
	if (instance == ::GetModuleHandle(_T("comdlg32.dll")) && style & WS_THICKFRAME)
	{
		// コンボボックス＆コンボリストボックス用。
		// WS_THICKFRAME がない場合は色の設定コモンダイアログなので何もしない。

		HTHEME theme = g_skin.getTheme(Dark::THEME_DIALOG);

		COLORREF color = my::getBrushColor(brush);
//		MY_TRACE_HEX(color);
		if (color == ::GetSysColor(COLOR_WINDOW))
		{
			if (g_skin.onDrawThemeBackground(theme, dc, WP_DIALOG, ETS_NORMAL, rc))
				return TRUE;
		}
		else
		{
			if (g_skin.onDrawThemeBackground(theme, dc, WP_DIALOG, ETS_SELECTED, rc))
				return TRUE;
		}
	}
#if 0
	else
	{
		HTHEME theme = g_skin.getTheme(Dark::THEME_DIALOG);

		if (g_skin.onDrawThemeBackground(theme, dc, Dark::WINDOW_DIALOGFACE, 0, rc))
			return TRUE;
	}
#endif

	return true_FillRect(dc, rc, brush);
}

BOOL DialogRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("DialogRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL DialogRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("DialogRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL DialogRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("DialogRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL DialogRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("DialogRenderer::DrawEdge(0x%08X, 0x%08X)\n"), edge, flags);

	if (edge == EDGE_SUNKEN)
	{
		if (g_skin.onDrawThemeBackground(g_skin.getTheme(Dark::THEME_WINDOW), dc, Dark::WINDOW_SUNKENEDGE, 0, rc))
		{
			if (flags & BF_ADJUST)
				::InflateRect(rc, -2, -2);

			return TRUE;
		}
	}
#if 0
	{
		HTHEME theme = g_skin.getTheme(Dark::THEME_DIALOG);

		if (g_skin.onDrawThemeBackground(theme, dc, Dark::WINDOW_WINDOWEDGE, 0, rc))
			return TRUE;
	}
#endif
	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL DialogRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("DialogRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL DialogRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("DialogRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL DialogRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	MY_TRACE(_T("DialogRenderer::ExtTextOutW(%d, %d, 0x%08X, 0x%08X, 0x%08X, %d, 0x%08X, 0x%08X, 0x%08X)\n"), x, y, options, rc, text, c, dx, ::GetBkColor(dc), ::GetTextColor(dc));

	if (options == ETO_OPAQUE)
	{
		HINSTANCE instance = (HINSTANCE)::GetWindowLong(currentState->m_hwnd, GWL_HINSTANCE);
		if (instance == ::GetModuleHandle(_T("comdlg32.dll")))
		{
			// コンボボックス＆コンボリストボックス用。

			HTHEME theme = g_skin.getTheme(Dark::THEME_DIALOG);

			COLORREF bkColor = ::GetBkColor(dc);
//			MY_TRACE_HEX(bkColor);
			if (bkColor == RGB(0xff, 0xff, 0xff))
			{
				if (g_skin.onExtTextOut(theme, dc, WP_DIALOG, ETS_NORMAL, x, y, options, rc, text, c, dx))
					return TRUE;
			}
			else
			{
				if (g_skin.onExtTextOut(theme, dc, WP_DIALOG, ETS_SELECTED, x, y, options, rc, text, c, dx))
					return TRUE;
			}
		}
	}

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL DialogRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("DialogRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
