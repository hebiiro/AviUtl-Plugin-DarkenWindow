#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//--------------------------------------------------------------------

HRESULT EditThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("EditThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	// 非クライアント領域のクライアントエッジ。
	my::fillRect_Dialog(dc, rc);
	my::drawDoubleEdge_Sunken(dc, rc);
	return S_OK;
//	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT EditThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("EditThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT EditThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("EditThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT EditThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("EditThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT EditRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("EditRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	{
		LRESULT result = my::onNcPaint(wndProc, hwnd, message, wParam, lParam);
		if (!result) return result;
	}

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int EditRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("EditRenderer::FillRect()\n"));
#if 0
	if (renderer->m_currentState.m_message == WM_NCPAINT)
	{
		// 非クライアント領域の背景
		my::fillRect_Dialog(dc, rc);
		return TRUE;
	}
#endif
	return true_FillRect(dc, rc, brush);
}

BOOL EditRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("EditRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL EditRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("EditRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL EditRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("EditRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL EditRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
//	MY_TRACE(_T("EditRenderer::DrawEdge()\n"));
#if 0
	if (renderer->m_currentState.m_message == WM_NCPAINT)
	{
		// 非クライアント領域のクライアントエッジ
		my::drawDoubleEdge_Sunken(dc, rc);
		return TRUE;
	}
#endif
	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL EditRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("EditRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL EditRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("EditRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL EditRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("EditRenderer::ExtTextOutW(0x%08X, %d, %d, 0x%08X)\n"), dc, x, y, options);
#if 1
	if (options & (ETO_GLYPH_INDEX | ETO_IGNORELANGUAGE))
	{
		if (options & ETO_OPAQUE)
		{
			COLORREF color = ::GetBkColor(dc);
			if (color == ::GetSysColor(COLOR_HIGHLIGHT))
			{
				::SetBkColor(dc, my::getFillColor_Window_Selected());
				::SetTextColor(dc, my::getForeTextColor_Window());
			}
		}
		if (::IsWindowEnabled(currentState->m_hwnd))
			my::shadowTextOut_Window(dc, x, y, options, rc, text, c, dx);
		else
			my::shadowTextOut_Window_Disabled(dc, x, y, options, rc, text, c, dx);
		return TRUE;
	}
#endif
//	MY_TRACE(_T("EditRenderer::ExtTextOutW(0x%08X, %d, %d, 0x%08X) Begin\n"), dc, x, y, options);
	BOOL result = true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
//	MY_TRACE(_T("EditRenderer::ExtTextOutW(0x%08X, %d, %d, 0x%08X) End\n"), dc, x, y, options);
	return result;
}

BOOL EditRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("EditRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}
