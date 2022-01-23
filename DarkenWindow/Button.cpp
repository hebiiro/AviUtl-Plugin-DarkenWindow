#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//--------------------------------------------------------------------

HRESULT ButtonThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("ButtonThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	RECT rc2 = *rc;

	switch (partId)
	{
/*
	case BP_USERBUTTON: // = 5,
	case BP_COMMANDLINK: // = 6,
	case BP_COMMANDLINKGLYPH: // = 7,
	case BP_RADIOBUTTON_HCDISABLED: // = 8,
	case BP_CHECKBOX_HCDISABLED: // = 9,
	case BP_GROUPBOX_HCDISABLED: // = 10,
	case BP_PUSHBUTTONDROPDOWN: // = 11,
*/
	case BP_PUSHBUTTON: // = 1,
		{
			my::fillRect_Dialog(dc, &rc2);

			switch (stateId)
			{
			case PBS_NORMAL: // = 1,
			case PBS_DISABLED: // = 4,
			case PBS_DEFAULTED: // = 5,
			case PBS_DEFAULTED_ANIMATING: // = 6,
				{
					return S_OK;
				}
			case PBS_HOT: // = 2,
				{
					my::drawDoubleEdge_Etched(dc, &rc2);
					return S_OK;
				}
			case PBS_PRESSED: // = 3,
				{
					my::frameRect_Button(dc, &rc2);
//					my::drawDoubleEdge_Sunken(dc, &rc2);
					return S_OK;
				}
			}

			break;
		}
	case BP_RADIOBUTTON: // = 2,
		{
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case RBS_UNCHECKEDNORMAL: // = 1,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					return S_OK;
				}
			case RBS_CHECKEDNORMAL: // = 5,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case RBS_UNCHECKEDHOT: // = 2,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					return S_OK;
				}
			case RBS_CHECKEDHOT: // = 6,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case RBS_UNCHECKEDPRESSED: // = 3,
				{
					::OffsetRect(&rc2, 1, 1);
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					return S_OK;
				}
			case RBS_CHECKEDPRESSED: // = 7,
				{
					::OffsetRect(&rc2, 1, 1);
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case RBS_UNCHECKEDDISABLED: // = 4,
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x63'), format);
					return S_OK;
				}
			case RBS_CHECKEDDISABLED: // = 8,
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x63'), format);
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			}

			break;
		}
	case BP_CHECKBOX: // = 3,
		{
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case CBS_UNCHECKEDNORMAL: // = 1,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					return S_OK;
				}
			case CBS_CHECKEDNORMAL: // = 5,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case CBS_MIXEDNORMAL: // = 9,
			case CBS_IMPLICITNORMAL: // = 13,
			case CBS_EXCLUDEDNORMAL: // = 17,
				break;
			case CBS_UNCHECKEDHOT: // = 2,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					return S_OK;
				}
			case CBS_CHECKEDHOT: // = 6,
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case CBS_MIXEDHOT: // = 10,
			case CBS_IMPLICITHOT: // = 14,
			case CBS_EXCLUDEDHOT: // = 18,
				break;
			case CBS_UNCHECKEDPRESSED: // = 3,
				{
					::OffsetRect(&rc2, 1, 1);
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					return S_OK;
				}
			case CBS_CHECKEDPRESSED: // = 7,
				{
					::OffsetRect(&rc2, 1, 1);
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x63'), format);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case CBS_MIXEDPRESSED: // = 11,
			case CBS_IMPLICITPRESSED: // = 15,
			case CBS_EXCLUDEDPRESSED: // = 19,
				break;
			case CBS_UNCHECKEDDISABLED: // = 4,
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x63'), format);
					return S_OK;
				}
			case CBS_CHECKEDDISABLED: // = 8,
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x63'), format);
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case CBS_MIXEDDISABLED: // = 12,
			case CBS_IMPLICITDISABLED: // = 16,
			case CBS_EXCLUDEDDISABLED: // = 20,
				break;
			}

			break;
		}
	case BP_GROUPBOX: // = 4,
		{
			switch (stateId)
			{
			case GBS_NORMAL:
			case GBS_DISABLED:
				{
					my::drawDoubleEdge_Etched(dc, &rc2);
					return S_OK;
				}
			}

			break;
		}
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT ButtonThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
//	MY_TRACE(_T("ButtonThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	RECT rc2 = *rc;

	switch (partId)
	{
/*
	case BP_USERBUTTON: // = 5,
	case BP_COMMANDLINK: // = 6,
	case BP_COMMANDLINKGLYPH: // = 7,
	case BP_RADIOBUTTON_HCDISABLED: // = 8,
	case BP_CHECKBOX_HCDISABLED: // = 9,
	case BP_GROUPBOX_HCDISABLED: // = 10,
	case BP_PUSHBUTTONDROPDOWN: // = 11,
*/
	case BP_PUSHBUTTON: // = 1,
		{
			switch (stateId)
			{
			case PBS_NORMAL: // = 1,
			case PBS_DEFAULTED: // = 5,
			case PBS_DEFAULTED_ANIMATING: // = 6,
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case PBS_DISABLED: // = 4,
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case PBS_PRESSED: // = 3,
				{
					::OffsetRect(&rc2, 1, 1);
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case PBS_HOT: // = 2,
				{
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			}

			break;
		}
	case BP_RADIOBUTTON: // = 2,
		{
			my::fillRect_Dialog(dc, &rc2);

			switch (stateId)
			{
			case RBS_UNCHECKEDNORMAL: // = 1,
			case RBS_CHECKEDNORMAL: // = 5,
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case RBS_UNCHECKEDHOT: // = 2,
			case RBS_CHECKEDHOT: // = 6,
				{
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case RBS_UNCHECKEDPRESSED: // = 3,
			case RBS_CHECKEDPRESSED: // = 7,
				{
					::OffsetRect(&rc2, 1, 1);
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case RBS_UNCHECKEDDISABLED: // = 4,
			case RBS_CHECKEDDISABLED: // = 8,
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			}

			break;
		}
	case BP_CHECKBOX: // = 3,
		{
			my::fillRect_Dialog(dc, &rc2);

			switch (stateId)
			{
			case CBS_UNCHECKEDNORMAL: // = 1,
			case CBS_CHECKEDNORMAL: // = 5,
			case CBS_MIXEDNORMAL: // = 9,
			case CBS_IMPLICITNORMAL: // = 13,
			case CBS_EXCLUDEDNORMAL: // = 17,
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case CBS_UNCHECKEDHOT: // = 2,
			case CBS_CHECKEDHOT: // = 6,
			case CBS_MIXEDHOT: // = 10,
			case CBS_IMPLICITHOT: // = 14,
			case CBS_EXCLUDEDHOT: // = 18,
				{
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case CBS_UNCHECKEDPRESSED: // = 3,
			case CBS_CHECKEDPRESSED: // = 7,
			case CBS_MIXEDPRESSED: // = 11,
			case CBS_IMPLICITPRESSED: // = 15,
			case CBS_EXCLUDEDPRESSED: // = 19,
				{
					::OffsetRect(&rc2, 1, 1);
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case CBS_UNCHECKEDDISABLED: // = 4,
			case CBS_CHECKEDDISABLED: // = 8,
			case CBS_MIXEDDISABLED: // = 12,
			case CBS_IMPLICITDISABLED: // = 16,
			case CBS_EXCLUDEDDISABLED: // = 20,
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			}

			break;
		}
	case BP_GROUPBOX: // = 4,
		{
			switch (stateId)
			{
			case GBS_NORMAL:
			case GBS_DISABLED:
				{
					break;
				}
			}

			break;
		}
	}

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT ButtonThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("ButtonThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT ButtonThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("ButtonThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT ButtonRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("ButtonRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	{
		LRESULT result = my::onNcPaint(wndProc, hwnd, message, wParam, lParam);
		if (!result) return result;
	}

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int ButtonRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("ButtonRenderer::FillRect()\n"));

	return true_FillRect(dc, rc, brush);
}

BOOL ButtonRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("ButtonRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL ButtonRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("ButtonRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL ButtonRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ButtonRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL ButtonRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("ButtonRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL ButtonRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ButtonRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL ButtonRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("ButtonRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL ButtonRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("ButtonRenderer::ExtTextOutW()\n"));

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ButtonRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ButtonRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
