#include "pch.h"
#include "DarkenWindow.h"
#include "ClassicHook.h"

//---------------------------------------------------------------------

DWORD g_tls = 0;

//---------------------------------------------------------------------

LRESULT Renderer::CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int Renderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	return true_FillRect(dc, rc, brush);
}

BOOL Renderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	return true_DrawFrame(dc, rc, width, type);
}

BOOL Renderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL Renderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	return true_FrameRect(dc, rc, brush);
}

BOOL Renderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL Renderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	return true_DrawFocusRect(dc, rc);
}

BOOL Renderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL Renderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL Renderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	return true_PatBlt(dc, x, y, w, h, rop);
}

//---------------------------------------------------------------------

State Dispatcher::getCurrentState()
{
	return m_currentState;
}

void Dispatcher::setCurrentState(const State& state)
{
	m_currentState = state;
}

void Dispatcher::setCurrentState(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_currentState.m_hwnd = hwnd;
	m_currentState.m_message = message;
	m_currentState.m_wParam = wParam;
	m_currentState.m_lParam = lParam;
}

int Dispatcher::FillRect(HDC dc, LPCRECT rc, HBRUSH brush)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->FillRect(&m_currentState, dc, rc, brush);
	return true_FillRect(dc, rc, brush);
}

BOOL Dispatcher::DrawFrame(HDC dc, LPRECT rc, UINT width, UINT type)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->DrawFrame(&m_currentState, dc, rc, width, type);
	return true_DrawFrame(dc, rc, width, type);
}

BOOL Dispatcher::DrawFrameControl(HDC dc, LPRECT rc, UINT type, UINT state)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->DrawFrameControl(&m_currentState, dc, rc, type, state);
	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL Dispatcher::FrameRect(HDC dc, LPCRECT rc, HBRUSH brush)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->FrameRect(&m_currentState, dc, rc, brush);
	return true_FrameRect(dc, rc, brush);
}

BOOL Dispatcher::DrawEdge(HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->DrawEdge(&m_currentState, dc, rc, edge, flags);
	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL Dispatcher::DrawFocusRect(HDC dc, LPCRECT rc)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->DrawFocusRect(&m_currentState, dc, rc);
	return true_DrawFocusRect(dc, rc);
}

BOOL Dispatcher::DrawStateW(HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->DrawStateW(&m_currentState, dc, fore, cb, lData, wData, x, y, cx, cy, flags);
	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL Dispatcher::ExtTextOutW(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->ExtTextOutW(&m_currentState, dc, x, y, options, rc, text, c, dx);
	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL Dispatcher::PatBlt(HDC dc, int x, int y, int w, int h, DWORD rop)
{
	Renderer* renderer = getRenderer(m_currentState.m_hwnd);
	if (renderer)
		return renderer->PatBlt(&m_currentState, dc, x, y, w, h, rop);
	return true_PatBlt(dc, x, y, w, h, rop);
}

//---------------------------------------------------------------------

ExtTextOutHookBlocker::ExtTextOutHookBlocker()
{
	m_dispatcher = getDispatcher();
	m_dispatcher->m_ExtTextOutLock = TRUE;
}

ExtTextOutHookBlocker::~ExtTextOutHookBlocker()
{
	m_dispatcher->m_ExtTextOutLock = FALSE;
}

//---------------------------------------------------------------------

void initDispatcher()
{
	MY_TRACE(_T("initDispatcher()\n"));

	g_tls = ::TlsAlloc();
}

void termDispatcher()
{
	MY_TRACE(_T("termDispatcher()\n"));

	::TlsFree(g_tls), g_tls = 0;
}

Dispatcher* getDispatcher()
{
	Dispatcher* dispatcher = (Dispatcher*)::TlsGetValue(g_tls);
	if (!dispatcher)
	{
		dispatcher = new Dispatcher();
		::TlsSetValue(g_tls, dispatcher);
	}
	return dispatcher;
}

void deleteDispatcher()
{
	MY_TRACE(_T("deleteDispatcher()\n"));

	Dispatcher* dispatcher = (Dispatcher*)::TlsGetValue(g_tls);
	if (dispatcher) delete dispatcher;
}

//---------------------------------------------------------------------

WindowRenderer g_windowRenderer;
DialogRenderer g_dialogRenderer;
StaticRenderer g_staticRenderer;
ButtonRenderer g_buttonRenderer;
EditRenderer g_editRenderer;
ComboBoxRenderer g_comboboxRenderer;
ListBoxRenderer g_listboxRenderer;
ToolTipRenderer g_tooltipRenderer;
TrackBarRenderer g_trackbarRenderer;
SpinRenderer g_spinRenderer;
ListViewRenderer g_listviewRenderer;
TreeViewRenderer g_treeviewRenderer;
ToolBarRenderer g_toolbarRenderer;
TabRenderer g_tabRenderer;
AviUtlRenderer g_aviutlRenderer;
AviUtlButtonRenderer g_aviutlButtonRenderer;
ExeditRenderer g_exeditRenderer;

void initRenderer(HWND hwnd)
{
//	MY_TRACE(_T("initRenderer(0x%08X)\n"), hwnd);

	Renderer* renderer = 0;

	TCHAR className[MAX_PATH] = {};
	::GetClassName(hwnd, className, MAX_PATH);
//	MY_TRACE_TSTR(className);
	if (::lstrcmpi(className, _T("#32770")) == 0) renderer = &g_dialogRenderer;
	else if (::lstrcmpi(className, WC_STATIC) == 0) renderer = &g_staticRenderer;
	else if (::lstrcmpi(className, WC_BUTTON) == 0) renderer = &g_buttonRenderer;
	else if (::lstrcmpi(className, WC_EDIT) == 0) renderer = &g_editRenderer;
	else if (::lstrcmpi(className, WC_COMBOBOX) == 0) renderer = &g_comboboxRenderer;
	else if (::lstrcmpi(className, WC_LISTBOX) == 0) renderer = &g_listboxRenderer;
	else if (::lstrcmpi(className, _T("ComboLBox")) == 0) renderer = &g_listboxRenderer;
	else if (::lstrcmpi(className, TOOLTIPS_CLASS) == 0) renderer = &g_tooltipRenderer;
	else if (::lstrcmpi(className, TRACKBAR_CLASS) == 0) renderer = &g_trackbarRenderer;
	else if (::lstrcmpi(className, UPDOWN_CLASS) == 0) renderer = &g_spinRenderer;
	else if (::lstrcmpi(className, WC_LISTVIEW) == 0) renderer = &g_listviewRenderer;
	else if (::lstrcmpi(className, _T("DirectUIHWND")) == 0) renderer = &g_listviewRenderer;
	else if (::lstrcmpi(className, WC_TREEVIEW) == 0) renderer = &g_treeviewRenderer;
	else if (::lstrcmpi(className, TOOLBARCLASSNAME) == 0) renderer = &g_toolbarRenderer;
	else if (::lstrcmpi(className, WC_TABCONTROL) == 0) renderer = &g_tabRenderer;
	else if (::lstrcmpi(className, _T("AviUtl")) == 0) renderer = &g_aviutlRenderer;
	else if (::lstrcmpi(className, _T("AviUtlButton")) == 0) renderer = &g_aviutlButtonRenderer;
	else if (::lstrcmpi(className, _T("ExtendedFilterClass")) == 0) renderer = &g_exeditRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") _T("Window"))) renderer = &g_dialogRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") WC_STATIC)) renderer = &g_staticRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") WC_BUTTON)) renderer = &g_buttonRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") WC_EDIT)) renderer = &g_editRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") WC_COMBOBOX)) renderer = &g_comboboxRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") WC_LISTBOX)) renderer = &g_listboxRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") TOOLTIPS_CLASS)) renderer = &g_tooltipRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") TRACKBAR_CLASS)) renderer = &g_trackbarRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") UPDOWN_CLASS)) renderer = &g_spinRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") WC_LISTVIEW)) renderer = &g_listviewRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") WC_TREEVIEW)) renderer = &g_treeviewRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") TOOLBARCLASSNAME)) renderer = &g_toolbarRenderer;
	else if (::StrStrI(className, _T("WindowsForms10.") WC_TABCONTROL)) renderer = &g_tabRenderer;

	::SetProp(hwnd, _T("DarkenWindow"), (HANDLE)renderer);
}

void termRenderer(HWND hwnd)
{
//	MY_TRACE(_T("termRenderer(0x%08X)\n"), hwnd);

	::RemoveProp(hwnd, _T("DarkenWindow"));
}

Renderer* getRenderer(HWND hwnd)
{
	return (Renderer*)::GetProp(hwnd, _T("DarkenWindow"));
}

//---------------------------------------------------------------------

IMPLEMENT_HOOK_PROC(int, WINAPI, FillRect, (HDC dc, LPCRECT rc, HBRUSH brush))
{
	DWORD from = *((DWORD*)&dc - 1);
//	MY_TRACE(_T("0x%08X => FillRect()\n"), from);
	Dispatcher* dispatcher = getDispatcher();
	return dispatcher->FillRect(dc, rc, brush);
}

IMPLEMENT_HOOK_PROC_NULL(BOOL, WINAPI, DrawFrame, (HDC dc, LPRECT rc, UINT width, UINT type))
{
	DWORD from = *((DWORD*)&dc - 1);
	MY_TRACE(_T("0x%08X => DrawFrame(%d, 0x%08X)\n"), from, width, type);
	Dispatcher* dispatcher = getDispatcher();
	return dispatcher->DrawFrame(dc, rc, width, type);
}

IMPLEMENT_HOOK_PROC(BOOL, WINAPI, DrawFrameControl, (HDC dc, LPRECT rc, UINT type, UINT state))
{
	DWORD from = *((DWORD*)&dc - 1);
	MY_TRACE(_T("0x%08X => DrawFrameControl()\n"), from);
	Dispatcher* dispatcher = getDispatcher();
	return dispatcher->DrawFrameControl(dc, rc, type, state);
}

IMPLEMENT_HOOK_PROC(BOOL, WINAPI, FrameRect, (HDC dc, LPCRECT rc, HBRUSH brush))
{
	DWORD from = *((DWORD*)&dc - 1);
	MY_TRACE(_T("0x%08X => FrameRect()\n"), from);
	Dispatcher* dispatcher = getDispatcher();
	return dispatcher->FrameRect(dc, rc, brush);
}

IMPLEMENT_HOOK_PROC(BOOL, WINAPI, DrawEdge, (HDC dc, LPRECT rc, UINT edge, UINT flags))
{
	DWORD from = *((DWORD*)&dc - 1);
//	MY_TRACE(_T("0x%08X => DrawEdge(0x%08X, 0x%08X)\n"), from, edge, flags);
	Dispatcher* dispatcher = getDispatcher();
	return dispatcher->DrawEdge(dc, rc, edge, flags);
}

IMPLEMENT_HOOK_PROC(BOOL, WINAPI, DrawFocusRect, (HDC dc, LPCRECT rc))
{
	DWORD from = *((DWORD*)&dc - 1);
	MY_TRACE(_T("0x%08X => DrawFocusRect()\n"), from);
	Dispatcher* dispatcher = getDispatcher();
	return dispatcher->DrawFocusRect( dc, rc);
}

IMPLEMENT_HOOK_PROC(BOOL, WINAPI, DrawStateW, (HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags))
{
	DWORD from = *((DWORD*)&dc - 1);
//	MY_TRACE(_T("0x%08X => DrawStateW(0x%08X)\n"), from, flags);
	Dispatcher* dispatcher = getDispatcher();
	return dispatcher->DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

IMPLEMENT_HOOK_PROC(BOOL, WINAPI, ExtTextOutW, (HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx))
{
	DWORD from = *((DWORD*)&dc - 1);
	Dispatcher* dispatcher = getDispatcher();

	if (dispatcher->m_ExtTextOutLock)
	{
//		MY_TRACE(_T("再帰呼び出し時はフックしない\n"));
		return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
	}
#if 0
	{
		// デバッグ用。

		WCHAR buffer[MAX_PATH] = {};
		if (text)
			memcpy(buffer, text, min(c * sizeof(WCHAR), MAX_PATH - 1));
		MY_TRACE(_T("0x%08X => ExtTextOutW(%ws)\n"), from, buffer);
	}
#endif
	dispatcher->m_ExtTextOutLock = TRUE;
	BOOL retValue = dispatcher->ExtTextOutW(dc, x, y, options, rc, text, c, dx);
	dispatcher->m_ExtTextOutLock = FALSE;

	return retValue;
}

IMPLEMENT_HOOK_PROC(BOOL, WINAPI, PatBlt, (HDC dc, int x, int y, int w, int h, DWORD rop))
{
	DWORD from = *((DWORD*)&dc - 1);
//	MY_TRACE(_T("0x%08X => PatBlt()\n"), from);
	Dispatcher* dispatcher = getDispatcher();
	return dispatcher->PatBlt(dc, x, y, w, h, rop);
}

//---------------------------------------------------------------------
