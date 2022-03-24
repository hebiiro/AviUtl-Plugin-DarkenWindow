#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//--------------------------------------------------------------------

HRESULT TreeViewThemeRenderer::DrawThemeParentBackground(HWND hwnd, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("TreeViewThemeRenderer::DrawThemeParentBackground(0x%08X, 0x%08X, (%d, %d, %d, %d))\n"),
		hwnd, dc, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeParentBackground(hwnd, dc, rc);
}

HRESULT TreeViewThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("TreeViewThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	{
		if (g_skin.onDrawThemeBackground(theme, dc, partId, stateId, rc))
			return S_OK;
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT TreeViewThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
	MY_TRACE(_T("TreeViewThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

HRESULT TreeViewThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("TreeViewThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT TreeViewThemeRenderer::DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options)
{
//	MY_TRACE(_T("TreeViewThemeRenderer::DrawThemeTextEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags);

	if (textFlags & DT_CALCRECT)
		return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);

	{
		if (g_skin.onDrawThemeText(theme, dc, partId, stateId, text, c, textFlags, rc))
			return S_OK;
	}

	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

HRESULT TreeViewThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("TreeViewThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT TreeViewThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("TreeViewThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT TreeViewRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("TreeViewRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

LRESULT TreeViewRenderer::CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("TreeViewRenderer::CustomDraw()\n"));
#if 1
	// テキストの色と背景色を指定する。(選択アイテム以外)

	NMTVCUSTOMDRAW* cd = (NMTVCUSTOMDRAW*)lParam;

	switch (cd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			return CDRF_NOTIFYITEMDRAW;
		}
	case CDDS_ITEMPREPAINT:
		{
			HTHEME theme = g_skin.getTheme(Dark::THEME_TREEVIEW);
			Dark::StatePtr state = g_skin.getState(theme, TVP_TREEITEM, TREIS_NORMAL);

			if (state && state->m_fillColor != CLR_NONE)
				cd->clrTextBk = state->m_fillColor;
			if (state && state->m_textForeColor != CLR_NONE)
				cd->clrText = state->m_textForeColor;

			return CDRF_NEWFONT;
		}
	}
#endif
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int TreeViewRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("TreeViewRenderer::FillRect()\n"));

	// アイテムがない部分の背景色を描画する。

	HTHEME theme = g_skin.getTheme(Dark::THEME_TREEVIEW);
	Dark::StatePtr state = g_skin.getState(theme, TVP_TREEITEM, TREIS_NORMAL);

	if (state && state->m_fillColor != CLR_NONE)
	{
		my::fillRect(dc, rc, state->m_fillColor);
		return TRUE;
	}

	return true_FillRect(dc, rc, brush);
}

BOOL TreeViewRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("TreeViewRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL TreeViewRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("TreeViewRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL TreeViewRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("TreeViewRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL TreeViewRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("TreeViewRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL TreeViewRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("TreeViewRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL TreeViewRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("TreeViewRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL TreeViewRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("TreeViewRenderer::ExtTextOutW(%d, %d, 0x%08X)\n"), x, y, options);

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL TreeViewRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("TreeViewRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
