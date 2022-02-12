#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//--------------------------------------------------------------------

HRESULT ListViewThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
	MY_TRACE(_T("ListViewThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT ListViewThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
	MY_TRACE(_T("ListViewThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	RECT rc2 = *rc;

	switch (partId)
	{
	case LVP_LISTITEM: // = 1,
		{
			switch (stateId)
			{
			case LISS_NORMAL: // = 1,
			case LISS_DISABLED: // = 4,
				{
					my::fillRect_Dialog(dc, &rc2);
					return S_OK;
				}
			case LISS_HOT: // = 2,
			case LISS_SELECTED: // = 3,
			case LISS_SELECTEDNOTFOCUS: // = 5,
			case LISS_HOTSELECTED: // = 6,
				{
					my::fillRect_Window_Selected(dc, &rc2);
					return S_OK;
				}
			}

			break;
		}
	case LVP_LISTGROUP: // = 2,
	case LVP_LISTDETAIL: // = 3,
	case LVP_LISTSORTEDDETAIL: // = 4,
	case LVP_EMPTYTEXT: // = 5,
	case LVP_GROUPHEADER: // = 6,
	case LVP_GROUPHEADERLINE: // = 7,
	case LVP_EXPANDBUTTON: // = 8,
	case LVP_COLLAPSEBUTTON: // = 9,
	case LVP_COLUMNDETAIL: // = 10,
		{
			break;
		}
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

	RECT rc2 = *rc;
	int ix = -1, iy = -1;
	int ox = 1, oy = 1;
	UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

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

LRESULT ListViewRenderer::CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
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
//	MY_TRACE(_T("ListViewRenderer::FillRect(%d, %d, %d, %d, 0x%08X)\n"), rc->left, rc->top, rc->right, rc->bottom, brush);

	COLORREF color = my::getBrushColor(brush);

	if (color == ::GetSysColor(COLOR_HIGHLIGHT))
	{
		my::fillRect_Window_Selected(dc, rc);
		return TRUE;
	}
	else
	{
		my::fillRect_Dialog(dc, rc);
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
//	MY_TRACE(_T("ListViewRenderer::ExtTextOutW(%d, %d, 0x%08X)\n"), x, y, options);

	if (options & ETO_OPAQUE)
		::SetBkColor(dc, my::getFillColor_Gutter());
#if 0
	if (text)
	{
		my::shadowTextOut_Window(dc, x, y, options, rc, text, c, dx);
		return TRUE;
	}
#endif
	if (text)
	{
		::SetTextColor(dc, my::getForeTextColor_Dialog());
		::SetBkColor(dc, my::getFillColor_Dialog());
	}

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ListViewRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ListViewRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
