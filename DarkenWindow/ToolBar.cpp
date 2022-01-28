#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//--------------------------------------------------------------------

HRESULT ToolBarThemeRenderer::DrawThemeParentBackground(HWND hwnd, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ToolBarThemeRenderer::DrawThemeParentBackground(0x%08X, 0x%08X, (%d, %d, %d, %d))\n"),
		hwnd, dc, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeParentBackground(hwnd, dc, rc);
}

HRESULT ToolBarThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
	MY_TRACE(_T("ToolBarThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	RECT rc2 = *rc;

	switch (partId)
	{
	case 0:
	case TP_BUTTON: // = 1,
		{
			switch (stateId)
			{
			case 0:
			case TS_NORMAL: // = 1,
			case TS_DISABLED: // = 4,
				{
					my::fillRect_Dialog(dc, &rc2);
					return S_OK;
				}
			case TS_HOT: // = 2,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Etched(dc, &rc2);
					return S_OK;
				}
			case TS_PRESSED: // = 3,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Sunken(dc, &rc2);
					return S_OK;
				}
			case TS_CHECKED: // = 5,
			case TS_HOTCHECKED: // = 6,
			case TS_NEARHOT: // = 7,
			case TS_OTHERSIDEHOT: // = 8,
				{
					break;
				}
			}

			break;
		}
	case TP_DROPDOWNBUTTON: // = 2,
	case TP_SPLITBUTTON: // = 3,
	case TP_SPLITBUTTONDROPDOWN: // = 4,
	case TP_SEPARATOR: // = 5,
	case TP_SEPARATORVERT: // = 6,
	case TP_DROPDOWNBUTTONGLYPH: // = 7,
		{
			switch (stateId)
			{
			case TS_NORMAL: // = 1,
			case TS_HOT: // = 2,
			case TS_PRESSED: // = 3,
			case TS_DISABLED: // = 4,
			case TS_CHECKED: // = 5,
			case TS_HOTCHECKED: // = 6,
			case TS_NEARHOT: // = 7,
			case TS_OTHERSIDEHOT: // = 8,
				{
					break;
				}
			}

			break;
		}
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT ToolBarThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
	MY_TRACE(_T("ToolBarThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

HRESULT ToolBarThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("ToolBarThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	RECT rc2 = *rc;

	switch (partId)
	{
	case 0:
	case TP_BUTTON: // = 1,
		{
			switch (stateId)
			{
			case TS_NORMAL: // = 1,
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case TS_HOT: // = 2,
				{
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case TS_PRESSED: // = 3,
				{
					my::drawShadowText_Dialog_Selected(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case TS_DISABLED: // = 4,
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case TS_CHECKED: // = 5,
			case TS_HOTCHECKED: // = 6,
			case TS_NEARHOT: // = 7,
			case TS_OTHERSIDEHOT: // = 8,
				{
					break;
				}
			}

			break;
		}
	case TP_DROPDOWNBUTTON: // = 2,
	case TP_SPLITBUTTON: // = 3,
	case TP_SPLITBUTTONDROPDOWN: // = 4,
	case TP_SEPARATOR: // = 5,
	case TP_SEPARATORVERT: // = 6,
	case TP_DROPDOWNBUTTONGLYPH: // = 7,
		{
			switch (stateId)
			{
			case TS_NORMAL: // = 1,
			case TS_HOT: // = 2,
			case TS_PRESSED: // = 3,
			case TS_DISABLED: // = 4,
			case TS_CHECKED: // = 5,
			case TS_HOTCHECKED: // = 6,
			case TS_NEARHOT: // = 7,
			case TS_OTHERSIDEHOT: // = 8,
				{
					break;
				}
			}

			break;
		}
	}

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT ToolBarThemeRenderer::DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options)
{
	MY_TRACE(_T("ToolBarThemeRenderer::DrawThemeTextEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags);

	RECT rc2 = *rc;
	int ix = -1, iy = -1;
	int ox = 1, oy = 1;
	UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

HRESULT ToolBarThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("ToolBarThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT ToolBarThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("ToolBarThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT ToolBarRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("ToolBarRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);
#if 0
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
#endif
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

LRESULT ToolBarRenderer::CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	NMLVCUSTOMDRAW* cd = (NMLVCUSTOMDRAW*)lParam;
/*
	switch (cd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			return CDRF_NOTIFYITEMDRAW;
		}
	case CDDS_ITEMPREPAINT:
		{
			cd->clrText = RGB(0xff, 0xff, 0xff);
			cd->clrTextBk = my::getFillColor_Window();
			return CDRF_NEWFONT;
		}
	}
*/
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int ToolBarRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ToolBarRenderer::FillRect(%d, %d, %d, %d)\n"), rc->left, rc->top, rc->right, rc->bottom);

	return true_FillRect(dc, rc, brush);
}

BOOL ToolBarRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("ToolBarRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL ToolBarRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("ToolBarRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL ToolBarRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ToolBarRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL ToolBarRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("ToolBarRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL ToolBarRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ToolBarRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL ToolBarRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("ToolBarRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL ToolBarRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	MY_TRACE(_T("ToolBarRenderer::ExtTextOutW(%d, %d, 0x%08X)\n"), x, y, options);

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ToolBarRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ToolBarRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
