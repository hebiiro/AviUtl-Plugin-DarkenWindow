#include "pch.h"
#include "resource.h"
#include "DarkenWindow.h"
//#include "ThemeHook.h"
//#include "ClassicHook.h"
#include "MyDraw.h"
#include "Skin.h"

//---------------------------------------------------------------------
namespace ExEdit {
//---------------------------------------------------------------------

HFONT* g_font = 0;
int g_drawingLayerIndex = 0;
int (*CallShowColorDialog)(DWORD u1, COLORREF* color, DWORD u3) = 0;

COLORREF getColorFromLayerName(LPCSTR name)
{
	if (!name) return CLR_NONE;

	LPCSTR sep = strchr(name, '#');
	if (!sep) return CLR_NONE;

	COLORREF color = strtoul(sep + 1, 0, 16);
	return RGB(GetBValue(color), GetGValue(color), GetRValue(color));
}

COLORREF getColorFromLayerIndex(int index)
{
	for (int i = index; i >= 0; i--)
	{
		ExEdit::LayerSetting* layer = g_auin.GetLayerSetting(i);
		LPCSTR name = layer->name;
		if (!name) continue; // レイヤー名が無効だった。
		LPCSTR sep = strchr(name, '#');
		if (!sep) continue; // セパレータがなかった。
		if (!sep[1]) return CLR_NONE; // セパレータ以降が空文字列ならデフォルトカラーを使用する。
		COLORREF color = strtoul(sep + 1, 0, 16);
		return RGB(GetBValue(color), GetGValue(color), GetRValue(color));
	}

	return CLR_NONE;
}

COLORREF getLayerBackgroundColor()
{
	ExEdit::LayerSetting* layer = g_auin.GetLayerSetting(g_drawingLayerIndex);

	// ステート ID を取得する。
	int stateId = 0;
	if (!(layer->flag & ExEdit::LayerSetting::Flag::UnDisp))
	{
		if (g_skin.getUseLayerColor())
		{
			// ユーザーカラーを取得する。
			COLORREF userColor = CLR_NONE;
			if (g_skin.getUseLayerColorEx())
				userColor = getColorFromLayerIndex(g_drawingLayerIndex);
			else
				userColor = getColorFromLayerName(layer->name);

			// ユーザーカラーが有効なら使用する。
			if (userColor != CLR_NONE)
				return userColor;
		}

		stateId = Dark::EXEDIT_LAYER_ACTIVE;
	}
	else
	{
		stateId = Dark::EXEDIT_LAYER_INACTIVE;
	}

	// テーマカラーがあればそれを使用する。
	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERBACKGROUND, stateId);
	if (state && state->m_fillColor != CLR_NONE)
		return state->m_fillColor;

	return CLR_NONE;
}

int getBlendCount(HDC dc)
{
	COLORREF color = ::GetDCBrushColor(dc);
//	MY_TRACE_HEX(color);
	switch (color)
	{
	case 0x00DEDEDE: return 1;
	case 0x00CCCCCC: return 2;
	case 0x00BABABA: return 3;

	case 0x00BEBEBE: return 1;
	case 0x00B1B1B1: return 2;
	case 0x00A3A3A3: return 3;
	}

	return 0;
}

COLORREF blend(COLORREF color1, COLORREF color2, float alpha)
{
	float omega = 1.0f - alpha;

	BYTE r = (BYTE)(GetRValue(color1) * omega + GetRValue(color2) * alpha);
	BYTE g = (BYTE)(GetGValue(color1) * omega + GetGValue(color2) * alpha);
	BYTE b = (BYTE)(GetBValue(color1) * omega + GetBValue(color2) * alpha);

	return RGB(r, g, b);
}

void initExEdit()
{
	MY_TRACE(_T("initExEdit()\n"));

	using namespace ExEdit;

	MY_TRACE(_T("拡張編集をフックします\n"));

	g_auin.initExEditAddress();
	uintptr_t exedit = g_auin.GetExEdit();

	ExEdit::g_font = (HFONT*)(exedit + 0x00167D84);

	hookAbsoluteCall(exedit + 0x0003833E, ExEdit::drawRootText);
	hookAbsoluteCall(exedit + 0x0003836A, ExEdit::drawRootEdge);

	hookAbsoluteCall(exedit + 0x00037CFF, ExEdit::drawLayerText);
	hookAbsoluteCall(exedit + 0x00037D46, ExEdit::drawLayerEdge);

	hookCall(exedit + 0x000380DF, ExEdit::drawTimelineLongScale);
	hookCall(exedit + 0x000381D7, ExEdit::drawTimelineShortScale);
	hookCall(exedit + 0x000381A2, ExEdit::drawTimelineTime);

	hookAbsoluteCall(exedit + 0x00038538, ExEdit::fillLayerBackground);
	hookAbsoluteCall(exedit + 0x0003860E, ExEdit::fillLayerBackground);
	hookAbsoluteCall(exedit + 0x000386E4, ExEdit::fillGroupBackground);

	hookCall(exedit + 0x00038845, ExEdit::drawLayerLeft);
	hookCall(exedit + 0x000388AA, ExEdit::drawLayerRight);
	hookCall(exedit + 0x00038871, ExEdit::drawLayerTop);
	hookCall(exedit + 0x000388DA, ExEdit::drawLayerBottom);
	hookCall(exedit + 0x00037A1F, ExEdit::drawLayerSeparator);

	castAddress(ExEdit::CallShowColorDialog, exedit + 0x0004D2A0);
	true_ShowLayerNameDialog = hookCall(exedit + 0x426D2, hook_ShowLayerNameDialog);

	DetourTransactionBegin();
	DetourUpdateThread(::GetCurrentThread());

	castAddress(true_drawLayerBackground, exedit + 0x00038410);
	ATTACH_HOOK_PROC(drawLayerBackground);

	if (DetourTransactionCommit() == NO_ERROR)
	{
		MY_TRACE(_T("API フックに成功しました\n"));
	}
	else
	{
		MY_TRACE(_T("API フックに失敗しました\n"));
	}
}

BOOL WINAPI drawRootText(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("drawRootText(0x%08X, %d, %d, 0x%08X)\n"), dc, x, y, options);

	WCHAR text2[MAX_PATH] = {};
	UINT c2 = ::MultiByteToWideChar(CP_ACP, 0, text, c, text2, MAX_PATH);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	if (g_skin.onExtTextOut(theme, dc, Dark::EXEDIT_ROOT, 0, x, y, options, rc, text2, c2, dx))
		return TRUE;

	return TRUE;
}

BOOL WINAPI drawRootEdge(HDC dc, LPRECT rc, UINT edge, UINT flags)
{
//	MY_TRACE(_T("drawRootEdge(0x%08X, 0x%08X, 0x%08X)\n"), dc, edge, flags);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	if (g_skin.onDrawThemeBackground(theme, dc, Dark::EXEDIT_ROOT, 0, rc))
		return TRUE;

	return TRUE;
}

// レイヤーボタンのテキストを描画。背景色で塗りつぶしも行う。
BOOL WINAPI drawLayerText(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx)
{
	MY_TRACE(_T("drawLayerText(0x%08X, %d, %d, 0x%08X)\n"), dc, x, y, options);

	WCHAR text2[MAX_PATH] = {};
	UINT c2 = ::MultiByteToWideChar(CP_ACP, 0, text, c, text2, MAX_PATH);

	int stateId = Dark::EXEDIT_LAYER_ACTIVE;

	COLORREF bkColor = ::GetBkColor(dc);
//	MY_TRACE_HEX(bkColor);
	switch (bkColor)
	{
	case 0x00F0F0F0: stateId = Dark::EXEDIT_LAYER_ACTIVE; break;
	case 0x00CCCCCC: stateId = Dark::EXEDIT_LAYER_INACTIVE; break;
	}

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	g_skin.onDrawThemeBackground(theme, dc, Dark::EXEDIT_LAYER, stateId, rc);

	{
		LPCWSTR sep = wcschr(text2, L'#');
		if (sep)
			c2 = (UINT)(sep - text2);
	}

	if (g_skin.drawText(dc, theme, Dark::EXEDIT_LAYER, stateId, (LPRECT)rc, text2, c2, DT_CENTER | DT_VCENTER | DT_SINGLELINE))
		return TRUE;
//	if (g_skin.onExtTextOut(theme, dc, Dark::EXEDIT_LAYER, stateId, x, y, options, rc, text2, c2, dx))
//		return TRUE;

	return ::ExtTextOutA(dc, x, y, options, rc, text, c, dx);
}

BOOL WINAPI drawLayerEdge(HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("drawLayerEdge(0x%08X, 0x%08X, 0x%08X)\n"), dc, edge, flags);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	if (g_skin.onDrawThemeBackground(theme, dc, Dark::EXEDIT_LAYEREDGE, 0, rc))
		return TRUE;

	return ::DrawEdge(dc, rc, edge, flags);
}

void drawTimelineLongScale(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawTimelineLongScale(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LONGSCALE, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		RECT rc = { mx, my, mx + 1, ly };
		my::fillRect(dc, &rc, state->m_fillColor);
	}
}

void drawTimelineShortScale(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawTimelineShortScale(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_SHORTSCALE, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		RECT rc = { mx, my, mx + 1, ly };
		my::fillRect(dc, &rc, state->m_fillColor);
	}
}

void drawTimelineTime(HDC dc, LPCSTR text, int x, int y, int w, int h, int scroll_x)
{
//	MY_TRACE(_T("drawTimelineTime(0x%08X, %hs, %d, %d, %d, %d, %d)\n"), dc, text, x, y, w, h, scroll_x);

	::SelectObject(dc, *g_font);

	TEXTMETRIC tm = {};
	::GetTextMetrics(dc, &tm);

	RECT rc = { x, y, x + w, y + h };
	::OffsetRect(&rc, tm.tmHeight / 4 + scroll_x, 0);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);

	g_skin.onDrawThemeText(theme, dc, Dark::EXEDIT_LONGSCALE, 0,
		(_bstr_t)text, -1, DT_LEFT | DT_VCENTER | DT_SINGLELINE, &rc);
}

int WINAPI fillLayerBackground(HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("fillLayerBackground(0x%08X, 0x%08X)\n"), dc, brush);

	// レイヤーの背景色を取得する。
	COLORREF color = getLayerBackgroundColor();
	if (color != CLR_NONE)
	{
		// カラーを適用する。
		::SetDCBrushColor(dc, color);
	}

	return true_FillRect(dc, rc, (HBRUSH)::GetStockObject(DC_BRUSH));
}

// グループ制御などの描画。
int WINAPI fillGroupBackground(HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("fillGroupBackground(0x%08X, 0x%08X)\n"), dc, brush);

	// レイヤーの背景色を取得する。
	COLORREF color = getLayerBackgroundColor();
	if (color != CLR_NONE)
	{
		// ブレンド回数を取得する。
		int blendCount = getBlendCount(dc);
		for (int i = 0; i < blendCount; i++)
		{
			// ブレンド回数分だけグループカラーとブレンドする。
			color = blend(color, Dark::ExEditGroup::m_color, Dark::ExEditGroup::m_alpha);
		}
		// ブレンドしたカラーを適用する。
		::SetDCBrushColor(dc, color);
	}
	return true_FillRect(dc, rc, brush);
}

void drawLayerLeft(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerLeft(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERLEFT, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, 1, ly - my, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

void drawLayerRight(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerRight(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERRIGHT, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, 1, ly - my, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

void drawLayerTop(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerTop(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERTOP, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, lx - mx, 1, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

void drawLayerBottom(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerBottom(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERBOTTOM, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, lx - mx, 1, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

void drawLayerSeparator(HDC dc, int mx, int my, int lx, int ly, HPEN pen)
{
//	MY_TRACE(_T("drawLayerSeparator(0x%08X, %d, %d, %d, %d, 0x%08X)\n"), dc, mx, my, lx, ly, pen);

	if (pen) ::SelectObject(dc, pen);

	HTHEME theme = g_skin.getTheme(Dark::THEME_EXEDIT);
	Dark::StatePtr state = g_skin.getState(theme, Dark::EXEDIT_LAYERSEPARATOR, 0);
	if (state && state->m_fillColor != CLR_NONE)
	{
		HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, state->getFillBrush());
		::PatBlt(dc, mx, my, 1, ly - my, PATCOPY);
		::SelectObject(dc, oldBrush);
	}
}

IMPLEMENT_HOOK_PROC_NULL(void, CDECL, drawLayerBackground, (HDC dc, int layerIndex, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7))
{
//	MY_TRACE(_T("drawLayerBackground(0x%08X, %d, %d, %d, %d, %d, %d)\n"), dc, layerIndex, a3, a4, a5, a6, a7);

	// 現在描画しているレイヤーのインデックスを取得する。
	g_drawingLayerIndex = layerIndex;

	true_drawLayerBackground(dc, layerIndex, a3, a4, a5, a6, a7);
}

IMPLEMENT_HOOK_PROC_NULL(INT_PTR, CALLBACK, LayerNameDialogProc, (HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam))
{
	switch (message)
	{
	case WM_INITDIALOG:
		{
			MY_TRACE(_T("WM_INITDIALOG\n"));

			break;
		}
	case WM_COMMAND:
		{
			UINT code = HIWORD(wParam);
			UINT id = LOWORD(wParam);
			HWND sender = (HWND)lParam;

			MY_TRACE(_T("WM_COMMAND, 0x%04X, 0x%04X, 0x%08X)\n"), code, id, sender);

			if (id == IDC_CHOOSE_COLOR)
			{
				char text[MAX_PATH] = {};
				::GetDlgItemTextA(hdlg, 171, text, MAX_PATH);
				MY_TRACE_STR(text);

				COLORREF color = getColorFromLayerName(text);
				MY_TRACE_HEX(color);

				if (IDOK != CallShowColorDialog(0, &color, 2))
					break;

				LPSTR sep = strchr(text, '#');
				if (sep) *sep = '\0';

				char newText[MAX_PATH] = {};
				::StringCbPrintfA(newText, sizeof(newText), "%s#%02x%02x%02x",
					text, GetRValue(color), GetGValue(color), GetBValue(color));
				::SetDlgItemTextA(hdlg, 171, newText);
			}

			break;
		}
	}

	return true_LayerNameDialogProc(hdlg, message, wParam, lParam);
}

IMPLEMENT_HOOK_PROC_NULL(INT_PTR, CDECL, ShowLayerNameDialog, (HINSTANCE instance, LPCSTR templateName, HWND parent, DLGPROC dialogProc))
{
	MY_TRACE(_T("ShowLayerNameDialog(0x%08X, %hs, 0x%08X, 0x%08X)\n"), instance, templateName, parent, dialogProc);

	if (::lstrcmpiA(templateName, "GET_LAYER_NAME") == 0)
	{
		MY_TRACE(_T("「レイヤー名」ダイアログをフックします\n"));

		true_LayerNameDialogProc = dialogProc;
		return true_ShowLayerNameDialog(g_instance, templateName, parent, hook_LayerNameDialogProc);
	}

	return true_ShowLayerNameDialog(instance, templateName, parent, dialogProc);
}

//--------------------------------------------------------------------
} // namespace ExEdit
//---------------------------------------------------------------------
