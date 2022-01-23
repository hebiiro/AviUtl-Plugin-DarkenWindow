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

WNDPROC getWndProc(HINSTANCE instance, LPCSTR className)
{
	WNDCLASSEXA wc = { sizeof(wc) };
	::GetClassInfoExA(instance, className, &wc);
	return wc.lpfnWndProc;
}

WNDPROC getWndProc(HINSTANCE instance, LPCWSTR className)
{
	WNDCLASSEXW wc = { sizeof(wc) };
	::GetClassInfoExW(instance, className, &wc);
	return wc.lpfnWndProc;
}

#define ATTACH_WND_HOOK_PROC(instance, className, procName) \
do \
{ \
	true_##procName = getWndProc(instance, className); \
	MY_TRACE_HEX(true_##procName); \
	ATTACH_HOOK_PROC(procName); \
} \
while (0)

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

	registerRendererNC(hwnd, VSCLASS_MENU, &g_menu);
	registerRenderer(hwnd, VSCLASS_SCROLLBAR, &g_scrollbar);
	registerRenderer(hwnd, VSCLASS_STATIC, &g_static);
	registerRenderer(hwnd, VSCLASS_BUTTON, &g_button);
	registerRenderer(hwnd, VSCLASS_EDIT, &g_edit);
	registerRenderer(hwnd, VSCLASS_LISTBOX, &g_listbox);
	registerRenderer(hwnd, VSCLASS_COMBOBOX, &g_combobox);
	registerRenderer(hwnd, VSCLASS_TOOLTIP, &g_tooltip);
	registerRenderer(hwnd, VSCLASS_TRACKBAR, &g_trackbar);
	registerRenderer(hwnd, VSCLASS_SPIN, &g_spin);
}

//---------------------------------------------------------------------

IMPLEMENT_HOOK_PROC(HRESULT, WINAPI, DrawThemeParentBackground, (HWND hwnd, HDC dc, LPCRECT rc))
{
#if 1
	DWORD from = *((DWORD*)&hwnd - 1);
	MY_TRACE(_T("0x%08X => DrawThemeParentBackground(0x%08X, 0x%08X, 0x%08X, (%d, %d, %d, %d))\n"),
		from, hwnd, dc, rc->left, rc->top, rc->right, rc->bottom);
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
	if (rcClip)
		MY_TRACE(_T("%d, %d, %d, %d\n"), rcClip->left, rcClip->top, rcClip->right, rcClip->bottom);
#endif
	return true_DrawThemeBackground(theme, dc, partId, stateId, rc, rcClip);
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

HRESULT ThemeRenderer::DrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, DWORD textFlags2, LPCRECT rc)
{
//	MY_TRACE(_T("ThemeRenderer::DrawThemeText(0x%08X, %d, %d, (%d, %d, %d, %d)), 0x%08X, 0x%08X\n"),
//		theme, partId, stateId, rc->left, rc->top, rc->right, rc->bottom, textFlags, textFlags2);

	return true_DrawThemeText(theme, dc, partId, stateId, text, c, textFlags, textFlags2, rc);
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
