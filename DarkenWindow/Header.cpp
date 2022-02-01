#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//--------------------------------------------------------------------

HRESULT HeaderThemeRenderer::onDrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc)
{
	RECT rc2 = *rc;
	int ix = -1, iy = -1;
	int ox = 1, oy = 1;
	UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	switch (partId)
	{
	case 0:
		{
			my::fillRect_Dialog(dc, &rc2);
			return S_OK;
		}
	case HP_HEADERITEM: // = 1,
		{
			my::fillRect_Dialog(dc, &rc2);

			switch (stateId)
			{
			case HIS_NORMAL: // = 1,
			case HIS_SORTEDNORMAL: // = 4,
			case HIS_ICONNORMAL: // = 7,
			case HIS_ICONSORTEDNORMAL: // = 10,
				{
					return S_OK;
				}
			case HIS_HOT: // = 2,
			case HIS_SORTEDHOT: // = 5,
			case HIS_ICONHOT: // = 8,
			case HIS_ICONSORTEDHOT: // = 11,
				{
					my::drawDoubleEdge_Etched(dc, &rc2);

					return S_OK;
				}
			case HIS_PRESSED: // = 3,
			case HIS_SORTEDPRESSED: // = 6,
			case HIS_ICONPRESSED: // = 9,
			case HIS_ICONSORTEDPRESSED: // = 12,
				{
					my::drawDoubleEdge_Sunken(dc, &rc2);

					return S_OK;
				}
			}

			break;
		}
	case HP_HEADERITEMLEFT: // = 2,
	case HP_HEADERITEMRIGHT: // = 3,
	case HP_HEADERDROPDOWN: // = 5,
	case HP_HEADERDROPDOWNFILTER: // = 6,
		{
			WCHAR text = L'\x0036';

			switch (stateId)
			{
			case HDDFS_NORMAL: // = 1,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, text, format);
					return S_OK;
				}
			case HDDFS_SOFTHOT: // = 2,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, text, format);
					return S_OK;
				}
			case HDDFS_HOT: // = 3,
				{
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, text, format);
					return S_OK;
				}
			}

			break;
		}
	case HP_HEADERSORTARROW: // = 4,
		{
			::InflateRect(&rc2, 2, 2); rc2.bottom += 2;

			switch (stateId)
			{
			case HSAS_SORTEDUP: // = 1,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, L'\x0036', format);
					return S_OK;
				}
			case HSAS_SORTEDDOWN: // = 2,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, L'\x0035', format);
					return S_OK;
				}
			}

			break;
		}
	case HP_HEADEROVERFLOW: // = 7,
		{
			break;
		}
	}

	return S_FALSE;
}

HRESULT HeaderThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
	MY_TRACE(_T("HeaderThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	if (S_OK == onDrawThemeBackground(theme, dc, partId, stateId, rc))
		return S_OK;

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT HeaderThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
	MY_TRACE(_T("HeaderThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	if (S_OK == onDrawThemeBackground(theme, dc, partId, stateId, rc))
		return S_OK;

	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

HRESULT HeaderThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("HeaderThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT HeaderThemeRenderer::DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options)
{
	MY_TRACE(_T("HeaderThemeRenderer::DrawThemeTextEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags);

	RECT rc2 = *rc;
	int ix = -1, iy = -1;
	int ox = 1, oy = 1;
	UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	switch (partId)
	{
	case HP_HEADERITEM: // = 1,
		{
			switch (stateId)
			{
			case HIS_NORMAL: // = 1,
			case HIS_SORTEDNORMAL: // = 4,
			case HIS_ICONNORMAL: // = 7,
			case HIS_ICONSORTEDNORMAL: // = 10,
				{
					my::drawShadowText_Dialog(dc, text, c, rc, textFlags);
					return S_OK;
				}
			case HIS_HOT: // = 2,
			case HIS_SORTEDHOT: // = 5,
			case HIS_ICONHOT: // = 8,
			case HIS_ICONSORTEDHOT: // = 11,
				{
					my::drawShadowText_Dialog_Hot(dc, text, c, rc, textFlags);
					return S_OK;
				}
			case HIS_PRESSED: // = 3,
			case HIS_SORTEDPRESSED: // = 6,
			case HIS_ICONPRESSED: // = 9,
			case HIS_ICONSORTEDPRESSED: // = 12,
				{
					my::drawShadowText_Dialog_Selected(dc, text, c, rc, textFlags);
					return S_OK;
				}
			}

			break;
		}
	case HP_HEADERITEMLEFT: // = 2,
	case HP_HEADERITEMRIGHT: // = 3,
	case HP_HEADERSORTARROW: // = 4,
	case HP_HEADERDROPDOWN: // = 5,
	case HP_HEADERDROPDOWNFILTER: // = 6,
	case HP_HEADEROVERFLOW: // = 7,
		{
			break;
		}
	}

	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

HRESULT HeaderThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("HeaderThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT HeaderThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("HeaderThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------
#if 0
LRESULT HeaderRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("HeaderRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	switch (message)
	{
	case WM_ERASEBKGND:
		{
			HDC dc = (HDC)wParam;
			RECT rc; ::GetClientRect(hwnd, &rc);
			my::fillRect_Dialog(dc, &rc);
			return TRUE;
		}
	}

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int HeaderRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("HeaderRenderer::FillRect()\n"));

	return true_FillRect(dc, rc, brush);
}

BOOL HeaderRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("HeaderRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL HeaderRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("HeaderRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL HeaderRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("HeaderRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL HeaderRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("HeaderRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL HeaderRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("HeaderRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL HeaderRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("HeaderRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL HeaderRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("HeaderRenderer::ExtTextOutW(%d, %d, 0x%08X)\n"), x, y, options);

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL HeaderRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("HeaderRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}
#endif
//--------------------------------------------------------------------
