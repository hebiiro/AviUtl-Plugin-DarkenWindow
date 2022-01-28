#include "pch.h"
#include "DarkenWindow.h"
#include "MyDraw.h"

//--------------------------------------------------------------------
namespace my {
//--------------------------------------------------------------------

COLORREF getBrushColor(HBRUSH brush)
{
	LOGBRUSH lb = {};
	if (::GetObject(brush, sizeof(lb), &lb) != sizeof(lb)) return CLR_NONE;
	if (lb.lbStyle != BS_SOLID) return CLR_NONE;
	return lb.lbColor;
}

void clipRect(HDC dc, LPCRECT rc)
{
	HRGN rgn = ::CreateRectRgnIndirect(rc);
	::SelectClipRgn(dc, rgn);
	::DeleteObject(rgn);
}

void clipRect(HDC dc, LPCRECT rc, int mode)
{
	HRGN rgn = ::CreateRectRgnIndirect(rc);
	::ExtSelectClipRgn(dc, rgn, mode);
	::DeleteObject(rgn);
}

void fillRect(HDC dc, LPCRECT rc, COLORREF fillColor)
{
	int w = rc->right - rc->left;
	int h = rc->bottom - rc->top;

	HBRUSH fillBrush = ::CreateSolidBrush(fillColor);
	HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, fillBrush);
	true_PatBlt(dc, rc->left, rc->top, w, h, PATCOPY);
	::SelectObject(dc, oldBrush);
	::DeleteObject(fillBrush);
}

void frameRect(HDC dc, LPCRECT rc, COLORREF edgeColor, int edgeWidth)
{
	int w = rc->right - rc->left;
	int h = rc->bottom - rc->top;

	HBRUSH edgeBrush = ::CreateSolidBrush(edgeColor);
	HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, edgeBrush);
	true_PatBlt(dc, rc->left, rc->top, w, edgeWidth, PATCOPY);
	true_PatBlt(dc, rc->left, rc->top, edgeWidth, h, PATCOPY);
	true_PatBlt(dc, rc->left, rc->bottom - edgeWidth, w, edgeWidth, PATCOPY);
	true_PatBlt(dc, rc->right - edgeWidth, rc->top, edgeWidth, h, PATCOPY);
	::SelectObject(dc, oldBrush);
	::DeleteObject(edgeBrush);
}

void drawRectangle(HDC dc, LPCRECT rc, COLORREF fillColor, COLORREF edgeColor, int edgeWidth)
{
	HBRUSH fillBrush = ::CreateSolidBrush(fillColor);
	HBRUSH edgeBrush = ::CreateSolidBrush(edgeColor);
	HBRUSH oldBrush = (HBRUSH)::GetCurrentObject(dc, OBJ_BRUSH);

	int w = rc->right - rc->left;
	int h = rc->bottom - rc->top;

	::SelectObject(dc, fillBrush);
	true_PatBlt(dc, rc->left, rc->top, w, h, PATCOPY);

	if (edgeWidth > 0)
	{
		::SelectObject(dc, edgeBrush);
		true_PatBlt(dc, rc->left, rc->top, w, edgeWidth, PATCOPY);
		true_PatBlt(dc, rc->left, rc->bottom - edgeWidth, w, edgeWidth, PATCOPY);
		true_PatBlt(dc, rc->left, rc->top, edgeWidth, h, PATCOPY);
		true_PatBlt(dc, rc->right - edgeWidth, rc->top, edgeWidth, h, PATCOPY);
	}

	::SelectObject(dc, oldBrush);
	::DeleteObject(fillBrush);
	::DeleteObject(edgeBrush);
}

void drawAlphaRectangle(HDC dc, LPCRECT rc, COLORREF fillColor, COLORREF edgeColor, int edgeWidth)
{
	int w = rc->right - rc->left;
	int h = rc->bottom - rc->top;
	RECT mrc = { 0, 0, w, h };
	HDC mdc = ::CreateCompatibleDC(dc);
	HBITMAP bitmap = ::CreateCompatibleBitmap(dc, w, h);
	HBITMAP oldBitmap = (HBITMAP)::SelectObject(mdc, bitmap);
	BLENDFUNCTION bf;
	bf.BlendOp = AC_SRC_OVER;
	bf.BlendFlags = 0;
	bf.SourceConstantAlpha = 128;
	bf.AlphaFormat = AC_SRC_ALPHA;
	drawRectangle(mdc, &mrc, fillColor, edgeColor, edgeWidth);
	::AlphaBlend(dc, rc->left, rc->top, w, h, mdc, 0, 0, w, h, bf);
	::SelectObject(mdc, oldBitmap);
	::DeleteObject(bitmap);
	::DeleteDC(mdc);
}

void drawText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF textForeColor, COLORREF textBackColor)
{
	RECT rc2 = *rc;
	int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
	int oldTextColor = ::SetTextColor(dc, textBackColor);
	::OffsetRect(&rc2, +1, +1);
	::DrawTextW(dc, text, length, &rc2, format);
	::OffsetRect(&rc2, -1, -1);
	::SetTextColor(dc, textForeColor);
	::DrawTextW(dc, text, length, &rc2, format);
	::SetBkMode(dc, oldBkMode);
	::SetTextColor(dc, oldTextColor);
}

void drawIcon(HDC dc, LPRECT rc, WCHAR text, COLORREF textForeColor, COLORREF textBackColor, int textWidth)
{
	int w = rc->right - rc->left;
	int h = rc->bottom - rc->top;
	POINT center =
	{
		::MulDiv(rc->left + rc->right, 1, 2),
		::MulDiv(rc->top + rc->bottom, 1, 2),
	};
	BOOL arrow = FALSE;
	POINT points[3] = {};

	switch (text)
	{
	case L'\x0035': // Up-Arrow
		{
			arrow = TRUE;
			center.y += ::MulDiv(h, 1, 8);
			int offset = ::MulDiv(h, 1, 4);
			points[0].x = center.x - offset;
			points[1].x = center.x;
			points[2].x = center.x + offset;
			points[0].y = center.y;
			points[1].y = center.y - offset;
			points[2].y = center.y;
			break;
		}
	case L'\x0036': // Down-Arrow
		{
			arrow = TRUE;
			center.y -= ::MulDiv(h, 1, 8);
			int offset = ::MulDiv(h, 1, 4);
			points[0].x = center.x - offset;
			points[1].x = center.x;
			points[2].x = center.x + offset;
			points[0].y = center.y;
			points[1].y = center.y + offset;
			points[2].y = center.y;
			break;
		}
	case L'\x0033': // Left-Arrow
		{
			arrow = TRUE;
			center.x += ::MulDiv(w, 1, 8);
			int offset = ::MulDiv(w, 1, 4);
			points[0].x = center.x;
			points[1].x = center.x - offset;
			points[2].x = center.x;
			points[0].y = center.y - offset;
			points[1].y = center.y;
			points[2].y = center.y + offset;
			break;
		}
	case L'\x0034': // Right-Arrow
		{
			arrow = TRUE;
			center.x -= ::MulDiv(w, 1, 8);
			int offset = ::MulDiv(w, 1, 4);
			points[0].x = center.x;
			points[1].x = center.x + offset;
			points[2].x = center.x;
			points[0].y = center.y - offset;
			points[1].y = center.y;
			points[2].y = center.y + offset;
			break;
		}
	}

	if (arrow)
	{
		HPEN pen = ::CreatePen(PS_SOLID, textWidth, textForeColor);
		HPEN oldPen = (HPEN)::SelectObject(dc, pen);
		BOOL result = ::Polyline(dc, points, 3);
		::SelectObject(dc, oldPen);
		::DeleteObject(pen);
	}
	else
	{
		HFONT font = ::CreateFontW(
			h, 0, 0, 0, 0, FALSE, FALSE, FALSE,
			DEFAULT_CHARSET, 0, 0, 0, 0, L"Webdings");
		HFONT oldFont = (HFONT)::SelectObject(dc, font);
		drawText(dc, &text, 1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE, textForeColor, textBackColor);
		::SelectObject(dc, oldFont);
		::DeleteObject(font);
	}
}

void drawSingleEdge(HDC dc, LPCRECT rc, COLORREF topLeftColor, COLORREF bottomRightColor)
{
	int edgeWidth = 1;

	int w = rc->right - rc->left;
	int h = rc->bottom - rc->top;

	HBRUSH topLeftBrush = ::CreateSolidBrush(topLeftColor);
	HBRUSH bottomRightBrush = ::CreateSolidBrush(bottomRightColor);
	HBRUSH oldBrush = (HBRUSH)::SelectObject(dc, topLeftBrush);
	true_PatBlt(dc, rc->left, rc->top, w, edgeWidth, PATCOPY);
	true_PatBlt(dc, rc->left, rc->top, edgeWidth, h, PATCOPY);
	::SelectObject(dc, bottomRightBrush);
	true_PatBlt(dc, rc->left, rc->bottom - edgeWidth, w, edgeWidth, PATCOPY);
	true_PatBlt(dc, rc->right - edgeWidth, rc->top, edgeWidth, h, PATCOPY);
	::SelectObject(dc, oldBrush);
	::DeleteObject(topLeftBrush);
	::DeleteObject(bottomRightBrush);
}

void drawDoubleEdge(HDC dc, LPCRECT rc, COLORREF innerTopLeftColor, COLORREF innerBottomRightColor, COLORREF outerTopLeftColor, COLORREF outerBottomRightColor)
{
	RECT rc2 = *rc;
	drawSingleEdge(dc, &rc2, outerTopLeftColor, outerBottomRightColor);
	::InflateRect(&rc2, -1, -1);
	drawSingleEdge(dc, &rc2, innerTopLeftColor, innerBottomRightColor);
}

void drawTripleEdge(HDC dc, LPCRECT rc, COLORREF innerTopLeftColor, COLORREF innerBottomRightColor, COLORREF middleTopLeftColor, COLORREF middleBottomRightColor, COLORREF outerTopLeftColor, COLORREF outerBottomRightColor)
{
	RECT rc2 = *rc;
	drawSingleEdge(dc, &rc2, outerTopLeftColor, outerBottomRightColor);
	::InflateRect(&rc2, -1, -1);
	drawSingleEdge(dc, &rc2, middleTopLeftColor, middleBottomRightColor);
	::InflateRect(&rc2, -1, -1);
	drawSingleEdge(dc, &rc2, innerTopLeftColor, innerBottomRightColor);
}

void drawFocusRect(HDC dc, LPCRECT rc, COLORREF lineColor, UINT lineStyle)
{
	RECT rc2 = *rc;
	rc2.right -= 1; rc2.bottom -= 1;
	LOGBRUSH LogBrush = {};
	LogBrush.lbStyle = BS_SOLID;
	LogBrush.lbColor = lineColor;
	LogBrush.lbHatch = 0;
	DWORD dwPenStyle[2] = { 0, 2 };
	HPEN pen = ::ExtCreatePen(PS_GEOMETRIC | PS_USERSTYLE, 1, &LogBrush, 2, dwPenStyle);
	HPEN oldPen = (HPEN)::SelectObject(dc, pen);
	::MoveToEx(dc, rc2.left, rc2.top, NULL);
	::LineTo(dc, rc2.right, rc2.top);
	::LineTo(dc, rc2.right, rc2.bottom);
	::LineTo(dc, rc2.left, rc2.bottom);
	::LineTo(dc, rc2.left, rc2.top);
	::SelectObject(dc, oldPen);
	::DeleteObject(pen);
}

void drawText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF textColor)
{
	RECT rc2 = *rc;
	int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
	int oldTextColor = ::SetTextColor(dc, textColor);
	::DrawTextW(dc, text, length, &rc2, format);
	::SetBkMode(dc, oldBkMode);
	::SetTextColor(dc, oldTextColor);
}

void drawShadowText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF textForeColor, COLORREF textBackColor)
{
	RECT rc2 = *rc;
	int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
	int oldTextColor = ::SetTextColor(dc, textBackColor);
	::OffsetRect(&rc2, +1, +1);
	::DrawTextW(dc, text, length, &rc2, format);
	::OffsetRect(&rc2, -1, -1);
	::SetTextColor(dc, textForeColor);
	::DrawTextW(dc, text, length, &rc2, format);
	::SetBkMode(dc, oldBkMode);
	::SetTextColor(dc, oldTextColor);
}

void drawShadowText2(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF textForeColor, COLORREF textBackColor)
{
	RECT rc2 = *rc;
	int oldTextColor = ::SetTextColor(dc, textBackColor);
	::OffsetRect(&rc2, +1, +1);
	::DrawTextW(dc, text, length, &rc2, format);
	::OffsetRect(&rc2, -1, -1);
	int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
	::SetTextColor(dc, textForeColor);
	::DrawTextW(dc, text, length, &rc2, format);
	::SetBkMode(dc, oldBkMode);
	::SetTextColor(dc, oldTextColor);
}

void shadowTextOut(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx, COLORREF textForeColor, COLORREF textBackColor)
{
	RECT rc2 = {};
	if (rc) rc2 = *rc;
	true_ExtTextOutW(dc, x, y, options, &rc2, L"", 0, dx);
	int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
	int oldTextColor = ::SetTextColor(dc, textBackColor);
	x += 1, y += 1; ::OffsetRect(&rc2, +1, +1);
	true_ExtTextOutW(dc, x, y, options & ~ETO_OPAQUE, &rc2, text, c, dx);
	::SetTextColor(dc, textForeColor);
	x -= 1, y -= 1; ::OffsetRect(&rc2, -1, -1);
	true_ExtTextOutW(dc, x, y, options & ~ETO_OPAQUE, &rc2, text, c, dx);
	::SetBkMode(dc, oldBkMode);
	::SetTextColor(dc, oldTextColor);
}

struct DrawIconMetrics
{
	DrawIconMetrics(LPCRECT rc, WCHAR icon)
	{
		w = rc->right - rc->left;
		h = rc->bottom - rc->top;
		center.x = ::MulDiv(rc->left + rc->right, 1, 2);
		center.y = ::MulDiv(rc->top + rc->bottom, 1, 2);
		arrow = FALSE;
#if 0
		switch (icon)
		{
		case L'\x0035': // Up-Arrow
			{
				arrow = TRUE;
				center.y += ::MulDiv(h, 1, 8);
				int offset = ::MulDiv(h, 1, 4);
				points[0].x = center.x - offset;
				points[1].x = center.x;
				points[2].x = center.x + offset;
				points[0].y = center.y;
				points[1].y = center.y - offset;
				points[2].y = center.y;
				break;
			}
		case L'\x0036': // Down-Arrow
			{
				arrow = TRUE;
				center.y -= ::MulDiv(h, 1, 8);
				int offset = ::MulDiv(h, 1, 4);
				points[0].x = center.x - offset;
				points[1].x = center.x;
				points[2].x = center.x + offset;
				points[0].y = center.y;
				points[1].y = center.y + offset;
				points[2].y = center.y;
				break;
			}
		case L'\x0033': // Left-Arrow
			{
				arrow = TRUE;
				center.x += ::MulDiv(w, 1, 8);
				int offset = ::MulDiv(w, 1, 4);
				points[0].x = center.x;
				points[1].x = center.x - offset;
				points[2].x = center.x;
				points[0].y = center.y - offset;
				points[1].y = center.y;
				points[2].y = center.y + offset;
				break;
			}
		case L'\x0034': // Right-Arrow
			{
				arrow = TRUE;
				center.x -= ::MulDiv(w, 1, 8);
				int offset = ::MulDiv(w, 1, 4);
				points[0].x = center.x;
				points[1].x = center.x + offset;
				points[2].x = center.x;
				points[0].y = center.y - offset;
				points[1].y = center.y;
				points[2].y = center.y + offset;
				break;
			}
		}
#endif
	}

	int w = 0;
	int h = 0;
	POINT center = {};
	BOOL arrow = FALSE;
	POINT points[3] = {};
};

void drawIcon(HDC dc, LPCRECT rc, WCHAR icon, COLORREF iconColor, int iconWidth)
{
	DrawIconMetrics di(rc, icon);

	if (di.arrow)
	{
		HPEN pen = ::CreatePen(PS_SOLID, iconWidth, iconColor);
		HPEN oldPen = (HPEN)::SelectObject(dc, pen);
		BOOL result = ::Polyline(dc, di.points, 3);
		::SelectObject(dc, oldPen);
		::DeleteObject(pen);
	}
	else
	{
		int size = yulib::Min(di.w, di.h);
		HFONT font = ::CreateFontW(-size, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Webdings");
		HFONT oldFont = (HFONT)::SelectObject(dc, font);
		drawText(dc, &icon, 1, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE, iconColor);
		::SelectObject(dc, oldFont);
		::DeleteObject(font);
	}
}

void drawShadowIcon(HDC dc, LPCRECT rc, WCHAR icon, UINT format, COLORREF iconForeColor, COLORREF iconBackColor, int iconWidth)
{
	DrawIconMetrics di(rc, icon);

	if (di.arrow)
	{
		HPEN forePen = ::CreatePen(PS_SOLID, iconWidth, iconForeColor);
		HPEN backPen = ::CreatePen(PS_SOLID, iconWidth, iconBackColor);
		HPEN oldPen = (HPEN)::SelectObject(dc, backPen);
		for (int i = 0; i < 3; i++){ di.points[i].x += 1; di.points[i].y += 1; }
		::Polyline(dc, di.points, 3);
		::SelectObject(dc, forePen);
		for (int i = 0; i < 3; i++){ di.points[i].x -= 1; di.points[i].y -= 1; }
		::Polyline(dc, di.points, 3);
		::SelectObject(dc, oldPen);
		::DeleteObject(forePen);
		::DeleteObject(backPen);
	}
	else
	{
		RECT rc2 = *rc;
		int size = yulib::Min(di.w, di.h);
		HFONT font = ::CreateFontW(-size, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, L"Webdings");
		HFONT oldFont = (HFONT)::SelectObject(dc, font);
		::OffsetRect(&rc2, +1, +1);
		drawText(dc, &icon, 1, &rc2, format, iconBackColor);
		::OffsetRect(&rc2, -1, -1);
		drawText(dc, &icon, 1, &rc2, format, iconForeColor);
		::SelectObject(dc, oldFont);
		::DeleteObject(font);
	}
}

//--------------------------------------------------------------------

COLORREF getFillColor_Dialog()
{
	return RGB(0x33, 0x33, 0x33);
}

COLORREF getFillColor_Dialog_Selected()
{
	return RGB(0xff, 0x00, 0x00);
}

COLORREF getForeTextColor_Dialog()
{
	return RGB(0xff, 0xff, 0xff);
}

COLORREF getForeTextColor_Dialog_Disabled()
{
	return RGB(0x99, 0x99, 0x99);
}

COLORREF getForeTextColor_Dialog_Selected()
{
	return RGB(0xff, 0x33, 0x33);
}

COLORREF getForeTextColor_Dialog_Hot()
{
	return RGB(0x33, 0xff, 0x33);
}

COLORREF getForeTextColor_Dialog_Focus()
{
	return RGB(0x33, 0x33, 0xff);
}

COLORREF getBackTextColor_Dialog()
{
	return RGB(0x00, 0x00, 0x00);
}

COLORREF getFillColor_Window()
{
	return RGB(0x00, 0x00, 0x00);
}

COLORREF getFillColor_Window_Selected()
{
	return RGB(0x99, 0x00, 0x00);
}

COLORREF getForeTextColor_Window()
{
	return RGB(0xff, 0xff, 0xff);
}

COLORREF getBackTextColor_Window()
{
	return RGB(0x33, 0x33, 0x33);
}

void fillRect_Dialog(HDC dc, LPCRECT rc)
{
	fillRect(dc, rc, getFillColor_Dialog());
}

void fillRect_Dialog_Selected(HDC dc, LPCRECT rc)
{
	fillRect(dc, rc, getFillColor_Dialog_Selected());
}

void fillRect_Window(HDC dc, LPCRECT rc)
{
	fillRect(dc, rc, getFillColor_Window());
}

void fillRect_Window_Selected(HDC dc, LPCRECT rc)
{
	fillRect(dc, rc, getFillColor_Window_Selected());
}

void fillRect_Gutter(HDC dc, LPCRECT rc)
{
	fillRect(dc, rc, RGB(0x40, 0x40, 0x40));
}

void fillRect_Separator(HDC dc, LPCRECT rc)
{
	fillRect(dc, rc, RGB(0x66, 0x66, 0x66));
}

void fillRect_TrackBar_Thumb(HDC dc, LPCRECT rc)
{
	fillRect(dc, rc, RGB(0x99, 0x00, 0x00));
}

void fillRect_TrackBar_Thumb_Pushed(HDC dc, LPCRECT rc)
{
	fillRect_Dialog(dc, rc);
}

void fillRect_TrackBar_Channel(HDC dc, LPCRECT rc)
{
	fillRect_Gutter(dc, rc);
}

void frameRect_Menu(HDC dc, LPCRECT rc)
{
	frameRect(dc, rc, RGB(0x00, 0x00, 0x00), 1);
}

void frameRect_Button(HDC dc, LPCRECT rc)
{
	frameRect(dc, rc, RGB(0x00, 0x00, 0x00), 1);
}

void drawSingleEdge_Raised(HDC dc, LPCRECT rc)
{
	drawSingleEdge(dc, rc,
		RGB(0x55, 0x55, 0x55),
		RGB(0x11, 0x11, 0x11));
}

void drawSingleEdge_Sunken(HDC dc, LPCRECT rc)
{
	drawSingleEdge(dc, rc,
//		RGB(0x22, 0x22, 0x22),
//		RGB(0x44, 0x44, 0x44));
		RGB(0x11, 0x11, 0x11),
		RGB(0x55, 0x55, 0x55));
}

void drawSingleEdge_Etched(HDC dc, LPCRECT rc)
{
	drawSingleEdge(dc, rc,
		RGB(0x00, 0x00, 0x00),
		RGB(0x55, 0x55, 0x55));
}

void drawDoubleEdge_Raised(HDC dc, LPCRECT rc)
{
	drawDoubleEdge(dc, rc,
		RGB(0x55, 0x55, 0x55),
		RGB(0x11, 0x11, 0x11),
		RGB(0x44, 0x44, 0x44),
		RGB(0x22, 0x22, 0x22));
}

void drawDoubleEdge_Sunken(HDC dc, LPCRECT rc)
{
	drawDoubleEdge(dc, rc,
		RGB(0x11, 0x11, 0x11),
		RGB(0x55, 0x55, 0x55),
		RGB(0x22, 0x22, 0x22),
		RGB(0x44, 0x44, 0x44));
}

void drawDoubleEdge_Bump(HDC dc, LPCRECT rc)
{
	drawDoubleEdge(dc, rc,
		RGB(0x44, 0x44, 0x44),
		RGB(0x22, 0x22, 0x22),
		RGB(0x44, 0x44, 0x44),
		RGB(0x22, 0x22, 0x22));
}

void drawDoubleEdge_Etched(HDC dc, LPCRECT rc)
{
	drawDoubleEdge(dc, rc,
		RGB(0x55, 0x55, 0x55),
		RGB(0x00, 0x00, 0x00),
		RGB(0x00, 0x00, 0x00),
		RGB(0x55, 0x55, 0x55));
}

void drawTripleEdge_Etched(HDC dc, LPCRECT rc)
{
	drawTripleEdge(dc, rc,
		RGB(0x55, 0x55, 0x55),
		RGB(0x00, 0x00, 0x00),
		RGB(0x00, 0x00, 0x00),
		RGB(0x00, 0x00, 0x00),
		RGB(0x00, 0x00, 0x00),
		RGB(0x55, 0x55, 0x55));
}

void drawFocusRect_Button(HDC dc, LPCRECT rc)
{
	drawFocusRect(dc, rc, RGB(0x00, 0x99, 0x00), PS_DOT);
}

void drawShadowText_Dialog(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format)
{
	drawShadowText(dc, text, length, rc, format, getForeTextColor_Dialog(), getBackTextColor_Dialog());
}

void drawShadowText_Dialog_Disabled(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format)
{
	drawShadowText(dc, text, length, rc, format, getForeTextColor_Dialog_Disabled(), getBackTextColor_Dialog());
}

void drawShadowText_Dialog_Selected(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format)
{
	drawShadowText(dc, text, length, rc, format, getForeTextColor_Dialog_Selected(), getBackTextColor_Dialog());
}

void drawShadowText_Dialog_Hot(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format)
{
	drawShadowText(dc, text, length, rc, format, getForeTextColor_Dialog_Hot(), getBackTextColor_Dialog());
}

void drawShadowText_Dialog_Focus(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format)
{
	drawShadowText(dc, text, length, rc, format, getForeTextColor_Dialog_Focus(), getBackTextColor_Dialog());
}

void drawShadowText_Window(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format)
{
	drawShadowText(dc, text, length, rc, format, getForeTextColor_Window(), getBackTextColor_Window());
}

void shadowTextOut_Dialog(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	shadowTextOut(dc, x, y, options, rc, text, c, dx, getForeTextColor_Dialog(), getBackTextColor_Dialog());
}

void shadowTextOut_Window(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	shadowTextOut(dc, x, y, options, rc, text, c, dx, getForeTextColor_Window(), getBackTextColor_Window());
}

void drawShadowIcon_Dialog(HDC dc, LPCRECT rc, WCHAR icon, UINT format)
{
	drawShadowIcon(dc, rc, icon, format, getForeTextColor_Dialog(), getBackTextColor_Dialog(), 1);
}

void drawShadowIcon_Dialog_Disabled(HDC dc, LPCRECT rc, WCHAR icon, UINT format)
{
	drawShadowIcon(dc, rc, icon, format, RGB(0x99, 0x99, 0x99), getBackTextColor_Dialog(), 1);
}

void drawShadowIcon_Dialog_Selected(HDC dc, LPCRECT rc, WCHAR icon, UINT format)
{
	drawShadowIcon(dc, rc, icon, format, RGB(0xff, 0x33, 0x33), getBackTextColor_Dialog(), 1);
}

void drawShadowIcon_Dialog_Hot(HDC dc, LPCRECT rc, WCHAR icon, UINT format)
{
	drawShadowIcon(dc, rc, icon, format, RGB(0x33, 0xff, 0x33), getBackTextColor_Dialog(), 1);
}

void drawShadowIcon_Dialog_Focus(HDC dc, LPCRECT rc, WCHAR icon, UINT format)
{
	drawShadowIcon(dc, rc, icon, format, RGB(0x99, 0x99, 0xff), getBackTextColor_Dialog(), 1);
}

void drawAlphaRectangle_Selected(HDC dc, LPCRECT rc)
{
	drawAlphaRectangle(dc, rc, RGB(0x00, 0x99, 0x00), RGB(0x00, 0x99, 0x00), 2);
}

void drawAlphaRectangle_Hot(HDC dc, LPCRECT rc)
{
	drawAlphaRectangle(dc, rc, RGB(0x99, 0x00, 0x00), RGB(0x99, 0x00, 0x00), 2);
}

void drawStaticEdge(HDC dc, LPCRECT rc)
{
	frameRect(dc, rc, RGB(0x00, 0x00, 0x00), 1);
}

void drawClientEdge(HDC dc, LPCRECT rc)
{
	frameRect(dc, rc, RGB(0x00, 0x00, 0x00), 2);
}

void drawWindowEdge(HDC dc, LPCRECT rc)
{
	frameRect(dc, rc, RGB(0x00, 0x00, 0x00), 2);
}

//--------------------------------------------------------------------

LRESULT WINAPI onNcPaint(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_NCPAINT:
		{
			true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);

			HDC dc = ::GetWindowDC(hwnd);
			RECT rc; ::GetWindowRect(hwnd, &rc);
			::OffsetRect(&rc, -rc.left, -rc.top);

			DWORD exStyle = GetWindowExStyle(hwnd);

			if (exStyle & WS_EX_WINDOWEDGE)
			{
//				my::drawWindowEdge(dc, &rc);
				my::drawDoubleEdge_Raised(dc, &rc);
				::InflateRect(&rc, -2, -2);
			}

			if (exStyle & WS_EX_STATICEDGE)
			{
//				my::drawStaticEdge(dc, &rc);
				my::drawSingleEdge_Sunken(dc, &rc);
				::InflateRect(&rc, -1, -1);
			}

			if (exStyle & WS_EX_CLIENTEDGE)
			{
//				my::drawClientEdge(dc, &rc);
				my::drawDoubleEdge_Sunken(dc, &rc);
				::InflateRect(&rc, -2, -2);
			}

			::ReleaseDC(hwnd, dc);

			return 0;
		}
	}

	return 1;
}

//--------------------------------------------------------------------
} // namespace my
//--------------------------------------------------------------------
