#pragma once

//--------------------------------------------------------------------

struct State
{
	HWND m_hwnd = 0;
	UINT m_message = 0;
	WPARAM m_wParam = 0;
	LPARAM m_lParam = 0;
};

class Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;
	virtual LRESULT CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

struct Dispatcher
{
	State m_currentState;

	State getCurrentState();
	void setCurrentState(const State& state);
	void setCurrentState(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

	int FillRect(HDC dc, LPCRECT rc, HBRUSH brush);
	BOOL DrawFrame(HDC dc, LPRECT rc, UINT width, UINT type);
	BOOL DrawFrameControl(HDC dc, LPRECT rc, UINT type, UINT state);
	BOOL FrameRect(HDC dc, LPCRECT rc, HBRUSH brush);
	BOOL DrawEdge(HDC dc, LPRECT rc, UINT edge, UINT flags);
	BOOL DrawFocusRect(HDC dc, LPCRECT rc);
	BOOL DrawStateW(HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);

	BOOL ExtTextOutW(HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	BOOL PatBlt(HDC dc, int x, int y, int w, int h, DWORD rop);
};

void initDispatcher();
void termDispatcher();
Dispatcher* getDispatcher();
void deleteDispatcher();

void initRenderer(HWND hwnd);
void termRenderer(HWND hwnd);
Renderer* getRenderer(HWND hwnd);

//---------------------------------------------------------------------

class DialogRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class ScrollBarRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class StaticRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class ButtonRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class EditRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class ListBoxRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class ComboBoxRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class ToolTipRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class TrackBarRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class SpinRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class ListViewRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class TreeViewRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class ToolBarRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual LRESULT CustomDraw(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class AviUtlRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class AviUtlButtonRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

class ExeditRenderer : public Renderer
{
public:
	virtual LRESULT CallWindowProcInternal(WNDPROC wndProc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	virtual int FillRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawFrame(State* currentState, HDC dc, LPRECT rc, UINT width, UINT type);
	virtual BOOL DrawFrameControl(State* currentState, HDC dc, LPRECT rc, UINT type, UINT state);
	virtual BOOL FrameRect(State* currentState, HDC dc, LPCRECT rc, HBRUSH brush);
	virtual BOOL DrawEdge(State* currentState, HDC dc, LPRECT rc, UINT edge, UINT flags);
	virtual BOOL DrawFocusRect(State* currentState, HDC dc, LPCRECT rc);
	virtual BOOL DrawStateW(State* currentState, HDC dc, HBRUSH fore, DRAWSTATEPROC cb, LPARAM lData, WPARAM wData, int x, int y, int cx, int cy, UINT flags);
	virtual BOOL ExtTextOutW(State* currentState, HDC dc, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	virtual BOOL PatBlt(State* currentState, HDC dc, int x, int y, int w, int h, DWORD rop);
};

//---------------------------------------------------------------------
