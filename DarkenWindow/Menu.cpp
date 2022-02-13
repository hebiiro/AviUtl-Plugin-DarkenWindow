#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

class Clipper
{
private:
	HDC m_dc;
public:
	Clipper(HDC dc, LPCRECT rcClip)
		: m_dc(dc)
	{
		if (rcClip)
		{
			RECT rc = *rcClip;
			::LPtoDP(m_dc, (LPPOINT)&rc, 2);
			my::clipRect(m_dc, &rc);
		}
	}
	~Clipper()
	{
//		::SelectClipRgn(m_dc, NULL);
	}
};

HRESULT MenuThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
	MY_TRACE(_T("MenuThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	RECT rc2 = *rc;

	switch (partId)
	{
/*
	case MENU_MENUITEM_TMSCHEMA: // = 1,
	case MENU_MENUDROPDOWN_TMSCHEMA: // = 2,
	case MENU_MENUBARITEM_TMSCHEMA: // = 3,
	case MENU_MENUBARDROPDOWN_TMSCHEMA: // = 4,
	case MENU_CHEVRON_TMSCHEMA: // = 5,
	case MENU_SEPARATOR_TMSCHEMA: // = 6,
*/
	case MENU_BARBACKGROUND: // = 7,
		{
//			MY_TRACE(_T("MENU_BARBACKGROUND, %d, %d, %d, %d\n"), rc->left, rc->top, rc->right, rc->bottom);

			rc2.bottom += 1;
			Clipper clipper(dc, rcClip);
			my::fillRect_Dialog(dc, &rc2);
			my::frameRect_Menu(dc, &rc2);
			return S_OK;
		}
	case MENU_POPUPBACKGROUND: // = 9, // ポップアップメニュー全体
		{
//			MY_TRACE(_T("MENU_POPUPBACKGROUND, %d, %d, %d, %d\n"), rc->left, rc->top, rc->right, rc->bottom);

			Clipper clipper(dc, rcClip);
			my::fillRect_Dialog(dc, &rc2);
			return S_OK;
		}
	case MENU_POPUPBORDERS: // = 10, // ポップアップメニュー外側のボーダー
		{
			my::fillRect_Dialog(dc, &rc2);
			my::frameRect_Menu(dc, &rc2);
			return S_OK;
		}
	case MENU_POPUPGUTTER: // = 13, // テキスト左のアイコンの背景
		{
			my::fillRect_Gutter(dc, &rc2);
			return S_OK;
		}
	case MENU_BARITEM: // = 8,
		{
//			MY_TRACE(_T("MENU_BARITEM\n"));

			switch (stateId)
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
					my::drawAlphaRectangle_Hot(dc, &rc2);
					return S_OK;
				}
			case MBI_DISABLEDHOT:
				{
					my::drawAlphaRectangle_Hot(dc, &rc2);
					return S_OK;
				}
			case MBI_PUSHED:
				{
					my::drawAlphaRectangle_Selected(dc, &rc2);
					return S_OK;
				}
			case MBI_DISABLEDPUSHED:
				{
					my::drawAlphaRectangle_Selected(dc, &rc2);
					return S_OK;
				}
			}

			break;
		}
	case MENU_POPUPCHECK: // = 11,
		{
//			::InflateRect(&rc2, 2, 2);
			::OffsetRect(&rc2, 3, 0);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case MC_CHECKMARKNORMAL:
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case MC_CHECKMARKDISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x61'), format);
					return S_OK;
				}
			case MC_BULLETNORMAL:
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x3D'), format);
					return S_OK;
				}
			case MC_BULLETDISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x3D'), format);
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

			switch (stateId)
			{
			case MPI_NORMAL:
			case MPI_DISABLED:
				{
					return S_OK;
				}
			case MPI_HOT:
			case MPI_DISABLEDHOT:
				{
					my::drawAlphaRectangle_Hot(dc, &rc2);
					return S_OK;
				}
			}

			break;
		}
	case MENU_POPUPSEPARATOR: // = 15,
		{
			::InflateRect(&rc2, -2, -2);
			my::fillRect_Separator(dc, &rc2);
			return S_OK;
		}
	case MENU_POPUPSUBMENU: // = 16,
		{
			::InflateRect(&rc2, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case MSM_NORMAL:
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x7E'), format);
					return S_OK;
				}
			case MSM_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x7E'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_SYSTEMCLOSE: // = 17,
		{
			::InflateRect(&rc2, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case MSYSC_NORMAL:
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x72'), format);
					return S_OK;
				}
			case MSYSC_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x72'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_SYSTEMMAXIMIZE: // = 18,
		{
			::InflateRect(&rc2, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case MSYSMX_NORMAL:
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x31'), format);
					return S_OK;
				}
			case MSYSMX_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x31'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_SYSTEMMINIMIZE: // = 19,
		{
			::InflateRect(&rc2, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case MSYSMN_NORMAL:
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x30'), format);
					return S_OK;
				}
			case MSYSMN_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x30'), format);
					return S_OK;
				}
			}

			break;
		}
	case MENU_SYSTEMRESTORE: // = 20,
		{
			::InflateRect(&rc2, 2, 2);
			UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

			switch (stateId)
			{
			case MSYSR_NORMAL:
				{
					my::drawShadowIcon_Dialog(dc, &rc2, _T('\x32'), format);
					return S_OK;
				}
			case MSYSR_DISABLED:
				{
					my::drawShadowIcon_Dialog_Disabled(dc, &rc2, _T('\x32'), format);
					return S_OK;
				}
			}

			break;
		}
	}

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT MenuThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
	MY_TRACE(_T("MenuThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	RECT rc2 = *rc;

	switch (partId)
	{
	case MENU_BARITEM: // = 8,
		{
			switch (stateId)
			{
			case MBI_NORMAL:
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case MBI_DISABLED:
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case MBI_HOT:
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case MBI_DISABLEDHOT:
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case MBI_PUSHED:
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case MBI_DISABLEDPUSHED:
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			}

			break;
		}
	case MENU_POPUPITEM: // = 14,
		{
			switch (stateId)
			{
			case MPI_NORMAL:
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case MPI_DISABLED:
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case MPI_HOT:
				{
					my::drawShadowText_Dialog(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			case MPI_DISABLEDHOT:
				{
					my::drawShadowText_Dialog_Disabled(dc, text, c, &rc2, textFlags);
					return S_OK;
				}
			}

			break;
		}
	}

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
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
