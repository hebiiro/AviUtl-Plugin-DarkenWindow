#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"

//---------------------------------------------------------------------

typedef std::map<HTHEME, ThemeRenderer*> ThemeRendererContainer;
static ThemeRendererContainer g_themeRenderers;

//---------------------------------------------------------------------

void registerRenderer(HTHEME theme, ThemeRenderer* renderer)
{
	g_themeRenderers.insert(ThemeRendererContainer::value_type(theme, renderer));
}

void registerRenderer(HWND hwnd, LPCWSTR vsclass, ThemeRenderer* renderer)
{
	HTHEME theme = ::OpenThemeData(hwnd, vsclass);
	MY_TRACE(_T("%ws == 0x%08X\n"), vsclass, theme);
	registerRenderer(theme, renderer);
}

void registerRendererNC(HWND hwnd, LPCWSTR vsclass, ThemeRenderer* renderer)
{
	UINT dpi = ::GetDpiForWindow(hwnd);
	HTHEME theme = ::OpenThemeDataForDpi(hwnd, vsclass, dpi);
//	HTHEME theme = ::OpenThemeDataEx(hwnd, vsclass, OTD_NONCLIENT);
	MY_TRACE(_T("%ws == 0x%08X\n"), vsclass, theme);
	registerRenderer(theme, renderer);
}

//---------------------------------------------------------------------

void initThemeHook(HWND hwnd)
{
	MY_TRACE(_T("initThemeHook()\n"));

	static MenuThemeRenderer g_menu;
	static ScrollBarThemeRenderer g_scrollbar;
	static StaticThemeRenderer g_static;
	static ButtonThemeRenderer g_button;
	static EditThemeRenderer g_edit;
	static ComboBoxThemeRenderer g_combobox;
	static ListBoxThemeRenderer g_listbox;
	static ToolTipThemeRenderer g_tooltip;
	static TrackBarThemeRenderer g_trackbar;
	static SpinThemeRenderer g_spin;
	static HeaderThemeRenderer g_header;
	static ListViewThemeRenderer g_listview;
	static TreeViewThemeRenderer g_treeview;
	static ToolBarThemeRenderer g_toolbar;

	registerRendererNC(hwnd, VSCLASS_MENU, &g_menu);
	registerRenderer(hwnd, VSCLASS_MENU, &g_menu);
	registerRenderer(hwnd, VSCLASS_SCROLLBAR, &g_scrollbar);
	registerRenderer(hwnd, VSCLASS_STATIC, &g_static);
	registerRenderer(hwnd, VSCLASS_BUTTON, &g_button);
	registerRenderer(hwnd, VSCLASS_EDIT, &g_edit);
	registerRenderer(hwnd, VSCLASS_LISTBOX, &g_listbox);
	registerRenderer(hwnd, VSCLASS_COMBOBOX, &g_combobox);
	registerRenderer(hwnd, VSCLASS_TOOLTIP, &g_tooltip);
	registerRenderer(hwnd, VSCLASS_TRACKBAR, &g_trackbar);
	registerRenderer(hwnd, VSCLASS_SPIN, &g_spin);
	registerRenderer(hwnd, VSCLASS_HEADER, &g_header);
	registerRenderer(hwnd, VSCLASS_LISTVIEW, &g_listview);
	registerRenderer(hwnd, VSCLASS_TREEVIEW, &g_treeview);
	registerRenderer(hwnd, VSCLASS_TOOLBAR, &g_toolbar);

	registerRendererNC(hwnd, L"Explorer::" VSCLASS_MENU, &g_menu);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_MENU, &g_menu);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_SCROLLBAR, &g_scrollbar);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_BUTTON, &g_button);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_LISTBOX, &g_listbox);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_COMBOBOX, &g_combobox);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_HEADER, &g_header);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_LISTVIEW, &g_listview);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_TREEVIEW, &g_treeview);
	registerRenderer(hwnd, L"Explorer::" VSCLASS_TOOLBAR, &g_toolbar);

//	registerRenderer(hwnd, L"CFD::" VSCLASS_EDIT, &g_edit);
//	registerRenderer(hwnd, L"Combobox::" VSCLASS_LISTBOX, &g_listbox);
	registerRenderer(hwnd, L"CFD::" VSCLASS_COMBOBOX, &g_combobox);
//	registerRenderer(hwnd, L"Combobox::" VSCLASS_COMBOBOX, &g_combobox);
	registerRenderer(hwnd, L"CFD::" VSCLASS_HEADER, &g_header);
	registerRenderer(hwnd, L"CFD::" VSCLASS_LISTVIEW, &g_listview);
	registerRenderer(hwnd, L"CFD::" VSCLASS_TREEVIEW, &g_treeview);
	registerRendererNC(hwnd, L"ItemsView::" VSCLASS_HEADER, &g_header);
	registerRendererNC(hwnd, L"ItemsView::" VSCLASS_LISTVIEW, &g_listview);
	registerRenderer(hwnd, L"Placesbar::" VSCLASS_TOOLBAR, &g_toolbar);
	registerRenderer(hwnd, L"AddressComposited::" VSCLASS_COMBOBOX, &g_combobox);
}

//---------------------------------------------------------------------

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, DrawThemeParentBackground, (HWND hwnd, HDC dc, LPCRECT rc))
{
#if 1
	DWORD from = *((DWORD*)&hwnd - 1);
//	MY_TRACE(_T("0x%08X => DrawThemeParentBackground(0x%08X, 0x%08X, 0x%08X)\n"), hwnd, dc, rc);
//	if (rc) MY_TRACE_RECT(*rc);
#endif
	return true_DrawThemeParentBackground(hwnd, dc, rc);
}

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, DrawThemeBackground, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip))
{
	auto it = g_themeRenderers.find(theme);
	if (it != g_themeRenderers.end())
		return it->second->DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
#if 1
	DWORD from = *((DWORD*)&theme - 1);
	MY_TRACE(_T("0x%08X => DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		from, theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);
	if (rcClip) MY_TRACE_RECT(*rcClip);
#endif
	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, DrawThemeBackgroundEx, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options))
{
	auto it = g_themeRenderers.find(theme);
	if (it != g_themeRenderers.end())
		return it->second->DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
#if 1
	DWORD from = *((DWORD*)&theme - 1);
	MY_TRACE(_T("0x%08X => DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		from, theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);
#endif
	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, DrawThemeText, (HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc))
{
	auto it = g_themeRenderers.find(theme);
	if (it != g_themeRenderers.end())
		return it->second->DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
#if 1
	DWORD from = *((DWORD*)&theme - 1);
	MY_TRACE(_T("0x%08X => DrawThemeText(0x%08X, %d, %d)\n"), from, theme, partId, stateId);
#endif
	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, DrawThemeTextEx, (HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options))
{
	auto it = g_themeRenderers.find(theme);
	if (it != g_themeRenderers.end())
		return it->second->DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
#if 1
	DWORD from = *((DWORD*)&theme - 1);
	MY_TRACE(_T("0x%08X => DrawThemeTextEx(0x%08X, %d, %d)\n"), from, theme, partId, stateId);
#endif
	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, DrawThemeIcon, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex))
{
	auto it = g_themeRenderers.find(theme);
	if (it != g_themeRenderers.end())
		return it->second->DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
#if 1
	DWORD from = *((DWORD*)&theme - 1);
	MY_TRACE(_T("0x%08X => DrawThemeIcon(0x%08X, %d, %d)\n"), from, theme, partId, stateId);
#endif
	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, DrawThemeEdge, (HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect))
{
	auto it = g_themeRenderers.find(theme);
	if (it != g_themeRenderers.end())
		return it->second->DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
#if 1
	DWORD from = *((DWORD*)&theme - 1);
	MY_TRACE(_T("0x%08X => DrawThemeEdge(0x%08X, %d, %d)\n"), from, theme, partId, stateId);
#endif
	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//--------------------------------------------------------------------

HRESULT ThemeRenderer::DrawThemeParentBackground(HWND hwnd, HDC dc, LPCRECT rc)
{
//	MY_TRACE(_T("ThemeRenderer::DrawThemeParentBackground()\n"));

	return true_DrawThemeParentBackground(hwnd, dc, rc);
}

HRESULT ThemeRenderer::DrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, LPCRECT rcClip)
{
//	MY_TRACE(_T("ThemeRenderer::DrawThemeBackground(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, rcClip);

	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
}

HRESULT ThemeRenderer::DrawThemeBackgroundEx(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, const DTBGOPTS* options)
{
//	MY_TRACE(_T("ThemeRenderer::DrawThemeBackgroundEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, options);

	return true_DrawThemeBackgroundEx(theme, dc, partId, stateId, rc, options);
}

HRESULT ThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
//	MY_TRACE(_T("ThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
}

HRESULT ThemeRenderer::DrawThemeTextEx(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPRECT rc, const DTTOPTS* options)
{
	MY_TRACE(_T("ThemeRenderer::DrawThemeTextEx(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags);

	return true_DrawThemeTextEx(theme, dc, partId, stateId, text, c, textFlags, rc, options);
}

HRESULT ThemeRenderer::DrawThemeIcon(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc, HIMAGELIST imageList, int imageIndex)
{
	MY_TRACE(_T("ThemeRenderer::DrawThemeIcon(0x%08X, %d, %d, (%d, %d, %d, %d))\n"),
		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom);

	return true_DrawThemeIcon(theme, dc, partId, stateId, rc, imageList, imageIndex);
}

HRESULT ThemeRenderer::DrawThemeEdge(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT destRect, UINT edge, UINT flags, LPRECT contentRect)
{
	MY_TRACE(_T("ThemeRenderer::DrawThemeEdge(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X\n"),
		theme, partId, stateId, destRect->left, destRect->top, destRect->right, destRect->bottom, contentRect);

	return true_DrawThemeEdge(theme, dc, partId, stateId, destRect, edge, flags, contentRect);
}

//---------------------------------------------------------------------
