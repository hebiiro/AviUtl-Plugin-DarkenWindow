#pragma once

//---------------------------------------------------------------------

void initThemeHook(HWND hwnd);

//---------------------------------------------------------------------
// Api Hook

DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeParentBackground, (HWND hwnd, HDC dc, LPCRECT rc));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeBackground, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeBackgroundEx, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeText, (HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeTextEx, (HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeIcon, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex));
DECLARE_HOOK_PROC(HRESULT, WINAPI, DrawThemeEdge, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect));

//---------------------------------------------------------------------

class ThemeRenderer
{
public:
	virtual HRESULT DrawThemeParentBackground(HWND hwnd, HDC dc, LPCRECT rc);
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class MenuThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class ScrollBarThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class StaticThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};


class ButtonThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class EditThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class ComboBoxThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class ListBoxThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class ToolTipThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class TrackBarThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class SpinThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class HeaderThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class ListViewThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class TreeViewThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeParentBackground(HWND hwnd, HDC dc, LPCRECT rc);
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

class ToolBarThemeRenderer : public ThemeRenderer
{
public:
	virtual HRESULT DrawThemeParentBackground(HWND hwnd, HDC dc, LPCRECT rc);
	virtual HRESULT DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip);
	virtual HRESULT DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options);
	virtual HRESULT DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc);
	virtual HRESULT DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options);
	virtual HRESULT DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int iImageIndex);
	virtual HRESULT DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect);
};

//--------------------------------------------------------------------
