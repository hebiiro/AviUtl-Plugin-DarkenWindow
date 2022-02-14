#pragma once

//---------------------------------------------------------------------
// Define

#define DECLARE_INTERNAL_PROC(resultType, callType, procName, args) \
	typedef resultType (callType *Type_##procName) args; \
	Type_##procName procName = NULL

#define DECLARE_HOOK_PROC(resultType, callType, procName, args) \
	typedef resultType (callType *Type_##procName) args; \
	extern Type_##procName true_##procName; \
	resultType callType hook_##procName args

#define IMPLEMENT_HOOK_PROC(resultType, callType, procName, args) \
	Type_##procName true_##procName = procName; \
	resultType callType hook_##procName args

#define IMPLEMENT_HOOK_PROC_NULL(resultType, callType, procName, args) \
	Type_##procName true_##procName = NULL; \
	resultType callType hook_##procName args

#define GET_INTERNAL_PROC(module, procName) \
	procName = (Type_##procName)::GetProcAddress(module, #procName)

#define GET_HOOK_PROC(module, procName) \
	true_##procName = (Type_##procName)::GetProcAddress(module, #procName)

#define ATTACH_HOOK_PROC(name) DetourAttach((PVOID*)&true_##name, hook_##name)

//---------------------------------------------------------------------
// Api Hook

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
DECLARE_HOOK_PROC(INT_PTR, WINAPI, DialogBoxParamA, (HINSTANCE instance, LPCSTR templateName, HWND parent, DLGPROC dialogFunc, LPARAM initParam));
DECLARE_HOOK_PROC(INT_PTR, WINAPI, DialogBoxParamW, (HINSTANCE instance, LPCWSTR templateName, HWND parent, DLGPROC dialogFunc, LPARAM initParam));

DECLARE_HOOK_PROC(HTHEME, WINAPI, OpenThemeData, (HWND hwnd, LPCWSTR classList));
DECLARE_HOOK_PROC(HTHEME, WINAPI, OpenThemeDataForDpi, (HWND hwnd, LPCWSTR classList, UINT dpi));
DECLARE_HOOK_PROC(HTHEME, WINAPI, OpenThemeDataEx, (HWND hwnd, LPCWSTR classList, DWORD flags));
DECLARE_HOOK_PROC(HRESULT, WINAPI, SetWindowTheme, (HWND hwnd, LPCWSTR subAppName, LPCWSTR subIdList));

DECLARE_HOOK_PROC(void, CDECL, exedit_00030500, ());
DECLARE_HOOK_PROC(BOOL, CDECL, exedit_000305E0, (int objectIndex));

//---------------------------------------------------------------------
namespace Exedit {
//---------------------------------------------------------------------

extern HFONT* g_font;

BOOL WINAPI drawRootText(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx);
BOOL WINAPI drawRootEdge(HDC dc, LPRECT rc, UINT edge, UINT flags);
BOOL WINAPI drawLayerText(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCSTR text, UINT c, CONST INT* dx);
BOOL WINAPI drawLayerEdge(HDC dc, LPRECT rc, UINT edge, UINT flags);
void drawTimelineLongGuage(HDC dc, int mx, int my, int lx, int ly, HPEN pen);
void drawTimelineShortGuage(HDC dc, int mx, int my, int lx, int ly, HPEN pen);
void drawTimelineTime(HDC dc, LPCSTR text, int x, int y, int w, int h, int scroll_x);
int WINAPI fillLayerBackground(HDC dc, LPCRECT rc, HBRUSH brush);
int WINAPI fillGroupBackground(HDC dc, LPCRECT rc, HBRUSH brush);

//---------------------------------------------------------------------
} // namespace Exedit
//---------------------------------------------------------------------
// Function

// CALL を書き換える
template<class T>
void hookCall(DWORD address, T hookProc)
{
	BYTE code[5];
	code[0] = 0xE8; // CALL
	*(DWORD*)&code[1] = (DWORD)hookProc - (address + 5);

	// CALL を書き換える。そのあと命令キャッシュをフラッシュする。
	::WriteProcessMemory(::GetCurrentProcess(), (LPVOID)address, code, sizeof(code), NULL);
	::FlushInstructionCache(::GetCurrentProcess(), (LPVOID)address, sizeof(code));
}

// CALL を書き換える
template<class T>
void hookAbsoluteCall(DWORD address, T& hookProc)
{
	BYTE code[6];
	code[0] = 0xE8; // CALL
	*(DWORD*)&code[1] = (DWORD)hookProc - (address + 5);
	code[5] = 0x90; // NOP

	// CALL を書き換える。そのあと命令キャッシュをフラッシュする。
	::WriteProcessMemory(::GetCurrentProcess(), (LPVOID)address, code, sizeof(code), NULL);
	::FlushInstructionCache(::GetCurrentProcess(), (LPVOID)address, sizeof(code));
}

// 絶対アドレスを書き換える
template<class T>
void writeAbsoluteAddress(DWORD address, const T* x)
{
	// 絶対アドレスを書き換える。そのあと命令キャッシュをフラッシュする。
	::WriteProcessMemory(::GetCurrentProcess(), (LPVOID)address, &x, sizeof(x), NULL);
	::FlushInstructionCache(::GetCurrentProcess(), (LPVOID)address, sizeof(x));
}

//---------------------------------------------------------------------
