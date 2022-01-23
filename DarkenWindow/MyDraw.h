#pragma once

//--------------------------------------------------------------------
namespace my {
//--------------------------------------------------------------------

COLORREF getBrushColor(HBRUSH brush);

void clipRect(HDC dc, LPCRECT rc);
void clipRect(HDC dc, LPCRECT rc, int mode);
void fillRect(HDC dc, LPCRECT rc, COLORREF fillColor);
void frameRect(HDC dc, LPCRECT rc, COLORREF edgeColor, int edgeWidth);
void drawRectangle(HDC dc, LPCRECT rc, COLORREF fillColor, COLORREF edgeColor, int edgeWidth);
void drawAlphaRectangle(HDC dc, LPCRECT rc, COLORREF fillColor, COLORREF edgeColor, int edgeWidth);
void drawSingleEdge(HDC dc, LPCRECT rc, COLORREF topLeftColor, COLORREF bottomRightColor);
void drawDoubleEdge(HDC dc, LPCRECT rc, COLORREF innerTopLeftColor, COLORREF innerBottomRightColor, COLORREF outerTopLeftColor, COLORREF outerBottomRightColor);
void drawTripleEdge(HDC dc, LPCRECT rc, COLORREF innerTopLeftColor, COLORREF innerBottomRightColor, COLORREF middleTopLeftColor, COLORREF middleBottomRightColor, COLORREF outerTopLeftColor, COLORREF outerBottomRightColor);
void drawFocusRect(HDC dc, LPCRECT rc, COLORREF lineColor, UINT lineStyle);
void drawText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF textColor);
void drawShadowText(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format, COLORREF textForeColor, COLORREF textBackColor);
void drawIcon(HDC dc, LPCRECT rc, WCHAR icon, COLORREF iconColor, int iconWidth);
void drawShadowIcon(HDC dc, LPCRECT rc, WCHAR icon, UINT format, COLORREF iconForeColor, COLORREF iconBackColor, int iconWidth);

COLORREF getFillColor_Dialog();
COLORREF getFillColor_Dialog_Selected();
COLORREF getForeTextColor_Dialog();
COLORREF getBackTextColor_Dialog();

COLORREF getFillColor_Window();
COLORREF getFillColor_Window_Selected();
COLORREF getForeTextColor_Window();
COLORREF getBackTextColor_Window();

void fillRect_Dialog(HDC dc, LPCRECT rc);
void fillRect_Window(HDC dc, LPCRECT rc);
void fillRect_Gutter(HDC dc, LPCRECT rc);
void fillRect_Separator(HDC dc, LPCRECT rc);
void fillRect_TrackBar_Thumb(HDC dc, LPCRECT rc);
void fillRect_TrackBar_Thumb_Pushed(HDC dc, LPCRECT rc);
void fillRect_TrackBar_Channel(HDC dc, LPCRECT rc);

void frameRect_Menu(HDC dc, LPCRECT rc);
void frameRect_Button(HDC dc, LPCRECT rc);

void drawSingleEdge_Raised(HDC dc, LPCRECT rc);
void drawSingleEdge_Sunken(HDC dc, LPCRECT rc);
void drawSingleEdge_Etched(HDC dc, LPCRECT rc);
void drawSingleEdge_Bump(HDC dc, LPCRECT rc);
void drawDoubleEdge_Raised(HDC dc, LPCRECT rc);
void drawDoubleEdge_Sunken(HDC dc, LPCRECT rc);
void drawDoubleEdge_Etched(HDC dc, LPCRECT rc);
void drawDoubleEdge_Bump(HDC dc, LPCRECT rc);
void drawTripleEdge_Raised(HDC dc, LPCRECT rc);
void drawTripleEdge_Sunken(HDC dc, LPCRECT rc);
void drawTripleEdge_Etched(HDC dc, LPCRECT rc);
void drawTripleEdge_Bump(HDC dc, LPCRECT rc);
void drawFocusRect_Button(HDC dc, LPCRECT rc);

void drawShadowText_Dialog(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format);
void drawShadowText_Dialog_Disabled(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format);
void drawShadowText_Dialog_Selected(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format);
void drawShadowText_Dialog_Hot(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format);
void drawShadowText_Dialog_Focus(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format);
void drawShadowText_Window(HDC dc, LPCWSTR text, int length, LPCRECT rc, UINT format);

void shadowTextOut_Dialog(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
void shadowTextOut_Window(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);

void drawShadowIcon_Dialog(HDC dc, LPCRECT rc, WCHAR icon, UINT format);
void drawShadowIcon_Dialog_Disabled(HDC dc, LPCRECT rc, WCHAR icon, UINT format);
void drawShadowIcon_Dialog_Selected(HDC dc, LPCRECT rc, WCHAR icon, UINT format);
void drawShadowIcon_Dialog_Hot(HDC dc, LPCRECT rc, WCHAR icon, UINT format);
void drawShadowIcon_Dialog_Focus(HDC dc, LPCRECT rc, WCHAR icon, UINT format);

void drawAlphaRectangle_Selected(HDC dc, LPCRECT rc);
void drawAlphaRectangle_Hot(HDC dc, LPCRECT rc);

void drawStaticEdge(HDC dc, LPCRECT rc);
void drawClientEdge(HDC dc, LPCRECT rc);
void drawWindowEdge(HDC dc, LPCRECT rc);

LRESULT WINAPI onNcPaint(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//--------------------------------------------------------------------
} // namespace my
//--------------------------------------------------------------------
