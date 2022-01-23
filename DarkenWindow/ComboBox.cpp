#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

//--------------------------------------------------------------------

HRESULT ComboBoxThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("ComboBoxThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	RECT rc2 = *rc;
	int ix = 0, iy = 0;
	int ox = 1, oy = 1;
	UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;
	WCHAR text = L'\x0036';

	switch (partId)
	{
/*
	case CP_DROPDOWNBUTTON: // =  1,
	case CP_BACKGROUND: // =  2,
	case CP_TRANSPARENTBACKGROUND: // =  3,
	case CP_DROPDOWNBUTTONLEFT: // =  7,
	case CP_CUEBANNER: // =  8,
	case CP_DROPDOWNITEM: // =  9,
*/
	case CP_BORDER: // =  4,
		{
			switch (stateId)
			{
			case CBXS_NORMAL: // =  1,
			case CBXS_DISABLED: // =  4,
				{
					my::fillRect_Gutter(dc, &rc2);
					return S_OK;
				}
			case CBXS_HOT: // =  2,
			case CBXS_PRESSED: // =  3,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Etched(dc, &rc2);
					return S_OK;
				}
			}

			break;
		}
	case CP_READONLY: // =  5,
		{
			switch (stateId)
			{
			case CBRO_NORMAL: // =  1,
			case CBRO_DISABLED: // =  4,
				{
					my::fillRect_Gutter(dc, &rc2);
					return S_OK;
				}
			case CBRO_HOT: // =  2,
			case CBRO_PRESSED: // =  3,
				{
					my::fillRect_Dialog(dc, &rc2);
					my::drawDoubleEdge_Etched(dc, &rc2);
					return S_OK;
				}
			}

			break;
		}
	case CP_DROPDOWNBUTTONRIGHT: // =  6,
		{
			//my::frameRect(dc, &rc2, RGB(0xff, 0x00, 0x00), 1);

			switch (stateId)
			{
			case CBXSR_NORMAL: // =  1,
				{
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog(dc, &rc2, text, format);
					return S_OK;
				}
			case CBXSR_HOT: // =  2,
				{
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Hot(dc, &rc2, text, format);
					return S_OK;
				}
			case CBXSR_DISABLED: // =  4,
				{
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, text, format);
					return S_OK;
				}
			case CBXSR_PRESSED: // =  3,
				{
					//::OffsetRect(&rc2, ox, oy);
					::InflateRect(&rc2, ix, iy);
					my::drawShadowIcon_Dialog_Selected(dc, &rc2, text, format);
					return S_OK;
				}
			}

			break;
		}
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT ComboBoxThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
//	MY_TRACE(_T("ComboBoxThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	RECT rc2 = *rc;

	switch (partId)
	{
/*
	case CP_DROPDOWNBUTTON: // =  1,
	case CP_BACKGROUND: // =  2,
	case CP_TRANSPARENTBACKGROUND: // =  3,
	case CP_BORDER: // =  4,
	case CP_DROPDOWNBUTTONRIGHT: // =  6,
	case CP_DROPDOWNBUTTONLEFT: // =  7,
	case CP_CUEBANNER: // =  8,
	case CP_DROPDOWNITEM: // =  9,
*/
	case CP_READONLY: // =  5,
		{
			switch (stateId)
			{
			case CBRO_NORMAL: // =  1,
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case CBRO_DISABLED: // =  4,
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case CBRO_PRESSED: // =  3,
				{
					::OffsetRect(&rc2, 1, 1);
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case CBRO_HOT: // =  2,
				{
					my::drawShadowText_Dialog_Hot(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			}

			break;
		}
	}

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT ComboBoxThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("ComboBoxThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT ComboBoxThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("ComboBoxThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

LRESULT ComboBoxRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("ComboBoxRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int ComboBoxRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("ComboBoxRenderer::FillRect()\n"));
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

BOOL ComboBoxRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("ComboBoxRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL ComboBoxRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("ComboBoxRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL ComboBoxRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ComboBoxRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL ComboBoxRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("ComboBoxRenderer::DrawEdge()\n"));
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

BOOL ComboBoxRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ComboBoxRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL ComboBoxRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("ComboBoxRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL ComboBoxRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("ComboBoxRenderer::ExtTextOutW()\n"));

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ComboBoxRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ComboBoxRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}

//--------------------------------------------------------------------
