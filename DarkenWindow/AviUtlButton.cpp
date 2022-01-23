#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

LRESULT AviUtlButtonRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("AviUtlButtonRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int AviUtlButtonRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("AviUtlButtonRenderer::FillRect()\n"));

	return true_FillRect(dc, rc, brush);
}

BOOL AviUtlButtonRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("AviUtlButtonRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL AviUtlButtonRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("AviUtlButtonRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL AviUtlButtonRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("AviUtlButtonRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL AviUtlButtonRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
//	MY_TRACE(_T("AviUtlButtonRenderer::DrawEdge()\n"));

	HWND hwnd = currentState->m_hwnd;
	HICON icon = (HICON)::GetWindowLong(hwnd, 0);
	int x = rc->left;
	int y = rc->top;
	int w = rc->right - rc->left;
	int h = rc->bottom - rc->top;

	int ix = x, iy = x;
	if (edge == EDGE_SUNKEN) ix += 1, iy += 1;

	my::fillRect_Dialog(dc, rc);
	::BitBlt(dc, x, y, w, h, 0, 0, 0, DSTINVERT);
//	int rop2 = ::SetROP2(dc, R2_NOTCOPYPEN);
	::DrawIconEx(dc, ix, iy, icon, w, h, 0, NULL, DI_NORMAL);
//	::SetROP2(dc, rop2);
	::BitBlt(dc, x, y, w, h, 0, 0, 0, DSTINVERT);
	if (edge == EDGE_SUNKEN)
		my::drawDoubleEdge_Sunken(dc, rc);
//	else
//		my::drawDoubleEdge_Raised(dc, rc);
	return TRUE;
//	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL AviUtlButtonRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("AviUtlButtonRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL AviUtlButtonRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("AviUtlButtonRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL AviUtlButtonRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	MY_TRACE(_T("AviUtlButtonRenderer::ExtTextOutW()\n"));

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL AviUtlButtonRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("AviUtlButtonRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}
