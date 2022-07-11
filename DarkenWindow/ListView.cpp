#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//--------------------------------------------------------------------

HRESULT ListViewThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("ListViewThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	// ここではホットアイテムの背景色しか描画できない。(コモンダイアログの場合)

	{
		if (g_skin.onDrawThemeBackground(theme, dc, partId, stateId, rc))
			return S_OK;
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT ListViewThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
//	MY_TRACE(_T("ListViewThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	// ここではホットアイテムの背景色しか描画できない。(コモンダイアログの場合)

	{
		if (g_skin.onDrawThemeBackground(theme, dc, partId, stateId, rc))
			return S_OK;
	}

	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

HRESULT ListViewThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("ListViewThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT ListViewThemeRenderer::DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options)
{
	MY_TRACE(_T("ListViewThemeRenderer::DrawThemeTextEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags);

	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

HRESULT ListViewThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("ListViewThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT ListViewThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("ListViewThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT ListViewRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("ListViewRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

LRESULT ListViewRenderer::CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("ListViewRenderer::CustomDraw()\n"));
#if 0
	NMLVCUSTOMDRAW* cd = (NMLVCUSTOMDRAW*)lParam;

	switch (cd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			return CDRF_NOTIFYITEMDRAW;
		}
	case CDDS_ITEMPREPAINT:
		{
			cd->clrText = my::getForeTextColor_Window();
			cd->clrTextBk = my::getFillColor_Window();
			return CDRF_NEWFONT;
		}
	}
#endif
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int ListViewRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	COLORREF color = my::getBrushColor(brush);
//	MY_TRACE(_T("ListViewRenderer::FillRect(%d, %d, %d, %d, 0x%08X, 0x%08X)\n"), rc->left, rc->top, rc->right, rc->bottom, brush, color);

	// 背景色を描画する。

	HTHEME theme = g_skin.getTheme(Dark::THEME_LISTVIEW);

	if (color == ::GetSysColor(COLOR_HIGHLIGHT))
	{
		if (g_skin.onDrawThemeBackground(theme, dc, LVP_LISTITEM, LISS_HOT, rc))
			return TRUE;
	}
	else if (color == ::GetSysColor(COLOR_3DFACE))
	{
		if (g_skin.onDrawThemeBackground(theme, dc, LVP_LISTITEM, LISS_SELECTEDNOTFOCUS, rc))
			return TRUE;
	}
	else
	{
		if (g_skin.onDrawThemeBackground(theme, dc, LVP_LISTITEM, LISS_NORMAL, rc))
			return TRUE;
	}

	return true_FillRect(dc, rc, brush);
}

BOOL ListViewRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("ListViewRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL ListViewRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("ListViewRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL ListViewRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ListViewRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL ListViewRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("ListViewRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL ListViewRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ListViewRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL ListViewRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("ListViewRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL ListViewRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("ListViewRenderer::ExtTextOutW(%d, %d, 0x%08X, 0x%08X, 0x%08X, %d, 0x%08X, 0x%08X)\n"), x, y, options, rc, text, c, ::GetTextColor(dc), ::GetBkColor(dc));
#if 1
	if (!(options & ETO_IGNORELANGUAGE))
	{
		HTHEME theme = g_skin.getTheme(Dark::THEME_LISTVIEW);

		if (options == ETO_OPAQUE)
		{
			// セパレータのカラーを指定する。

			Dark::StatePtr state = g_skin.getState(theme, LVP_LISTITEM, 0);

			if (state && state->m_fillColor != CLR_NONE)
			{
				::SetBkColor(dc, state->m_fillColor);
				return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
			}
		}

		int stateId = LISS_NORMAL;

		COLORREF bkColor = ::GetBkColor(dc);
		if (bkColor == ::GetSysColor(COLOR_HIGHLIGHT) || bkColor == CLR_NONE)
			stateId = LISS_HOT;

		if (g_skin.onExtTextOut(theme, dc, LVP_LISTITEM, stateId, x, y, options, rc, text, c, dx))
			return TRUE;
	}
#endif
	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ListViewRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ListViewRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
