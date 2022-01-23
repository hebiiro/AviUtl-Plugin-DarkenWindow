#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

LRESULT AviUtlRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("AviUtlRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);

	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int AviUtlRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
//	MY_TRACE(_T("AviUtlRenderer::FillRect(0x%08X)\n"), brush);

	if (brush == (HBRUSH)(COLOR_BTNFACE + 1))
	{
		my::fillRect_Dialog(dc, rc);
		return TRUE;
	}

	COLORREF color = my::getBrushColor(brush);

	if (color == RGB(0xcc, 0xcc, 0xcc))
	{
		static HBRUSH g_brush = ::CreateSolidBrush(my::getFillColor_Window());
		brush = g_brush;
	}

	return true_FillRect(dc, rc, brush);
}

BOOL AviUtlRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("AviUtlRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL AviUtlRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("AviUtlRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL AviUtlRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("AviUtlRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL AviUtlRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
//	MY_TRACE(_T("AviUtlRenderer::DrawEdge()\n"));

	if (flags & BF_MIDDLE)
	{
		my::fillRect_Dialog(dc, rc);
	}

	switch (edge)
	{
	case BDR_RAISEDOUTER:
		{
			my::drawSingleEdge_Raised(dc, rc);
			return TRUE;
		}
	case BDR_SUNKENOUTER:
		{
			my::drawSingleEdge_Sunken(dc, rc);
			return TRUE;
		}
	case EDGE_RAISED:
		{
			my::drawDoubleEdge_Raised(dc, rc);
			return TRUE;
		}
	case EDGE_SUNKEN:
		{
			my::drawDoubleEdge_Sunken(dc, rc);
			return TRUE;
		}
	case EDGE_BUMP:
		{
			my::drawDoubleEdge_Bump(dc, rc);
			return TRUE;
		}
	case EDGE_ETCHED:
		{
			my::drawDoubleEdge_Etched(dc, rc);
			return TRUE;
		}
	}

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL AviUtlRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("AviUtlRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL AviUtlRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("AviUtlRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL AviUtlRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
#if 1
	if (options & ETO_OPAQUE)
	{
		COLORREF bkColor = ::GetBkColor(dc);
//		MY_TRACE(_T("AviUtlRenderer::ExtTextOutW(%d, %d, 0x%08X), 0x%08X\n"), x, y, options, bkColor);

		if (bkColor == ::GetSysColor(COLOR_BTNFACE))
		{
			::SetBkColor(dc, my::getFillColor_Dialog());
		}
		else if (bkColor == RGB(0xcc, 0xcc, 0xcc))
		{
			::SetBkColor(dc, my::getFillColor_Window());
		}
	}

	my::shadowTextOut_Window(dc, x, y, options, rc, text, c, dx);
	return TRUE;
#else
	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
#endif
}

BOOL AviUtlRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("AviUtlRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}
