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

void drawText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF fillColor, COLORREF textColor)
{
	int oldBkMode = ::GetBkMode(dc);
	COLORREF oldBkColor = ::GetBkColor(dc);
	COLORREF oldTextColor = ::GetTextColor(dc);

	RECT rc2 = {};
	if (rc) rc2 = *rc;

	if (fillColor != CLR_NONE)
		::SetBkColor(dc, fillColor);
	else
		::SetBkMode(dc, TRANSPARENT);

	::SetTextColor(dc, textColor);
	::DrawTextW(dc, text, length, &rc2, format);

	::SetBkMode(dc, oldBkMode);
	::SetBkColor(dc, oldBkColor);
	::SetTextColor(dc, oldTextColor);
}

void textOut(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx, COLORREF fillColor, COLORREF textColor)
{
	int oldBkMode = ::GetBkMode(dc);
	COLORREF oldBkColor = ::GetBkColor(dc);
	COLORREF oldTextColor = ::GetTextColor(dc);

	RECT rc2 = {};
	if (rc) rc2 = *rc;

	if (fillColor != CLR_NONE)
		::SetBkColor(dc, fillColor);
	else
		::SetBkMode(dc, TRANSPARENT);

	::SetTextColor(dc, textColor);
	true_ExtTextOutW(dc, x, y, options, &rc2, text, c, dx);

	::SetBkMode(dc, oldBkMode);
	::SetBkColor(dc, oldBkColor);
	::SetTextColor(dc, oldTextColor);
}

void drawShadowText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF fillColor, COLORREF textForeColor, COLORREF textBackColor)
{
	int oldBkMode = ::GetBkMode(dc);
	COLORREF oldBkColor = ::GetBkColor(dc);
	COLORREF oldTextColor = ::GetTextColor(dc);

	RECT rc2 = {};
	if (rc) rc2 = *rc;

	if (fillColor != CLR_NONE)
		::SetBkColor(dc, fillColor);
	else
		::SetBkMode(dc, TRANSPARENT);

	if (textBackColor != CLR_NONE)
	{
		::OffsetRect(&rc2, +1, +1);
		::SetTextColor(dc, textBackColor);
		::DrawTextW(dc, text, length, &rc2, format);
		::OffsetRect(&rc2, -1, -1);
		::SetBkMode(dc, TRANSPARENT);
	}

	::SetTextColor(dc, textForeColor);
	::DrawTextW(dc, text, length, &rc2, format);

	::SetBkMode(dc, oldBkMode);
	::SetBkColor(dc, oldBkColor);
	::SetTextColor(dc, oldTextColor);
}

void shadowTextOut(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx, COLORREF fillColor, COLORREF textForeColor, COLORREF textBackColor)
{
	int oldBkMode = ::GetBkMode(dc);
	COLORREF oldBkColor = ::GetBkColor(dc);
	COLORREF oldTextColor = ::GetTextColor(dc);

	RECT rc2 = {};
	if (rc) rc2 = *rc;

	if (fillColor != CLR_NONE)
	{
		// 背景を塗りつぶす。
		::SetBkColor(dc, fillColor);
		true_ExtTextOutW(dc, x, y, options, &rc2, L"", 0, dx);
	}

	::SetBkMode(dc, TRANSPARENT);

	if (textBackColor != CLR_NONE)
	{
		x += 1, y += 1; ::OffsetRect(&rc2, +1, +1);
		::SetTextColor(dc, textBackColor);
		true_ExtTextOutW(dc, x, y, options & ~ETO_OPAQUE, &rc2, text, c, dx);
		x -= 1, y -= 1; ::OffsetRect(&rc2, -1, -1);
	}

	::SetTextColor(dc, textForeColor);
	true_ExtTextOutW(dc, x, y, options & ~ETO_OPAQUE, &rc2, text, c, dx);

	::SetBkMode(dc, oldBkMode);
	::SetBkColor(dc, oldBkColor);
	::SetTextColor(dc, oldTextColor);
}

//--------------------------------------------------------------------
} // namespace my
//--------------------------------------------------------------------
