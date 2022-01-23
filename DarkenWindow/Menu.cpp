#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

HRESULT MenuThemeRenderer::DrawThemeBackground(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCRECT pRect, LPCRECT pClipRect)
{
	RECT rc = *pRect;

	switch (iPartId)
	{
/*
	case MENU_MENUITEM_TMSCHEMA: // = 1,
	case MENU_MENUDROPDOWN_TMSCHEMA: // = 2,
	case MENU_MENUBARITEM_TMSCHEMA: // = 3,
	case MENU_MENUBARDROPDOWN_TMSCHEMA: // = 4,
	case MENU_CHEVRON_TMSCHEMA: // = 5,
	case MENU_SEPARATOR_TMSCHEMA: // = 6,
	case MENU_POPUPCHECK: // = 11,
	case MENU_POPUPCHECKBACKGROUND: // = 12,
*/
	case MENU_BARBACKGROUND: // = 7,
		{
//			MY_TRACE(_T("MENU_BARBACKGROUND, %d, %d, %d, %d\n"), pRect->left, pRect->top, pRect->right, pRect->bottom);

			rc.bottom += 1;
			if (pClipRect) my::clipRect(hdc, pClipRect);
			my::fillRect_Dialog(hdc, &rc);
			my::frameRect_Menu(hdc, &rc);
			return S_OK;
		}
	case MENU_POPUPBACKGROUND: // = 9, // ポップアップメニュー全体
		{
//			MY_TRACE(_T("MENU_POPUPBACKGROUND, %d, %d, %d, %d\n"), pRect->left, pRect->top, pRect->right, pRect->bottom);

			if (pClipRect) my::clipRect(hdc, pClipRect);
			my::fillRect_Dialog(hdc, &rc);
			return S_OK;
		}
	case MENU_POPUPBORDERS: // = 10, // ポップアップメニュー外側のボーダー
		{
			my::fillRect_Dialog(hdc, &rc);
			my::frameRect_Menu(hdc, &rc);
			return S_OK;
		}
	case MENU_POPUPGUTTER: // = 13, // テキスト左のアイコンの背景
		{
			my::fillRect_Gutter(hdc, &rc);
			return S_OK;
		}
	case MENU_BARITEM: // = 8,
		{
//			MY_TRACE(_T("MENU_BARITEM\n"));

			switch (iStateId)
			{
			case MBI_NORMAL:
				{
					return S_OK;
				}
			case MBI_DISABLED:
				{
					return S_OK;
				}
			case MBI_HOT:
				{
					my::drawAlphaRectangle_Hot(hdc, &rc);
					return S_OK;
				}
			case MBI_DISABLEDHOT:
				{
					my::drawAlphaRectangle_Hot(hdc, &rc);
					return S_OK;
				}
			case MBI_PUSHED:
				{
					my::drawAlphaRectangle_Selected(hdc, &rc);
					return S_OK;
				}
			case MBI_DISABLEDPUSHED:
				{
					my::drawAlphaRectangle_Selected(hdc, &rc);
					return S_OK;
				}
			}

			break;
		}
	case MENU_POPUPCHECK: // = 11,
		{
//			::InflateRect(&rc, 2, 2);
			::OffsetRect(&rc, 3, 0);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (iStateId)
			{
			case MC_CHECKMARKNORMAL:
				{
					my::drawShadowIcon_Dialog(hdc, &rc, _T('\x61'), format);
					return S_OK;
				}
			case MC_CHECKMARKDISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(hdc, &rc, _T('\x61'), format);
					return S_OK;
				}
			case MC_BULLETNORMAL:
				{
					my::drawShadowIcon_Dialog(hdc, &rc, _T('\x3D'), format);
					return S_OK;
				}
			case MC_BULLETDISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(hdc, &rc, _T('\x3D'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_POPUPCHECKBACKGROUND: // = 12,
		{
			return S_OK;
		}
	case MENU_POPUPITEM: // = 14,
		{
//			MY_TRACE(_T("MENU_POPUPITEM\n"));

			switch (iStateId)
			{
			case MPI_NORMAL:
			case MPI_DISABLED:
				{
					return S_OK;
				}
			case MPI_HOT:
			case MPI_DISABLEDHOT:
				{
					my::drawAlphaRectangle_Hot(hdc, &rc);
					return S_OK;
				}
			}

			break;
		}
	case MENU_POPUPSEPARATOR: // = 15,
		{
			::InflateRect(&rc, -2, -2);
			my::fillRect_Separator(hdc, &rc);
			return S_OK;
		}
	case MENU_POPUPSUBMENU: // = 16,
		{
			::InflateRect(&rc, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (iStateId)
			{
			case MSM_NORMAL:
				{
					my::drawShadowIcon_Dialog(hdc, &rc, _T('\x7E'), format);
					return S_OK;
				}
			case MSM_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(hdc, &rc, _T('\x7E'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_SYSTEMCLOSE: // = 17,
		{
			::InflateRect(&rc, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (iStateId)
			{
			case MSYSC_NORMAL:
				{
					my::drawShadowIcon_Dialog(hdc, &rc, _T('\x72'), format);
					return S_OK;
				}
			case MSYSC_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(hdc, &rc, _T('\x72'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_SYSTEMMAXIMIZE: // = 18,
		{
			::InflateRect(&rc, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (iStateId)
			{
			case MSYSMX_NORMAL:
				{
					my::drawShadowIcon_Dialog(hdc, &rc, _T('\x31'), format);
					return S_OK;
				}
			case MSYSMX_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(hdc, &rc, _T('\x31'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_SYSTEMMINIMIZE: // = 19,
		{
			::InflateRect(&rc, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (iStateId)
			{
			case MSYSMN_NORMAL:
				{
					my::drawShadowIcon_Dialog(hdc, &rc, _T('\x30'), format);
					return S_OK;
				}
			case MSYSMN_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(hdc, &rc, _T('\x30'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_SYSTEMRESTORE: // = 20,
		{
			::InflateRect(&rc, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (iStateId)
			{
			case MSYSR_NORMAL:
				{
					my::drawShadowIcon_Dialog(hdc, &rc, _T('\x32'), format);
					return S_OK;
				}
			case MSYSR_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(hdc, &rc, _T('\x32'), format);
					return S_OK;
				}
			}

			break;
		}
	}

	return true_DrawThemeBackground(hTheme, hdc, iPartId, iStateId, pRect, pClipRect);
}

HRESULT MenuThemeRenderer::DrawThemeText(HTHEME hTheme, HDC hdc, int iPartId, int iStateId, LPCWSTR pszText, int cchText, DWORD dwTextFlags, DWORD dwTextFlags2, LPCRECT pRect)
{
	RECT rc = *pRect;

	switch (iPartId)
	{
	case MENU_BARITEM: // = 8,
		{
			switch (iStateId)
			{
			case MBI_NORMAL:
				{
					my::drawShadowText_Dialog(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			case MBI_DISABLED:
				{
					my::drawShadowText_Dialog_Disabled(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			case MBI_HOT:
				{
					my::drawShadowText_Dialog(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			case MBI_DISABLEDHOT:
				{
					my::drawShadowText_Dialog_Disabled(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			case MBI_PUSHED:
				{
					my::drawShadowText_Dialog(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			case MBI_DISABLEDPUSHED:
				{
					my::drawShadowText_Dialog_Disabled(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			}

			break;
		}
	case MENU_POPUPITEM: // = 14,
		{
			switch (iStateId)
			{
			case MPI_NORMAL:
				{
					my::drawShadowText_Dialog(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			case MPI_DISABLED:
				{
					my::drawShadowText_Dialog_Disabled(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			case MPI_HOT:
				{
					my::drawShadowText_Dialog(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			case MPI_DISABLEDHOT:
				{
					my::drawShadowText_Dialog_Disabled(hdc, pszText, cchText, &rc, dwTextFlags);
					return S_OK;
				}
			}

			break;
		}
	}

	return true_DrawThemeText(hTheme, hdc, iPartId, iStateId, pszText, cchText, dwTextFlags, dwTextFlags2, pRect);
}

HRESULT MenuThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("MenuThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT MenuThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("MenuThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}
