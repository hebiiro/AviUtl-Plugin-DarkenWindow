#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//--------------------------------------------------------------------

HRESULT TrackBarThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("TrackBarThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	RECT rc2 = *rc;

	switch (partId)
	{
#if 0
	case TKP_TRACKVERT: // = 2,
	case TKP_TICS: // = 9,
	case TKP_TICSVERT: // = 10,
#endif
	case TKP_TRACK: // = 1,
		{
			my::fillRect_TrackBar_Channel(dc, &rc2);
			my::drawDoubleEdge_Sunken(dc, &rc2);
			return S_OK;
		}
#if 1
	case TKP_THUMB: // = 3,
	case TKP_THUMBBOTTOM: // = 4,
	case TKP_THUMBTOP: // = 5,
	case TKP_THUMBVERT: // = 6,
	case TKP_THUMBLEFT: // = 7,
	case TKP_THUMBRIGHT: // = 8,
		{
#if 0
			int x = rc2.left;
			int y = rc2.top;
			int w = rc2.right - rc2.left;
			int h = rc2.bottom - rc2.top;
			int cx = (rc2.left + rc2.right) / 2;

			POINT points[5];
			points[0].x = rc2.left;
			points[0].y = rc2.top;
			points[1].x = rc2.right;
			points[1].y = rc2.top;
			points[2].x = rc2.right;
			points[2].y = rc2.bottom - w / 2;
			points[3].x = cx;
			points[3].y = rc2.bottom;
			points[4].x = rc2.left;
			points[4].y = rc2.bottom - w / 2;
#endif
			COLORREF fillColor = RGB(0x33, 0x33, 0x33);
			COLORREF edgeColor = RGB(0x00, 0x00, 0x00);

			switch (stateId)
			{
			case TUS_NORMAL: // = 1,
				{
					fillColor = RGB(0x66, 0x66, 0xff);
					break;
				}
			case TUS_HOT: // = 2,
				{
					fillColor = RGB(0x66, 0xff, 0x66);
					//edgeColor = RGB(0x00, 0xff, 0x00);
					break;
				}
			case TUS_PRESSED: // = 3,
			case TUS_FOCUSED: // = 4,
				{
					fillColor = RGB(0xff, 0x66, 0x66);
					//edgeColor = RGB(0xff, 0x00, 0x00);
					break;
				}
			case TUS_DISABLED: // = 5,
				{
					fillColor = RGB(0x99, 0x99, 0x99);
					//edgeColor = RGB(0x66, 0x66, 0x66);
					break;
				}
			}

			my::drawRectangle(dc, &rc2, fillColor, edgeColor, 1);
			//my::drawDoubleEdge_Raised(dc, &rc2);
			return S_OK;
		}
#endif
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT TrackBarThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("TrackBarThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT TrackBarThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("TrackBarThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT TrackBarThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("TrackBarThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT TrackBarRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("TrackBarRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	switch (message)
	{
	case WM_ERASEBKGND:
		{
			return TRUE;
		}
	case WM_LBUTTONDOWN:
		{
			LRESULT result = true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);

			// コモンコントロールをバージョン 6 にすると、WM_LBUTTONDOWN で
			// SB_THUMBTRACK が送られてこないので手動で送る。
			int pos = ::SendMessage(hwnd, TBM_GETPOS, 0, 0);
			::SendMessage(::GetParent(hwnd), WM_HSCROLL, MAKEWPARAM(SB_THUMBTRACK, pos), (LPARAM)hwnd);

			return result;
		}
	}

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int TrackBarRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("TrackBarRenderer::FillRect()\n"));

	return true_FillRect(dc, rc, brush);
}

BOOL TrackBarRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("TrackBarRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL TrackBarRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("TrackBarRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL TrackBarRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("TrackBarRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL TrackBarRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("TrackBarRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL TrackBarRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("TrackBarRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL TrackBarRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("TrackBarRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL TrackBarRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
#if 0
	COLORREF bkColor = ::GetBkColor(dc);
//	MY_TRACE(_T("TrackBarRenderer::ExtTextOutW(%d, %d, 0x%08X), 0x%08X\n"), x, y, options, bkColor);
	if (bkColor == ::GetSysColor(COLOR_HIGHLIGHT))
	{
		::SetBkColor(dc, my::getFillColor_Dialog_Selected());
	}
#endif
	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL TrackBarRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("TrackBarRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
