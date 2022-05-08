#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//--------------------------------------------------------------------

HRESULT ListBoxThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("ListBoxThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT ListBoxThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
	MY_TRACE(_T("ListBoxThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

HRESULT ListBoxThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("ListBoxThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT ListBoxThemeRenderer::DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options)
{
	MY_TRACE(_T("ListBoxThemeRenderer::DrawThemeTextEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags);

	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

HRESULT ListBoxThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("ListBoxThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT ListBoxThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("ListBoxThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT ListBoxRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("ListBoxRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	{
		LRESULT result = onNcPaint(wndProc, hwnd, message, wParam, lParam);
		if (!result) return result;
	}

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int ListBoxRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("ListBoxRenderer::FillRect()\n"));

	return true_FillRect(dc, rc, brush);
}

BOOL ListBoxRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("ListBoxRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL ListBoxRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("ListBoxRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL ListBoxRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ListBoxRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL ListBoxRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("ListBoxRenderer::DrawEdge(0x%08X, 0x%08X), 0x%08X\n"), edge, flags, currentState->m_message);

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL ListBoxRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ListBoxRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL ListBoxRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
//	MY_TRACE(_T("ListBoxRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL ListBoxRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	MY_TRACE(_T("ListBoxRenderer::ExtTextOutW(%d, %d, 0x%08X, 0x%08X, 0x%08X, %d, 0x%08X)\n"), x, y, options, rc, text, c, ::GetBkColor(dc));
#if 1
	if (options & (ETO_GLYPH_INDEX | ETO_IGNORELANGUAGE))
	{
		HTHEME theme = g_skin.getTheme(Dark::THEME_LISTBOX);

		if (options & ETO_OPAQUE)
		{
			COLORREF color = ::GetBkColor(dc);
			COLORREF hilightColor = ::GetSysColor(COLOR_HIGHLIGHT);
			MY_TRACE(_T("color = 0x%08X, 0x%08X\n"), color, hilightColor);
			if (color == hilightColor)
			{
				if (g_skin.onExtTextOut(theme, dc, EP_EDITTEXT, ETS_SELECTED, x, y, options, rc, text, c, dx))
					return TRUE;
			}
		}

		if (::IsWindowEnabled(currentState->m_hwnd))
		{
			if (g_skin.onExtTextOut(theme, dc, EP_EDITTEXT, ETS_NORMAL, x, y, options, rc, text, c, dx))
				return TRUE;
		}
		else
		{
			if (g_skin.onExtTextOut(theme, dc, EP_EDITTEXT, ETS_DISABLED, x, y, options, rc, text, c, dx))
				return TRUE;
		}
	}
#endif
	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ListBoxRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ListBoxRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
