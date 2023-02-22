#pragma once

//---------------------------------------------------------------------

extern AviUtlInternal g_auin;
extern HINSTANCE g_instance;

//---------------------------------------------------------------------

DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeParentBackground, (HWND hwnd, HDC dc, LPCRECT rc));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeBackground, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeText, (HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeIcon, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeEdge, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect));

DECLARE_HOOK_PROC(int, WINAPI, FillRect, (HDC dc, LPCRECT rc, HBRUSH brush));
DECLARE_HOOK_PROC(BOOL, WINAPI, DrawFrame, (HDC dc, LPRECT rc, UINT type, UINT state));
DECLARE_HOOK_PROC(BOOL, WINAPI, DrawFrameControl, (HDC dc, LPRECT rc, UINT type, UINT state));
DECLARE_HOOK_PROC(BOOL, WINAPI, FrameRect, (HDC dc, LPCRECT rc, HBRUSH brush));
DECLARE_HOOK_PROC(BOOL, WINAPI, DrawEdge, (HDC dc, LPRECT rc, UINT edge, UINT flags));
DECLARE_HOOK_PROC(BOOL, WINAPI, DrawFocusRect, (HDC dc, LPCRECT rc));
DECLARE_HOOK_PROC(BOOL, WINAPI, DrawStateW, (HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags));
DECLARE_HOOK_PROC(BOOL, WINAPI, ExtTextOutW, (HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx));
DECLARE_HOOK_PROC(BOOL, WINAPI, PatBlt, (HDC dc, int x, int y, int w, int h, DWORD rop));

DECLARE_HOOK_PROC(LRESULT, WINAPI, CallWindowProcInternal, (WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam));
DECLARE_HOOK_PROC(HICON, WINAPI, LoadIconA, (HINSTANCE instance, LPCSTR iconName));
DECLARE_HOOK_PROC(HICON, WINAPI, LoadIconW, (HINSTANCE instance, LPCWSTR iconName));
DECLARE_HOOK_PROC(HANDLE, WINAPI, LoadImageA, (HINSTANCE instance, LPCSTR name, UINT type, int cx, int cy, UINT flags));
DECLARE_HOOK_PROC(HANDLE, WINAPI, LoadImageW, (HINSTANCE instance, LPCWSTR name, UINT type, int cx, int cy, UINT flags));
DECLARE_HOOK_PROC(BOOL, WINAPI, DrawIconEx, (HDC dc, int x, int y, HICON icon, int w, int h, UINT step, HBRUSH brush, UINT flags));
DECLARE_HOOK_PROC(HMENU, WINAPI, LoadMenuA, (HINSTANCE instance, LPCSTR menuName));

DECLARE_HOOK_PROC(HTHEME, WINAPI, OpenThemeData, (HWND hwnd, LPCWSTR classList));
DECLARE_HOOK_PROC(HTHEME, WINAPI, OpenThemeDataForDpi, (HWND hwnd, LPCWSTR classList, UINT dpi));
DECLARE_HOOK_PROC(HTHEME, WINAPI, OpenThemeDataEx, (HWND hwnd, LPCWSTR classList, DWORD flags));
DECLARE_HOOK_PROC(HRESULT, WINAPI, SetWindowTheme, (HWND hwnd, LPCWSTR subAppName, LPCWSTR subIdList));

DECLARE_HOOK_PROC(BOOL, WINAPI, loudness_TextOutA, (HDC dc, int x, int y, LPCSTR text, int c));

//---------------------------------------------------------------------
namespace ExEdit {
//---------------------------------------------------------------------

extern HFONT* g_font;
extern int g_drawingLayerIndex;
extern int (*CallShowColorDialog)(DWORD u1, COLORREF* color, DWORD u3);

void initExEdit();

BOOL WINAPI drawRootText(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx);
BOOL WINAPI drawRootEdge(HDC dc, LPRECT rc, UINT edge, UINT flags);
BOOL WINAPI drawLayerText(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx);
BOOL WINAPI drawLayerEdge(HDC dc, LPRECT rc, UINT edge, UINT flags);
void drawTimelineLongScale(HDC dc, int mx, int my, int lx, int ly, HPEN pen);
void drawTimelineShortScale(HDC dc, int mx, int my, int lx, int ly, HPEN pen);
void drawTimelineTime(HDC dc, LPCSTR text, int x, int y, int w, int h, int scroll_x);
int WINAPI fillLayerBackground(HDC dc, LPCRECT rc, HBRUSH brush);
int WINAPI fillGroupBackground(HDC dc, LPCRECT rc, HBRUSH brush);

void drawLayerLeft(HDC dc, int mx, int my, int lx, int ly, HPEN pen);
void drawLayerRight(HDC dc, int mx, int my, int lx, int ly, HPEN pen);
void drawLayerTop(HDC dc, int mx, int my, int lx, int ly, HPEN pen);
void drawLayerBottom(HDC dc, int mx, int my, int lx, int ly, HPEN pen);
void drawLayerSeparator(HDC dc, int mx, int my, int lx, int ly, HPEN pen);

DECLARE_HOOK_PROC(void, CDECL, drawLayerBackground, (HDC dc, int layerIndex, DWORD a3, DWORD a4, DWORD a5, DWORD a6, DWORD a7));
DECLARE_HOOK_PROC(INT_PTR, CALLBACK, LayerNameDialogProc, (HWND hdlg, UINT message, WPARAM wParam, LPARAM lParam));
DECLARE_HOOK_PROC(INT_PTR, CDECL, ShowLayerNameDialog, (HINSTANCE instance, LPCSTR templateName, HWND parent, DLGPROC dialogProc));

//---------------------------------------------------------------------
} // namespace ExEdit
//---------------------------------------------------------------------

LRESULT WINAPI onNcPaint(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

//---------------------------------------------------------------------
