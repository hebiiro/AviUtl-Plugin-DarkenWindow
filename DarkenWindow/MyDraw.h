#pragma once

//--------------------------------------------------------------------
namespace my {
//--------------------------------------------------------------------

COLORREF getPenColor(HPEN pen);
COLORREF getBrushColor(HBRUSH brush);

void clipRect(HDC dc, LPCRECT rc);
void clipRect(HDC dc, LPCRECT rc, int mode);
void fillRect(HDC dc, LPCRECT rc, COLORREF fillColor);
void frameRect(HDC dc, LPCRECT rc, COLORREF edgeColor, int edgeWidth);
void roundRect(HDC dc, LPCRECT rc, COLORREF fillColor, COLORREF edgeColor, int edgeWidth, int roundWidth, int roundHeight);
void drawRectangle(HDC dc, LPCRECT rc, COLORREF fillColor, COLORREF edgeColor, int edgeWidth);
void drawAlphaRectangle(HDC dc, LPCRECT rc, COLORREF fillColor, COLORREF edgeColor, int edgeWidth, int alpha);
void drawAlphaRoundRect(HDC dc, LPCRECT rc, COLORREF fillColor, COLORREF edgeColor, int edgeWidth, int roundWidth, int roundHeight, int alpha);
void drawSingleEdge(HDC dc, LPCRECT rc, COLORREF topLeftColor, COLORREF bottomRightColor);
void drawDoubleEdge(HDC dc, LPCRECT rc, COLORREF innerTopLeftColor, COLORREF innerBottomRightColor, COLORREF outerTopLeftColor, COLORREF outerBottomRightColor);
void drawFocusRect(HDC dc, LPCRECT rc, COLORREF lineColor, UINT lineStyle);
void drawText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF fillColor, COLORREF textColor);
void textOut(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx, COLORREF fillColor, COLORREF textForeColor);
void drawShadowText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF fillColor, COLORREF textForeColor, COLORREF textBackColor);
void shadowTextOut(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx, COLORREF fillColor, COLORREF textForeColor, COLORREF textBackColor);

//--------------------------------------------------------------------
} // namespace my
//--------------------------------------------------------------------
