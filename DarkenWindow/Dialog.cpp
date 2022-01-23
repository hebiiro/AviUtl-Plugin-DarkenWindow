#include "pch.h"
#include "DarkenWindow.h"
#include "ClassicHook.h"
#include "MyDraw.h"

WNDPROC g_dialogWndProc = 0;
LRESULT my_dialogWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DWORD from = *((DWORD*)&hwnd - 1);
//	MY_TRACE(_T("my_dialogWndProc(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	switch (message)
	{
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORMSGBOX:
	case WM_CTLCOLORBTN:
	case WM_CTLCOLORSCROLLBAR:
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLOREDIT:
	case WM_CTLCOLORLISTBOX:
		{
			MY_TRACE(_T("0x%08X => WM_CTLCOLOR, 0x%08X, 0x%08X, 0x%08X, 0x%08X\n"), from, hwnd, message, wParam, lParam);

			HDC dc = (HDC)wParam;

			HBRUSH brush = (HBRUSH)::CallWindowProc(g_dialogWndProc, hwnd, message, wParam, lParam);
			COLORREF bkColor = ::GetBkColor(dc);
			MY_TRACE(_T("brush = 0x%08X, bkColor = 0x%08X\n"), brush, bkColor);

//			if (brush == (HBRUSH)(COLOR_BTNFACE + 1))
			if (bkColor == ::GetSysColor(COLOR_BTNFACE))
			{
				static HBRUSH g_brush = ::CreateSolidBrush(my::getFillColor_Dialog());
				::SetTextColor(dc, my::getForeTextColor_Dialog());
				::SetBkColor(dc, my::getFillColor_Dialog());
				return (LRESULT)g_brush;
			}

			return (LRESULT)brush;
		}
	}

	return ::CallWindowProc(g_dialogWndProc, hwnd, message, wParam, lParam);
}

LRESULT DialogRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("DialogRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), wndProc, hwnd, message, wParam, lParam);
#if 0
	switch (message)
	{
	case WM_CREATE:
		{
/*
			::CreateWindowEx(
				0,
				WC_EDIT,
				_T("DarkenWindow"),
				WS_VISIBLE | WS_CHILD,
				0, 0, 0, 0,
				hwnd,
				(HMENU)10000, 0, 0);
*/
			//::RedrawWindow(hwnd, 0, 0, RDW_ERASE | RDW_FRAME | RDW_INTERNALPAINT | RDW_INVALIDATE);
			//::SetWindowTheme(hwnd, L"explorer", 0);
			g_dialogWndProc = (WNDPROC)::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)my_dialogWndProc);

			break;
		}
	}
#endif
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int DialogRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("DialogRenderer::FillRect()\n"));

	return true_FillRect(dc, rc, brush);
}

BOOL DialogRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("DialogRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL DialogRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("DialogRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL DialogRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("DialogRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL DialogRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("DialogRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL DialogRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("DialogRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL DialogRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("DialogRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL DialogRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	MY_TRACE(_T("DialogRenderer::ExtTextOutW()\n"));

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL DialogRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("DialogRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}
