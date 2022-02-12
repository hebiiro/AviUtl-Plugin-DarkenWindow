#include "pch.h"
#include "DarkenWindow.h"
#include "ThemeHook.h"
#include "ClassicHook.h"
#include "MyDraw.h"

HWND getComboBox(HWND exeditObjectDialog)
{
	for (UINT i = 8200; i >= 8100; i--)
	{
		// ウィンドウハンドルを取得する。
		HWND hwnd = ::GetDlgItem(exeditObjectDialog, i);

		// コンボボックスかどうかクラス名で調べる。
		TCHAR className[MAX_PATH] = {};
		::GetClassName(hwnd, className, MAX_PATH);
		if (::lstrcmpi(className, WC_COMBOBOX) != 0) continue;

		if (::IsWindowVisible(hwnd)) // ウィンドウが可視なら
		{
			// ID - 2 のウィンドウを返す。
			return ::GetDlgItem(exeditObjectDialog, i - 2);
		}
	}

	return 0;
}

LRESULT ExeditRenderer::CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
//	MY_TRACE(_T("ExeditRenderer::CallWindowProcInternal(0x%08X, 0x%08X, 0x%08X, 0x%08X)\n"), hwnd, message, wParam, lParam);
#if 1
	if (wndProc != (WNDPROC)::GetClassLong(hwnd, GCL_WNDPROC))
		return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);

	switch (message)
	{
	case WM_PAINT:
		{
			MY_TRACE(_T("WM_PAINT, 0x%08X, 0x%08X)\n"), wParam, lParam);

			HDC dc = ::GetDC(hwnd);
			RECT rc; ::GetClientRect(hwnd, &rc);
			my::fillRect_Dialog(dc, &rc);
			::ReleaseDC(hwnd, dc);

			break;
		}
	case WM_ERASEBKGND:
		{
//			MY_TRACE(_T("WM_ERASEBKGND, 0x%08X, 0x%08X)\n"), wParam, lParam);

			HDC dc = (HDC)wParam;
			RECT rc; ::GetClientRect(hwnd, &rc);
			my::fillRect_Dialog(dc, &rc);

			return TRUE;
		}
	case WM_COMMAND:
		{
			UINT code = HIWORD(wParam);
			UINT id = LOWORD(wParam);
			HWND sender = (HWND)lParam;

//			MY_TRACE(_T("WM_COMMAND, 0x%04X, 0x%04X, 0x%08X)\n"), code, id, sender);

			if (id == 2079)
			{
				MY_TRACE(_T("アニメーション効果が追加されました\n"));

				if (wndProc == (WNDPROC)::GetClassLong(hwnd, GCL_WNDPROC))
				{
					LRESULT result = true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
					HWND combobox = getComboBox(hwnd);
					MY_TRACE_HEX(combobox);
					::SendMessage(hwnd, WM_CTLCOLOREDIT, 0, (LPARAM)combobox);
					return result;
				}
			}

			break;
		}
	case WM_NOTIFY:
		{
			NMHDR* nm = (NMHDR*)lParam;

//			MY_TRACE(_T("WM_NOTIFY, 0x%08X, 0x%08X, %d\n"), nm->hwndFrom, nm->idFrom, nm->code);

			break;
		}
	}
#endif
	return true_CallWindowProcInternal(wndProc, hwnd, message, wParam, lParam);
}

int ExeditRenderer::FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ExeditRenderer::FillRect(0x%08X)\n"), brush);

	if (brush == (HBRUSH)(COLOR_BTNFACE + 1))
	{
		my::fillRect_Dialog(dc, rc);
		return TRUE;
	}

	return true_FillRect(dc, rc, brush);
}

BOOL ExeditRenderer::DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type)
{
	MY_TRACE(_T("ExeditRenderer::DrawFrame()\n"));

	return true_DrawFrame(dc, rc, width, type);
}

BOOL ExeditRenderer::DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state)
{
	MY_TRACE(_T("ExeditRenderer::DrawFrameControl()\n"));

	return true_DrawFrameControl(dc, rc, type, state);
}

BOOL ExeditRenderer::FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush)
{
	MY_TRACE(_T("ExeditRenderer::FrameRect()\n"));

	return true_FrameRect(dc, rc, brush);
}

BOOL ExeditRenderer::DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags)
{
	MY_TRACE(_T("ExeditRenderer::DrawEdge()\n"));

	return true_DrawEdge(dc, rc, edge, flags);
}

BOOL ExeditRenderer::DrawFocusRect(State* currentState, HDC dc, LPCRECT rc)
{
	MY_TRACE(_T("ExeditRenderer::DrawFocusRect()\n"));

	return true_DrawFocusRect( dc, rc);
}

BOOL ExeditRenderer::DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags)
{
	MY_TRACE(_T("ExeditRenderer::DrawStateW()\n"));

	return true_DrawStateW(dc, fore, cb, lData, wData, x, y, cx, cy, flags);
}

BOOL ExeditRenderer::ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
//	MY_TRACE(_T("ExeditRenderer::ExtTextOutW()\n"));

	return true_ExtTextOutW(dc, x, y, options, rc, text, c, dx);
}

BOOL ExeditRenderer::PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop)
{
	MY_TRACE(_T("ExeditRenderer::PatBlt()\n"));

	return true_PatBlt(dc, x, y, w, h, rop);
}
