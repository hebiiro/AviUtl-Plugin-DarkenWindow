#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//--------------------------------------------------------------------

HRESULT CommandModuleThemeRenderer::DrawThemeParentBackground(HWND hwnd, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("CommandModuleThemeRenderer::DrawThemeParentBackground(0x%08X, 0x%08X, (%d, %d, %d, %d))\n"),
		hwnd, dc, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeParentBackground(hwnd, dc, rc);
}

HRESULT CommandModuleThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
	MY_TRACE(_T("CommandModuleThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT CommandModuleThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
	MY_TRACE(_T("CommandModuleThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	{
		if (g_skin.onDrawThemeBackground(theme, dc, partId, stateId, rc))
			return S_OK;
	}

	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

HRESULT CommandModuleThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("CommandModuleThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT CommandModuleThemeRenderer::DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options)
{
	MY_TRACE(_T("CommandModuleThemeRenderer::DrawThemeTextEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags);

	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

HRESULT CommandModuleThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("CommandModuleThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT CommandModuleThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("CommandModuleThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------
