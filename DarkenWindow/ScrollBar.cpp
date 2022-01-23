#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//--------------------------------------------------------------------

HRESULT ScrollBarThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("ScrollBarThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	RECT rc2 = *rc;
	int ix = -1, iy = -1;
	UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	switch (partId)
	{
	case SBP_ARROWBTN: // = 1, // 両サイドのボタン
		{
			switch (stateId)
			{
			case ABS_UPNORMAL: // = 1,
			case ABS_UPHOVER: // = 17,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog(dc, &rc2, L'\x0035', format);
					return S_OK;
				}
			case ABS_UPHOT: // = 2,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, L'\x0035', format);
					return S_OK;
				}
			case ABS_UPPRESSED: // = 3,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Sunken(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					::OffsetRect(&rc2, 0, 1);
					my::drawShadowIcon_Dialog_Selected(dc, &rc2, L'\x0035', format);
					return S_OK;
				}
			case ABS_UPDISABLED: // = 4,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, L'\x0035', format);
					return S_OK;
				}
			case ABS_DOWNNORMAL: // = 5,
			case ABS_DOWNHOVER: // = 18,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog(dc, &rc2, L'\x0036', format);
					return S_OK;
				}
			case ABS_DOWNHOT: // = 6,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, L'\x0036', format);
					return S_OK;
				}
			case ABS_DOWNPRESSED: // = 7,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Sunken(dc, &rc2);
					::OffsetRect(&rc2, 0, 1);
					my::drawShadowIcon_Dialog_Selected(dc, &rc2, L'\x0036', format);
					return S_OK;
				}
			case ABS_DOWNDISABLED: // = 8,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, L'\x0036', format);
					return S_OK;
				}
			case ABS_LEFTNORMAL: // = 9,
			case ABS_LEFTHOVER: // = 19,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog(dc, &rc2, L'\x0033', format);
					return S_OK;
				}
			case ABS_LEFTHOT: // = 10,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, L'\x0033', format);
					return S_OK;
				}
			case ABS_LEFTPRESSED: // = 11,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Sunken(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					::OffsetRect(&rc2, 1, 0);
					my::drawShadowIcon_Dialog_Selected(dc, &rc2, L'\x0033', format);
					return S_OK;
				}
			case ABS_LEFTDISABLED: // = 12,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, L'\x0033', format);
					return S_OK;
				}
			case ABS_RIGHTNORMAL: // = 13,
			case ABS_RIGHTHOVER: // = 20,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog(dc, &rc2, L'\x0034', format);
					return S_OK;
				}
			case ABS_RIGHTHOT: // = 14,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, L'\x0034', format);
					return S_OK;
				}
			case ABS_RIGHTPRESSED: // = 15,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Sunken(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					::OffsetRect(&rc2, 1, 0);
					my::drawShadowIcon_Dialog_Selected(dc, &rc2, L'\x0034', format);
					return S_OK;
				}
			case ABS_RIGHTDISABLED: // = 16,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, L'\x0034', format);
					return S_OK;
				}
			}

			break;
		}
	case SBP_THUMBBTNHORZ: // = 2, // 真ん中のつまみ
	case SBP_THUMBBTNVERT: // = 3, // 真ん中のつまみ
		{
			switch (stateId)
			{
			case SCRBS_NORMAL:
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					return S_OK;
				}
			case SCRBS_HOT:
			case SCRBS_HOVER:
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, L'\x007E', format);
					return S_OK;
				}
			case SCRBS_PRESSED:
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Selected(dc, &rc2, L'\x007E', format);
					return S_OK;
				}
			case SCRBS_DISABLED:
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Raised(dc, &rc2);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, L'\x007E', format);
					return S_OK;
				}
			}

			break;
		}
	case SBP_LOWERTRACKHORZ: // = 4, // つまみが動く範囲
	case SBP_UPPERTRACKHORZ: // = 5, // つまみが動く範囲
	case SBP_LOWERTRACKVERT: // = 6, // つまみが動く範囲
	case SBP_UPPERTRACKVERT: // = 7, // つまみが動く範囲
		{
//			my::fillRect_Gutter(dc, &rc2);
			my::fillRect_Separator(dc, &rc2);
			return S_OK;
		}
#if 0
	case SBP_GRIPPERHORZ: // = 8, // つまみのさらに真ん中
	case SBP_GRIPPERVERT: // = 9, // つまみのさらに真ん中
		{
			switch (stateId)
			{
			case SCRBS_NORMAL:
				{
					return S_OK;
				}
			case SCRBS_HOT:
				{
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, L'\x007E', format);
					return S_OK;
				}
			case SCRBS_HOVER:
				{
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, L'\x007E', format);
					return S_OK;
				}
			case SCRBS_PRESSED:
				{
					my::drawShadowIcon_Dialog_Selected(dc, &rc2, L'\x007E', format);
					return S_OK;
				}
			case SCRBS_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, L'\x007E', format);
					return S_OK;
				}
			}

			break;
		}
#endif
	case SBP_SIZEBOX: // = 10, // 不明 おそらく右下のコーナー
	case SBP_SIZEBOXBKGND: // = 11, // 不明
		{
//			my::fillRect_Gutter(dc, &rc2);
			my::fillRect_Dialog(dc, &rc2);
			my::drawShadowIcon_Dialog_Selected(dc, &rc2, L'\x002E', format);
			return S_OK;
		}
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT ScrollBarThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
//	MY_TRACE(_T("ScrollBarThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT ScrollBarThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("ScrollBarThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT ScrollBarThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("ScrollBarThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT ScrollBarRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	MY_TRACE(_T("ScrollBarRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int ScrollBarRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ScrollBarRenderer::FillRect()\n"));

	return true_FillRect(dc, rc, brush);
}

BOOL ScrollBarRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("ScrollBarRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL ScrollBarRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("ScrollBarRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL ScrollBarRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ScrollBarRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL ScrollBarRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("ScrollBarRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL ScrollBarRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ScrollBarRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL ScrollBarRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("ScrollBarRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL ScrollBarRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	MY_TRACE(_T("ScrollBarRenderer::ExtTextOutW()\n"));

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ScrollBarRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ScrollBarRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
