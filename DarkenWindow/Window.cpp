#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//--------------------------------------------------------------------

HRESULT WindowThemeRenderer::DrawThemeParentBackground(HWND hwnd, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("WindowThemeRenderer::DrawThemeParentBackground(0x%08X, 0x%08X, (%d, %d, %d, %d))\n"),
		hwnd, dc, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeParentBackground(hwnd, dc, rc);
}

HRESULT WindowThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("WindowThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	{
		if (g_skin.onDrawThemeBackground(theme, dc, partId, stateId, rc))
			return S_OK;
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT WindowThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
	MY_TRACE(_T("WindowThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

HRESULT WindowThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
//	MY_TRACE(_T("WindowThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	{
		if (g_skin.onDrawThemeText(theme, dc, partId, stateId, text, c, textFlags, rc))
			return S_OK;
	}

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT WindowThemeRenderer::DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options)
{
	MY_TRACE(_T("WindowThemeRenderer::DrawThemeTextEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags);

	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

HRESULT WindowThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("WindowThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT WindowThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("WindowThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT WindowRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("WindowRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	{
		LRESULT result = onNcPaint(wndProc, hwnd, message, wParam, lParam);
		if (!result) return result;
	}

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int WindowRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("WindowRenderer::FillRect(0x%08X)\n"), brush);

	return true_FillRect(dc, rc, brush);
}

BOOL WindowRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("WindowRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL WindowRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("WindowRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL WindowRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("WindowRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL WindowRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("WindowRenderer::DrawEdge(0x%08X, 0x%08X), 0x%08X\n"), edge, flags, currentState->m_message);

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL WindowRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("WindowRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL WindowRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
//	MY_TRACE(_T("WindowRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL WindowRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	MY_TRACE(_T("WindowRenderer::ExtTextOutW(%d, %d, 0x%08X, 0x%08X, 0x%08X, %d, 0x%08X)\n"), x, y, options, rc, text, c, ::GetBkColor(dc));

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL WindowRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("WindowRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
