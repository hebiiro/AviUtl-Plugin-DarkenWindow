#pragma once

#ifdef _DEBUG
#define MY_TRACE			(CMyTracer(_T(__FILE__), __LINE__)).output
#define MY_TRACE_BINARY		(CMyTracer(_T(__FILE__), __LINE__)).outputBinary
#else
#define MY_TRACE			(void)sizeof
#define MY_TRACE_BINARY		(void)sizeof
#endif

#define MY_TRACE_STR(xxx)	MY_TRACE(_T(#xxx) _T(" = %hs\n"), xxx)
#define MY_TRACE_WSTR(xxx)	MY_TRACE(_T(#xxx) _T(" = %ws\n"), xxx)
#define MY_TRACE_TSTR(xxx)	MY_TRACE(_T(#xxx) _T(" = %s\n"), xxx)
#define MY_TRACE_INT(xxx)	MY_TRACE(_T(#xxx) _T(" = %d\n"), xxx)
#define MY_TRACE_HEX(xxx)	MY_TRACE(_T(#xxx) _T(" = 0x%08X\n"), xxx)
#define MY_TRACE_REAL(xxx)	MY_TRACE(_T(#xxx) _T(" = %f\n"), xxx)
#define MY_TRACE_RECT(xxx)	MY_TRACE(_T(#xxx) _T(" = %d, %d, %d, %d\n"), (xxx).left, (xxx).top, (xxx).right, (xxx).bottom)
#define MY_TRACE_POINT(xxx)	MY_TRACE(_T(#xxx) _T(" = %d, %d\n"), (xxx).x, (xxx).y)
#define MY_TRACE_SIZE(xxx)	MY_TRACE(_T(#xxx) _T(" = %d, %d\n"), (xxx).cx, (xxx).cy)

#define MY_TRACE_RECT2(xxx) \
do \
{ \
	int x = (xxx).left; \
	int y = (xxx).top; \
	int w = (xxx).right - (xxx).left; \
	int h = (xxx).bottom - (xxx).top; \
	MY_TRACE(_T(#xxx) _T(" = %d, %d, %d, %d\n"), x, y, w, h); \
} \
while (0)

#define MY_TRACE_HWND(hwnd) \
do \
{ \
	TCHAR windowText[MAX_PATH] = {}; \
	::GetWindowText(hwnd, windowText, MAX_PATH); \
	TCHAR className[MAX_PATH] = {}; \
	::GetClassName(hwnd, className, MAX_PATH); \
	MY_TRACE(_T(#hwnd) _T(" = 0x%08X = \"%s\" (%s)\n"), hwnd, windowText, className); \
} \
while (0)

#define MY_TRACE_COM_ERROR(hr) MY_TRACE(_T("0x%08X = %s\n"), hr, _com_error(hr).ErrorMessage())

class CMyTracer
{
public:

	CMyTracer(LPCTSTR file, int line)
		: m_file(file)
		, m_line(line)
	{
	}

	void __cdecl output(LPCTSTR format, ...)
	{
		const int maxSize = 2048;

		va_list va;
		va_start(va, format);
		TCHAR text[maxSize];
		int c = ::StringCbVPrintf(text, sizeof(text), format, va);
		va_end(va);

		if (c == EOF)
			return;

		outputInternal(text);
	}

	void outputInternal(LPCTSTR text)
	{
		const int maxSize = 2048;

		TCHAR output[maxSize] = { 0 };

		if (m_file)
		{
			TCHAR fileSpec[MAX_PATH];
			::StringCbCopy(fileSpec, sizeof(fileSpec), m_file);
			::PathStripPath(fileSpec);

			::StringCbPrintf(output, sizeof(output), _T("%s(%d) : %s"), fileSpec, m_line, text);
		}
		else
		{
			::StringCbCopy(output, sizeof(output), text);
		}

		extern void ___outputLog(LPCTSTR text, LPCTSTR output);

		___outputLog(text, output);
	}

	TCHAR ch(BYTE a)
	{
		if (a > 0x09)
			return _T('A') + a - 0x0A;
		else
			return _T('0') + a;
	}

	void outputBinary(const void* buf, int c)
	{
		const BYTE* bytes = (const BYTE*)buf;

		if (!bytes)
		{
			outputInternal(_T("(null)\n"));
			return;
		}

		const int line = 16;

		TCHAR text[line * 3 + 2];

		for (int i = 0; i < c;)
		{
			int j;

			for (j = 0; i < c && j < line * 3; i++)
			{
				text[j++] = ch(bytes[i] >> 4);
				text[j++] = ch(bytes[i] & 0x0F);
				text[j++] = _T(' ');
			}

			text[j++] = _T('\n');
			text[j++] = _T('\0');

			outputInternal(text);
		}
	}

private:

	LPCTSTR m_file;
	int m_line;

};
