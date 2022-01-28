#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

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
#if 1
	RECT rc2 = *rc;

	switch (partId)
	{
	case TVP_TREEITEM: // = 1,
		{
			switch (stateId)
			{
			case TREIS_NORMAL: // = 1,
			case TREIS_HOT: // = 2,
			case TREIS_DISABLED: // = 4,
				{
					my::fillRect_Dialog(dc, &rc2);
					return S_OK;
				}
			case TREIS_SELECTED: // = 3,
			case TREIS_SELECTEDNOTFOCUS: // = 5,
			case TREIS_HOTSELECTED: // = 6,
				{
					my::fillRect(dc, &rc2, my::getFillColor_Window_Selected());
					return S_OK;
				}
			}

			break;
		}
	case TVP_GLYPH: // = 2,
		{
			WCHAR text = L'\x0036';
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case GLPS_CLOSED: text = L'\x0034'; break;
			case GLPS_OPENED: text = L'\x0036'; break;
			}

			my::fillRect(dc, &rc2, my::getFillColor_Dialog());
			my::drawShadowIcon_Dialog(dc, &rc2, text, format);
			return S_OK;
		}
	case TVP_HOTGLYPH: // = 4,
		{
			WCHAR text = L'\x0036';
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case HGLPS_CLOSED: text = L'\x0034'; break;
			case HGLPS_OPENED: text = L'\x0036'; break;
			}

			my::fillRect(dc, &rc2, my::getFillColor_Dialog());
			my::drawShadowIcon_Dialog_Hot(dc, &rc2, text, format);
			return S_OK;
		}
	case TVP_BRANCH: // = 3,
		{
			break;
		}
	}
#endif
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
#if 1
	RECT rc2 = *rc;
	UINT format = textFlags | DT_NOCLIP;

	switch (partId)
	{
	case TVP_TREEITEM: // = 1,
		{
			DTTOPTS op = { sizeof(op) };
//			op.dwFlags = DTT_TEXTCOLOR | DTT_BORDERCOLOR | DTT_SHADOWCOLOR | DTT_SHADOWTYPE | DTT_SHADOWOFFSET | DTT_BORDERSIZE | DTT_APPLYOVERLAY | DTT_GLOWSIZE;
			op.crText = my::getForeTextColor_Dialog();
			op.crBorder = RGB(0x00, 0x00, 0x00);
			op.crShadow = my::getBackTextColor_Dialog();
			op.iTextShadowType = TST_SINGLE;
			op.iTextShadowType = TST_CONTINUOUS;
			op.ptShadowOffset.x = 1;
			op.ptShadowOffset.y = 1;
			op.iBorderSize = 1;
			op.fApplyOverlay = TRUE;
			op.iGlowSize = 1;

			int bkMode = ::SetBkMode(dc, OPAQUE);
			int bkColor = ::SetBkColor(dc, my::getFillColor_Dialog());

			switch (stateId)
			{
			case TREIS_SELECTED: // = 3,
			case TREIS_SELECTEDNOTFOCUS: // = 5,
			case TREIS_HOTSELECTED: // = 6,
			case TREIS_DISABLED: // = 4,
				{
					::SetBkColor(dc, my::getFillColor_Window_Selected());
					break;
				}
			}

			switch (stateId)
			{
			case TREIS_NORMAL: // = 1,
			case TREIS_SELECTED: // = 3,
			case TREIS_SELECTEDNOTFOCUS: // = 5,
				{
					my::drawShadowText2(dc, text, c, rc, format,
						my::getForeTextColor_Dialog(), my::getBackTextColor_Dialog());
					break;
				}
			case TREIS_HOT: // = 2,
			case TREIS_HOTSELECTED: // = 6,
				{
					my::drawShadowText2(dc, text, c, rc, format,
						my::getForeTextColor_Dialog_Hot(), my::getBackTextColor_Dialog());
					break;
				}
			case TREIS_DISABLED: // = 4,
				{
					my::drawShadowText2(dc, text, c, rc, format,
						my::getForeTextColor_Dialog_Disabled(), my::getBackTextColor_Dialog());
					break;
				}
			}

			::SetBkMode(dc, bkMode);
			::SetBkColor(dc, bkColor);

			return S_OK;
//			return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
//			return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, &op);
		}
	case TVP_GLYPH: // = 2,
	case TVP_BRANCH: // = 3,
	case TVP_HOTGLYPH: // = 4,
		{
			break;
		}
	}
#endif
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
#if 0
	switch (message)
	{
	case WM_ERASEBKGND:
		{
			HDC dc = (HDC)wParam;
			RECT rc; ::GetClientRect(hwnd, &rc);
			my::fillRect(dc, &rc, my::getFillColor_Dialog());
			return TRUE;
		}
	case WM_PAINT:
		{
			LRESULT result = true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
			HDC dc = ::GetDC(hwnd);
			HBRUSH brush = ::CreateSolidBrush(my::getFillColor_Dialog());
			HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, brush);
			HTREEITEM ti = TreeView_GetFirstVisible(hwnd);
			while (ti)
			{
				RECT rc; TreeView_GetItemRect(hwnd, ti, &rc, FALSE);
				::ExtFloodFill(dc, rc.left, rc.top, RGB(0xff, 0xff, 0xff), FLOODFILLSURFACE);
				ti = TreeView_GetNextVisible(hwnd, ti);
			}
			::SelectObject(dc, oldBrush);
			::DeleteObject(brush);
			::ReleaseDC(hwnd, dc);
			return result;
		}
	}
#endif
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

LRESULT TreeViewRenderer::CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
#if 1
	NMTVCUSTOMDRAW* cd = (NMTVCUSTOMDRAW*)lParam;

	switch (cd->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:
		{
			return CDRF_NOTIFYITEMDRAW;
		}
	case CDDS_ITEMPREPAINT:
		{
			cd->clrText = RGB(0xff, 0xff, 0xff);
			cd->clrTextBk = my::getFillColor_Dialog();
			return CDRF_NEWFONT;
		}
	}
#endif
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int TreeViewRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("TreeViewRenderer::FillRect()\n"));

	my::fillRect_Dialog(dc, rc);
	return TRUE;
//	return true_FillRect(dc, rc, brush);
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
