#include "pch.h"
#include "Skin.h"
#include "MyDraw.h"
#include "DarkenWindow.h"

//--------------------------------------------------------------------

Dark::Skin g_skin;

//--------------------------------------------------------------------
namespace Dark {\
//--------------------------------------------------------------------

struct ColorSet
{
	COLORREF m_fillColor;
	COLORREF m_edgeColor;
	COLORREF m_textBkColor;
	COLORREF m_textForeColor;
	COLORREF m_textBackColor;

	static const int fillColor = 0;
	static const int edgeColor = 1;
	static const int textBkColor = 2;
	static const int textForeColor = 3;
	static const int textBackColor = 4;
};

typedef std::map<_bstr_t, ColorSet> ColorSetMap;

ColorSetMap g_colorSetMap;

inline HRESULT WINAPI getPrivateProfileColor2(
	const MSXML2::IXMLDOMElementPtr& element, LPCWSTR name, COLORREF& outValue, int type)
{
	_variant_t var = element->getAttribute(name);
	if (var.vt == VT_NULL) return S_FALSE;
	_bstr_t varBSTR(var);
	auto it = g_colorSetMap.find(varBSTR);
	if (it != g_colorSetMap.end())
	{
		switch (type)
		{
		case ColorSet::fillColor: outValue = it->second.m_fillColor; return S_OK;
		case ColorSet::edgeColor: outValue = it->second.m_edgeColor; return S_OK;
		case ColorSet::textBkColor: outValue = it->second.m_textBkColor; return S_OK;
		case ColorSet::textForeColor: outValue = it->second.m_textForeColor; return S_OK;
		case ColorSet::textBackColor: outValue = it->second.m_textBackColor; return S_OK;
		}
	}

	return ::getPrivateProfileColor(element, name, outValue);
}

//--------------------------------------------------------------------

Figure::Figure()
{
	m_name = L"";
	m_ix = m_iy = m_ox = m_ox = 0;
	m_left = m_top = m_right = m_bottom = INT_MIN;
	m_addLeft = m_addTop = m_addRight = m_addBottom = INT_MIN;
}

Figure::~Figure()
{
}

void Figure::load(const MSXML2::IXMLDOMElementPtr& element)
{
	getPrivateProfileBSTR(element, L"name", m_name);

	getPrivateProfileInt(element, L"ix", m_ix);
	getPrivateProfileInt(element, L"iy", m_iy);
	getPrivateProfileInt(element, L"ox", m_ox);
	getPrivateProfileInt(element, L"oy", m_oy);

	getPrivateProfileInt(element, L"left", m_left);
	getPrivateProfileInt(element, L"top", m_top);
	getPrivateProfileInt(element, L"right", m_right);
	getPrivateProfileInt(element, L"bottom", m_bottom);

	getPrivateProfileInt(element, L"addLeft", m_addLeft);
	getPrivateProfileInt(element, L"addTop", m_addTop);
	getPrivateProfileInt(element, L"addRight", m_addRight);
	getPrivateProfileInt(element, L"addBottom", m_addBottom);
}

void Figure::draw(HDC dc, LPRECT rc)
{
	::InflateRect(rc, m_ix, m_iy);
	::OffsetRect(rc, m_ox, m_oy);

	if (m_left != INT_MIN) rc->left = m_left;
	if (m_top != INT_MIN) rc->top = m_top;
	if (m_right != INT_MIN) rc->right = m_right;
	if (m_bottom != INT_MIN) rc->bottom = m_bottom;

	if (m_addLeft != INT_MIN) rc->left += m_addLeft;
	if (m_addTop != INT_MIN) rc->top += m_addTop;
	if (m_addRight != INT_MIN) rc->right += m_addRight;
	if (m_addBottom != INT_MIN) rc->bottom += m_addBottom;
}

//--------------------------------------------------------------------

Null::Null()
{
}

Null::~Null()
{
}

void Null::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
}

void Null::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
}

//--------------------------------------------------------------------

FillRect::FillRect()
{
	m_fillColor = CLR_NONE;
}

FillRect::~FillRect()
{
}

void FillRect::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
	getPrivateProfileColor2(element, L"fillColor", m_fillColor, ColorSet::fillColor);
}

void FillRect::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	if (m_fillColor != CLR_NONE)
		my::fillRect(dc, rc, m_fillColor);
}

//--------------------------------------------------------------------

FrameRect::FrameRect()
{
	m_edgeColor = CLR_NONE;
	m_edgeWidth = 0;
}

FrameRect::~FrameRect()
{
}

void FrameRect::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
	getPrivateProfileColor2(element, L"edgeColor", m_edgeColor, ColorSet::edgeColor);
	getPrivateProfileInt(element, L"edgeWidth", m_edgeWidth);
}

void FrameRect::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	if (m_edgeColor != CLR_NONE)
		my::frameRect(dc, rc, m_edgeColor, m_edgeWidth);
}

//--------------------------------------------------------------------

RoundRect::RoundRect()
{
	m_fillColor = CLR_NONE;
	m_edgeColor = CLR_NONE;
	m_edgeWidth = 0;
	m_roundWidth = 0;
	m_roundHeight = 0;
}

RoundRect::~RoundRect()
{
}

void RoundRect::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
	getPrivateProfileColor2(element, L"fillColor", m_fillColor, ColorSet::fillColor);
	getPrivateProfileColor2(element, L"edgeColor", m_edgeColor, ColorSet::edgeColor);
	getPrivateProfileInt(element, L"edgeWidth", m_edgeWidth);
	getPrivateProfileInt(element, L"roundWidth", m_roundWidth);
	getPrivateProfileInt(element, L"roundHeight", m_roundHeight);
}

void RoundRect::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	my::roundRect(dc, rc, m_fillColor, m_edgeColor, m_edgeWidth, m_roundWidth, m_roundHeight);
}

//--------------------------------------------------------------------

DrawAlphaRectangle::DrawAlphaRectangle()
{
	m_fillColor = CLR_NONE;
	m_edgeColor = CLR_NONE;
	m_edgeWidth = 0;
}

DrawAlphaRectangle::~DrawAlphaRectangle()
{
}

void DrawAlphaRectangle::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
	getPrivateProfileColor2(element, L"fillColor", m_fillColor, ColorSet::fillColor);
	getPrivateProfileColor2(element, L"edgeColor", m_edgeColor, ColorSet::edgeColor);
	getPrivateProfileInt(element, L"edgeWidth", m_edgeWidth);
}

void DrawAlphaRectangle::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	my::drawAlphaRectangle(dc, rc, m_fillColor, m_edgeColor, m_edgeWidth);
}

//--------------------------------------------------------------------

COLORREF DrawSingleRaisedEdge::m_topLeftColor = CLR_NONE;
COLORREF DrawSingleRaisedEdge::m_bottomRightColor = CLR_NONE;

DrawSingleRaisedEdge::DrawSingleRaisedEdge()
{
}

DrawSingleRaisedEdge::~DrawSingleRaisedEdge()
{
}

void DrawSingleRaisedEdge::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
}

void DrawSingleRaisedEdge::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	my::drawSingleEdge(dc, rc, m_topLeftColor, m_bottomRightColor);
}

//--------------------------------------------------------------------

COLORREF DrawSingleSunkenEdge::m_topLeftColor = CLR_NONE;
COLORREF DrawSingleSunkenEdge::m_bottomRightColor = CLR_NONE;

DrawSingleSunkenEdge::DrawSingleSunkenEdge()
{
}

DrawSingleSunkenEdge::~DrawSingleSunkenEdge()
{
}

void DrawSingleSunkenEdge::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
}

void DrawSingleSunkenEdge::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	my::drawSingleEdge(dc, rc, m_topLeftColor, m_bottomRightColor);
}

//--------------------------------------------------------------------

COLORREF DrawDoubleRaisedEdge::m_innerTopLeftColor = CLR_NONE;
COLORREF DrawDoubleRaisedEdge::m_innerBottomRightColor = CLR_NONE;
COLORREF DrawDoubleRaisedEdge::m_outerTopLeftColor = CLR_NONE;
COLORREF DrawDoubleRaisedEdge::m_outerBottomRightColor = CLR_NONE;

DrawDoubleRaisedEdge::DrawDoubleRaisedEdge()
{
}

DrawDoubleRaisedEdge::~DrawDoubleRaisedEdge()
{
}

void DrawDoubleRaisedEdge::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
}

void DrawDoubleRaisedEdge::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	my::drawDoubleEdge(dc, rc,
		m_innerTopLeftColor, m_innerBottomRightColor,
		m_outerTopLeftColor, m_outerBottomRightColor);
}

//--------------------------------------------------------------------

COLORREF DrawDoubleSunkenEdge::m_innerTopLeftColor = CLR_NONE;
COLORREF DrawDoubleSunkenEdge::m_innerBottomRightColor = CLR_NONE;
COLORREF DrawDoubleSunkenEdge::m_outerTopLeftColor = CLR_NONE;
COLORREF DrawDoubleSunkenEdge::m_outerBottomRightColor = CLR_NONE;

DrawDoubleSunkenEdge::DrawDoubleSunkenEdge()
{
}

DrawDoubleSunkenEdge::~DrawDoubleSunkenEdge()
{
}

void DrawDoubleSunkenEdge::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
}

void DrawDoubleSunkenEdge::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	my::drawDoubleEdge(dc, rc,
		m_innerTopLeftColor, m_innerBottomRightColor,
		m_outerTopLeftColor, m_outerBottomRightColor);
}

//--------------------------------------------------------------------

COLORREF DrawDoubleBumpEdge::m_innerTopLeftColor = CLR_NONE;
COLORREF DrawDoubleBumpEdge::m_innerBottomRightColor = CLR_NONE;
COLORREF DrawDoubleBumpEdge::m_outerTopLeftColor = CLR_NONE;
COLORREF DrawDoubleBumpEdge::m_outerBottomRightColor = CLR_NONE;

DrawDoubleBumpEdge::DrawDoubleBumpEdge()
{
}

DrawDoubleBumpEdge::~DrawDoubleBumpEdge()
{
}

void DrawDoubleBumpEdge::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
}

void DrawDoubleBumpEdge::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	my::drawDoubleEdge(dc, rc,
		m_innerTopLeftColor, m_innerBottomRightColor,
		m_outerTopLeftColor, m_outerBottomRightColor);
}

//--------------------------------------------------------------------

COLORREF DrawDoubleEtchedEdge::m_innerTopLeftColor = CLR_NONE;
COLORREF DrawDoubleEtchedEdge::m_innerBottomRightColor = CLR_NONE;
COLORREF DrawDoubleEtchedEdge::m_outerTopLeftColor = CLR_NONE;
COLORREF DrawDoubleEtchedEdge::m_outerBottomRightColor = CLR_NONE;

DrawDoubleEtchedEdge::DrawDoubleEtchedEdge()
{
}

DrawDoubleEtchedEdge::~DrawDoubleEtchedEdge()
{
}

void DrawDoubleEtchedEdge::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
}

void DrawDoubleEtchedEdge::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);
	my::drawDoubleEdge(dc, rc,
		m_innerTopLeftColor, m_innerBottomRightColor,
		m_outerTopLeftColor, m_outerBottomRightColor);
}

//--------------------------------------------------------------------

TextFigure::TextFigure()
{
	m_fontName = L"";
	m_fillColor = CLR_NONE;
	m_foreColor = CLR_NONE;
	m_backColor = CLR_NONE;
	m_format = UINT_MAX;
}

TextFigure::~TextFigure()
{
}

void TextFigure::load(const MSXML2::IXMLDOMElementPtr& element)
{
	Figure::load(element);
	getPrivateProfileBSTR(element, L"fontName", m_fontName);
	getPrivateProfileColor2(element, L"fillColor", m_fillColor, ColorSet::fillColor);
	getPrivateProfileColor2(element, L"foreColor", m_foreColor, ColorSet::textForeColor);
	getPrivateProfileColor2(element, L"backColor", m_backColor, ColorSet::textBackColor);
	getPrivateProfileInt(element, L"format", m_format);
}

void TextFigure::drawText(HDC dc, LPRECT rc, LPCWSTR text, int c, UINT format)
{
	Figure::draw(dc, rc);

	my::drawShadowText(dc, text, c, rc, format, m_fillColor, m_foreColor, m_backColor);
}

void TextFigure::textOut(HDC dc, int x, int y, UINT options, LPRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	Figure::draw(dc, rc);

	my::shadowTextOut(dc, x, y, options, rc, text, c, dx, m_fillColor, m_foreColor, m_backColor);
}

//--------------------------------------------------------------------

DrawText::DrawText()
{
}

DrawText::~DrawText()
{
}

void DrawText::load(const MSXML2::IXMLDOMElementPtr& element)
{
	TextFigure::load(element);
}

void DrawText::drawText(HDC dc, LPRECT rc, LPCWSTR text, int c, UINT format)
{
	Figure::draw(dc, rc);

	my::drawText(dc, text, c, rc, format, m_fillColor, m_foreColor);
}

void DrawText::textOut(HDC dc, int x, int y, UINT options, LPRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	Figure::draw(dc, rc);

	my::textOut(dc, x, y, options, rc, text, c, dx, m_fillColor, m_foreColor);
}

//--------------------------------------------------------------------

IconFigure::IconFigure()
{
	m_text = L"";
}

IconFigure::~IconFigure()
{
}

void IconFigure::load(const MSXML2::IXMLDOMElementPtr& element)
{
	TextFigure::load(element);
	getPrivateProfileBSTR(element, L"text", m_text);
}

void IconFigure::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);

	int length = ::lstrlenW(m_text);

	RECT rc2 = *rc;
	int w = rc2.right - rc2.left;
	int h = rc2.bottom - rc2.top;
	int size = min(w, h);

	UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	HFONT font = ::CreateFontW(-size, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, m_fontName);
	HFONT oldFont = (HFONT)::SelectObject(dc, font);
	int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
	COLORREF oldTextColor = ::GetTextColor(dc);

	if (m_backColor != CLR_NONE)
	{
		::OffsetRect(&rc2, +1, +1);
		::SetTextColor(dc, m_backColor);
		for (int i = 0; i < length; i++) ::DrawTextW(dc, (BSTR)m_text + i, 1, &rc2, format);
		::OffsetRect(&rc2, -1, -1);
	}
	if (m_foreColor != CLR_NONE)
	{
		::SetTextColor(dc, m_foreColor);
		for (int i = 0; i < length; i++) ::DrawTextW(dc, (BSTR)m_text + i, 1, &rc2, format);
	}

	::SetTextColor(dc, oldTextColor);
	::SetBkMode(dc, oldBkMode);
	::SelectObject(dc, oldFont);
	::DeleteObject(font);
}

//--------------------------------------------------------------------

DrawIcon::DrawIcon()
{
}

DrawIcon::~DrawIcon()
{
}

void DrawIcon::load(const MSXML2::IXMLDOMElementPtr& element)
{
	IconFigure::load(element);
}

void DrawIcon::draw(HDC dc, LPRECT rc)
{
	Figure::draw(dc, rc);

	int length = ::lstrlenW(m_text);

	RECT rc2 = *rc;
	int w = rc2.right - rc2.left;
	int h = rc2.bottom - rc2.top;
	int size = min(w, h);

	UINT format = DT_NOCLIP | DT_CENTER | DT_VCENTER | DT_SINGLELINE;

	HFONT font = ::CreateFontW(-size, 0, 0, 0, 0, FALSE, FALSE, FALSE, DEFAULT_CHARSET, 0, 0, 0, 0, m_fontName);
	HFONT oldFont = (HFONT)::SelectObject(dc, font);
	int oldBkMode = ::SetBkMode(dc, TRANSPARENT);
	COLORREF oldTextColor = ::SetTextColor(dc, m_foreColor);

	for (int i = 0; i < length; i++) ::DrawTextW(dc, (BSTR)m_text + i, 1, &rc2, format);

	::SetTextColor(dc, oldTextColor);
	::SetBkMode(dc, oldBkMode);
	::SelectObject(dc, oldFont);
	::DeleteObject(font);
}

//--------------------------------------------------------------------

State::State()
{
	m_fillColor = CLR_NONE;
	m_textBkColor = CLR_NONE;
	m_textForeColor = CLR_NONE;
	m_textBackColor = CLR_NONE;
	m_fillBrush = 0;
}

State::~State()
{
	deleteFillBrush();
}

HBRUSH State::getFillBrush()
{
	if (!m_fillBrush && m_fillColor != CLR_NONE)
		m_fillBrush = ::CreateSolidBrush(m_fillColor);
	return m_fillBrush;
}

void State::deleteFillBrush()
{
	if (m_fillBrush)
	{
		::DeleteObject(m_fillBrush);
		m_fillBrush = 0;
	}
}

//--------------------------------------------------------------------

Part::Part()
{
}

StatePtr Part::addState(int stateId)
{
	if ((int)m_stateArray.size() <= stateId)
		m_stateArray.resize(stateId + 1);

	if (!m_stateArray[stateId])
		m_stateArray[stateId] = StatePtr(new State());

	return m_stateArray[stateId];
}

StatePtr Part::getState(int stateId)
{
	if (stateId < 0 || stateId >= (int)m_stateArray.size())
		return 0;

	return m_stateArray[stateId];
}

//--------------------------------------------------------------------

VSClass::VSClass()
{
}

LPCWSTR VSClass::getId() const
{
	return m_id;
}

void VSClass::setId(LPCWSTR id)
{
	m_id = id;
}

PartPtr VSClass::addPart(int partId)
{
	if ((int)m_partArray.size() <= partId)
		m_partArray.resize(partId + 1);

	if (!m_partArray[partId])
		m_partArray[partId] = PartPtr(new Part());

	return m_partArray[partId];
}

PartPtr VSClass::getPart(int partId)
{
	if (partId < 0 || partId >= (int)m_partArray.size())
		return 0;

	return m_partArray[partId];
}

//--------------------------------------------------------------------

Skin::Skin()
{
	m_instance = 0;
	m_hwnd = 0;
	memset(m_themes, 0, sizeof(m_themes));

	m_dwm.m_darkMode = -1;
	m_dwm.m_cornerMode = -1;
	m_dwm.m_activeBorderColor = -1;
	m_dwm.m_activeCaptionColor = -1;
	m_dwm.m_activeTextColor = -1;
	m_dwm.m_inactiveBorderColor = -1;
	m_dwm.m_inactiveCaptionColor = -1;
	m_dwm.m_inactiveTextColor = -1;
}

Skin::~Skin()
{
	for (auto theme : m_themes)
		::CloseThemeData(theme);
}

void CALLBACK Skin::timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time)
{
	Skin* skin = (Skin*)timerId;

	if (skin->reloadSettings(FALSE))
		::EnumWindows(enumWindowsProc, (LPARAM)skin);
}

BOOL CALLBACK Skin::enumWindowsProc(HWND hwnd, LPARAM lParam)
{
	Skin* skin = (Skin*)lParam;

	DWORD pid = 0;
	DWORD tid = ::GetWindowThreadProcessId(hwnd, &pid);

	if (pid == ::GetCurrentProcessId())
	{
		MY_TRACE(_T("Skin::enumWindowsProc(0x%08X)\n"), hwnd);

		::RedrawWindow(hwnd, 0, 0,
			RDW_ERASE | RDW_FRAME | RDW_INTERNALPAINT |
			RDW_INVALIDATE | RDW_ALLCHILDREN);
	}

	return TRUE;
}

void Skin::init(HINSTANCE instance, HWND hwnd)
{
	MY_TRACE(_T("Skin::init()\n"));

	m_instance = instance;
	m_hwnd = hwnd;
	m_themes[THEME_MENU] = ::OpenThemeData(m_hwnd, VSCLASS_MENU);
	m_themes[THEME_SCROLLBAR] = ::OpenThemeData(m_hwnd, VSCLASS_SCROLLBAR);
	m_themes[THEME_STATIC] = ::OpenThemeData(m_hwnd, VSCLASS_STATIC);
	m_themes[THEME_BUTTON] = ::OpenThemeData(m_hwnd, VSCLASS_BUTTON);
	m_themes[THEME_EDIT] = ::OpenThemeData(m_hwnd, VSCLASS_EDIT);
	m_themes[THEME_LISTBOX] = ::OpenThemeData(m_hwnd, VSCLASS_LISTBOX);
	m_themes[THEME_COMBOBOX] = ::OpenThemeData(m_hwnd, VSCLASS_COMBOBOX);
	m_themes[THEME_TOOLTIP] = ::OpenThemeData(m_hwnd, VSCLASS_TOOLTIP);
	m_themes[THEME_TRACKBAR] = ::OpenThemeData(m_hwnd, VSCLASS_TRACKBAR);
	m_themes[THEME_SPIN] = ::OpenThemeData(m_hwnd, VSCLASS_SPIN);
	m_themes[THEME_HEADER] = ::OpenThemeData(m_hwnd, VSCLASS_HEADER);
	m_themes[THEME_LISTVIEW] = ::OpenThemeData(m_hwnd, VSCLASS_LISTVIEW);
	m_themes[THEME_TREEVIEW] = ::OpenThemeData(m_hwnd, VSCLASS_TREEVIEW);
	m_themes[THEME_TOOLBAR] = ::OpenThemeData(m_hwnd, VSCLASS_TOOLBAR);
	m_themes[THEME_WINDOW] = ::OpenThemeData(m_hwnd, VSCLASS_WINDOW);
	m_themes[THEME_DIALOG] = ::OpenThemeData(m_hwnd, VSCLASS_WINDOW);
	m_themes[THEME_CTLCOLOR] = ::OpenThemeData(m_hwnd, VSCLASS_WINDOW);
	m_themes[THEME_EXEDIT] = ::OpenThemeData(m_hwnd, VSCLASS_WINDOW);

	::SetTimer(m_hwnd, (UINT_PTR)this, 1000, timerProc);
}

BOOL Skin::reloadSettings(BOOL force)
{
	if (force)
		MY_TRACE(_T("Skin::reloadSettings()\n"));

	if (!force && !m_fileUpdateCheckers.isFileUpdated())
		return FALSE;

	for (auto x : m_vsclassMap)
		::CloseThemeData(x.first);

	m_fileUpdateCheckers.clear();
	m_figureMap.clear();
	m_iconFigureMap.clear();
	m_textFigureMap.clear();
	m_vsclassMap.clear();

	WCHAR fileName[MAX_PATH] = {};
	::GetModuleFileNameW(m_instance, fileName, MAX_PATH);
	::PathRemoveExtensionW(fileName);
	::PathAppendW(fileName, L"DarkenWindowSettings.xml");

	reloadSettingsInternal(fileName);
	if (!force)
		reloadExeditSettings();

	return TRUE;
}

void Skin::reloadSettingsInternal(LPCWSTR fileName)
{
	MY_TRACE(_T("Skin::reloadSettingsInternal(%ws)\n"), fileName);

	m_fileUpdateCheckers.add(fileName);

	WCHAR folder[MAX_PATH] = {};
	::StringCbCopyW(folder, sizeof(folder), fileName);
	::PathRemoveFileSpecW(folder);

	try
	{
		MSXML2::IXMLDOMDocumentPtr document(__uuidof(MSXML2::DOMDocument));

		if (document->load(fileName) == VARIANT_FALSE)
		{
			MY_TRACE(_T("%ws file loading failed\n"), fileName);

			return;
		}

		{
			// スキンファイルを読み込む。

			MSXML2::IXMLDOMNodeListPtr nodeList =
				document->documentElement->getElementsByTagName(L"Skin");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				_bstr_t fileName;
				getPrivateProfileBSTR(element, L"fileName", fileName);

				WCHAR fileName2[MAX_PATH] = {};
				::StringCbCopyW(fileName2, sizeof(fileName2), folder);
				::PathAppendW(fileName2, fileName);

				reloadSkin(fileName2);
			}
		}
	}
	catch (_com_error& e)
	{
		MY_TRACE(_T("%s\n"), e.ErrorMessage());
	}
}

void Skin::reloadExeditSettings()
{
	MY_TRACE(_T("Skin::reloadExeditSettings()\n"));

	HMODULE exedit_auf = ::GetModuleHandle(_T("exedit.auf"));

	{
		HTHEME theme = getTheme(Dark::THEME_EXEDIT);
		Dark::StatePtr state = getState(theme, Dark::EXEDIT_SELECTION, 0);
		if (state && state->m_fillColor != CLR_NONE)
			writeAbsoluteAddress((DWORD)exedit_auf + 0x0003807E, &state->m_fillColor);
	}
	{
		HTHEME theme = getTheme(Dark::THEME_EXEDIT);
		Dark::StatePtr state = getState(theme, Dark::EXEDIT_SELECTIONEDGE, 0);
		if (state && state->m_fillColor != CLR_NONE)
			writeAbsoluteAddress((DWORD)exedit_auf + 0x00038076, &state->m_fillColor);
	}
	{
		HTHEME theme = getTheme(Dark::THEME_EXEDIT);
		Dark::StatePtr state = getState(theme, Dark::EXEDIT_SELECTIONBK, 0);
		if (state && state->m_fillColor != CLR_NONE)
			writeAbsoluteAddress((DWORD)exedit_auf + 0x00038087, &state->m_fillColor);
	}
}

void Skin::reloadSkin(LPCWSTR fileName)
{
	MY_TRACE(_T("Skin::reloadSkin(%ws)\n"), fileName);

	m_fileUpdateCheckers.add(fileName);

	try
	{
		MSXML2::IXMLDOMDocumentPtr document(__uuidof(MSXML2::DOMDocument));

		if (document->load(fileName) == VARIANT_FALSE)
		{
			MY_TRACE(_T("%ws file loading failed\n"), fileName);

			return;
		}

		loadAttributes(document->documentElement);
		loadFigures(document->documentElement);
		loadVSClasses(document->documentElement);
	}
	catch (_com_error& e)
	{
		MY_TRACE(_T("%s\n"), e.ErrorMessage());
	}
}

void Skin::loadAttributes(const MSXML2::IXMLDOMElementPtr& parentElement)
{
//	MY_TRACE(_T("Skin::loadAttributes()\n"));

	// <Attributes> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"Attributes");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		{
			// <DrawSingleRaisedEdge> を読み込む。

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"DrawSingleRaisedEdge");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				getPrivateProfileColor2(element, L"topLeftColor", DrawSingleRaisedEdge::m_topLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"bottomRightColor", DrawSingleRaisedEdge::m_bottomRightColor, ColorSet::edgeColor);
			}
		}

		{
			// <DrawSingleSunkenEdge> を読み込む。

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"DrawSingleSunkenEdge");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				getPrivateProfileColor2(element, L"topLeftColor", DrawSingleSunkenEdge::m_topLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"bottomRightColor", DrawSingleSunkenEdge::m_bottomRightColor, ColorSet::edgeColor);
			}
		}

		{
			// <DrawDoubleRaisedEdge> を読み込む。

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"DrawDoubleRaisedEdge");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				getPrivateProfileColor2(element, L"innerTopLeftColor", DrawDoubleRaisedEdge::m_innerTopLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"innerBottomRightColor", DrawDoubleRaisedEdge::m_innerBottomRightColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"outerTopLeftColor", DrawDoubleRaisedEdge::m_outerTopLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"outerBottomRightColor", DrawDoubleRaisedEdge::m_outerBottomRightColor, ColorSet::edgeColor);
			}
		}

		{
			// <DrawDoubleSunkenEdge> を読み込む。

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"DrawDoubleSunkenEdge");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				getPrivateProfileColor2(element, L"innerTopLeftColor", DrawDoubleSunkenEdge::m_innerTopLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"innerBottomRightColor", DrawDoubleSunkenEdge::m_innerBottomRightColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"outerTopLeftColor", DrawDoubleSunkenEdge::m_outerTopLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"outerBottomRightColor", DrawDoubleSunkenEdge::m_outerBottomRightColor, ColorSet::edgeColor);
			}
		}

		{
			// <DrawDoubleBumpEdge> を読み込む。

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"DrawDoubleBumpEdge");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				getPrivateProfileColor2(element, L"innerTopLeftColor", DrawDoubleBumpEdge::m_innerTopLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"innerBottomRightColor", DrawDoubleBumpEdge::m_innerBottomRightColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"outerTopLeftColor", DrawDoubleBumpEdge::m_outerTopLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"outerBottomRightColor", DrawDoubleBumpEdge::m_outerBottomRightColor, ColorSet::edgeColor);
			}
		}

		{
			// <DrawDoubleEtchedEdge> を読み込む。

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"DrawDoubleEtchedEdge");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				getPrivateProfileColor2(element, L"innerTopLeftColor", DrawDoubleEtchedEdge::m_innerTopLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"innerBottomRightColor", DrawDoubleEtchedEdge::m_innerBottomRightColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"outerTopLeftColor", DrawDoubleEtchedEdge::m_outerTopLeftColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"outerBottomRightColor", DrawDoubleEtchedEdge::m_outerBottomRightColor, ColorSet::edgeColor);
			}
		}

		{
			// <Dwm> を読み込む。

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"Dwm");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				getPrivateProfileColor2(element, L"activeBorderColor", m_dwm.m_activeBorderColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"activeCaptionColor", m_dwm.m_activeCaptionColor, ColorSet::fillColor);
				getPrivateProfileColor2(element, L"activeTextColor", m_dwm.m_activeTextColor, ColorSet::textForeColor);
				getPrivateProfileColor2(element, L"inactiveBorderColor", m_dwm.m_inactiveBorderColor, ColorSet::edgeColor);
				getPrivateProfileColor2(element, L"inactiveCaptionColor", m_dwm.m_inactiveCaptionColor, ColorSet::fillColor);
				getPrivateProfileColor2(element, L"inactiveTextColor", m_dwm.m_inactiveTextColor, ColorSet::textForeColor);
				getPrivateProfileInt(element, L"darkMode", m_dwm.m_darkMode);
				getPrivateProfileInt(element, L"cornerMode", m_dwm.m_cornerMode);
			}
		}

		{
			// <Icon> を読み込む。

			m_iconColorArray.clear();

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"Icon");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				// <ChangeColor> を読み込む。

				MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"ChangeColor");
				int c = nodeList->length;
				for (int i = 0; i < c; i++)
				{
					MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

					IconColor iconColor = { CLR_NONE, CLR_NONE };
					getPrivateProfileColor(element, L"src", iconColor.m_src);
					getPrivateProfileColor(element, L"dst", iconColor.m_dst);

					if (iconColor.m_src != CLR_NONE && iconColor.m_dst != CLR_NONE)
						m_iconColorArray.push_back(iconColor);
				}
			}
		}

		{
			// <ColorNames> を読み込む。

			g_colorSetMap.clear();

			MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"ColorNames");
			int c = nodeList->length;
			for (int i = 0; i < c; i++)
			{
				MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

				// <ColorName> を読み込む。

				MSXML2::IXMLDOMNodeListPtr nodeList = element->getElementsByTagName(L"ColorName");
				int c = nodeList->length;
				for (int i = 0; i < c; i++)
				{
					MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

					_bstr_t name = L"";
					getPrivateProfileString(element, L"name", name);

					ColorSet colorSet;

					colorSet.m_fillColor = CLR_NONE;
					getPrivateProfileColor(element, L"fillColor", colorSet.m_fillColor);

					colorSet.m_edgeColor = CLR_NONE;
					getPrivateProfileColor(element, L"edgeColor", colorSet.m_edgeColor);

					colorSet.m_textBkColor = CLR_NONE;
					getPrivateProfileColor(element, L"textBkColor", colorSet.m_textBkColor);

					colorSet.m_textForeColor = CLR_NONE;
					getPrivateProfileColor(element, L"textForeColor", colorSet.m_textForeColor);

					colorSet.m_textBackColor = CLR_NONE;
					getPrivateProfileColor(element, L"textBackColor", colorSet.m_textBackColor);

					g_colorSetMap[name] = colorSet;
				}
			}
		}
	}
}

void Skin::loadFigures(const MSXML2::IXMLDOMElementPtr& parentElement)
{
//	MY_TRACE(_T("Skin::loadFigures()\n"));

	// <Figures> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"Figures");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		loadFigure<Null>(element, L"Null");
		loadFigure<FillRect>(element, L"FillRect");
		loadFigure<FrameRect>(element, L"FrameRect");
		loadFigure<RoundRect>(element, L"RoundRect");
		loadFigure<DrawAlphaRectangle>(element, L"DrawAlphaRectangle");
		loadFigure<DrawSingleRaisedEdge>(element, L"DrawSingleRaisedEdge");
		loadFigure<DrawSingleSunkenEdge>(element, L"DrawSingleSunkenEdge");
		loadFigure<DrawDoubleRaisedEdge>(element, L"DrawDoubleRaisedEdge");
		loadFigure<DrawDoubleSunkenEdge>(element, L"DrawDoubleSunkenEdge");
		loadFigure<DrawDoubleBumpEdge>(element, L"DrawDoubleBumpEdge");
		loadFigure<DrawDoubleEtchedEdge>(element, L"DrawDoubleEtchedEdge");
		loadTextFigure<DrawText>(element, L"DrawText");
		loadTextFigure<DrawShadowText>(element, L"DrawShadowText");
		loadIconFigure<DrawIcon>(element, L"DrawIcon");
		loadIconFigure<DrawShadowIcon>(element, L"DrawShadowIcon");
	}
}

void Skin::loadVSClasses(const MSXML2::IXMLDOMElementPtr& parentElement)
{
//	MY_TRACE(_T("Skin::loadVSClasses()\n"));

	// <VSClasses> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"VSClasses");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		loadVSClass(element);
	}
}

void Skin::loadVSClass(const MSXML2::IXMLDOMElementPtr& parentElement)
{
//	MY_TRACE(_T("Skin::loadVSClass()\n"));

	// <VSClass> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"VSClass");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		_bstr_t id = L"";
		getPrivateProfileBSTR(element, L"id", id);

		VSClassPtr vsclass = VSClassPtr(new VSClass());
		vsclass->setId(id);

		loadName(element, vsclass);
		loadPart(element, vsclass);
	}
}

void Skin::loadName(const MSXML2::IXMLDOMElementPtr& parentElement, VSClassPtr& vsclass)
{
//	MY_TRACE(_T("Skin::loadName()\n"));

	// <Name> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"Name");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		BOOL dpi = FALSE;
		getPrivateProfileBool(element, L"dpi", dpi);
		_bstr_t name = element->text;

		HTHEME theme;
		if (dpi)
		{
			UINT dpi = ::GetDpiForWindow(m_hwnd);
			theme = ::OpenThemeDataForDpi(m_hwnd, name, dpi);
		}
		else
		{
			theme = ::OpenThemeData(m_hwnd, name);
		}
//		MY_TRACE_HEX(theme);

		if (m_vsclassMap[theme])
		{
			// theme はすでに存在するので閉じる。
			::CloseThemeData(theme);

			// 既存の vsclass を返す。
			vsclass = m_vsclassMap[theme];
		}
		else
		{
			// theme と vsclass を格納する。
			m_vsclassMap[theme] = vsclass;
		}
	}
}

void Skin::loadPart(const MSXML2::IXMLDOMElementPtr& parentElement, const VSClassPtr& vsclass)
{
//	MY_TRACE(_T("Skin::loadPart()\n"));

	// <Part> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"Part");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		_bstr_t partIdString = L"";
		getPrivateProfileBSTR(element, L"id", partIdString);
//		MY_TRACE_WSTR((LPCWSTR)partIdString);

		PartPtr part = vsclass->addPart(getPartId(partIdString));

		loadState(element, part);
	}
}

void Skin::loadState(const MSXML2::IXMLDOMElementPtr& parentElement, const PartPtr& part)
{
//	MY_TRACE(_T("Skin::loadState()\n"));

	// <State> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"State");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		_bstr_t stateIdString = L"";
		getPrivateProfileBSTR(element, L"id", stateIdString);
//		MY_TRACE_WSTR((LPCWSTR)stateIdString);

		StatePtr state = part->addState(getStateId(stateIdString));

		getPrivateProfileColor2(element, L"fillColor", state->m_fillColor, ColorSet::fillColor);
		getPrivateProfileColor2(element, L"textBkColor", state->m_textBkColor, ColorSet::textBkColor);
		getPrivateProfileColor2(element, L"textForeColor", state->m_textForeColor, ColorSet::textForeColor);
		getPrivateProfileColor2(element, L"textBackColor", state->m_textBackColor, ColorSet::textBackColor);

		state->deleteFillBrush();

		loadFigure(element, state);
		loadIconFigure(element, state);
		loadTextFigure(element, state);
	}
}

void Skin::loadFigure(const MSXML2::IXMLDOMElementPtr& parentElement, const StatePtr& state)
{
//	MY_TRACE(_T("Skin::loadFigure()\n"));

	// <Figure> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"Figure");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		_bstr_t name = L"";
		getPrivateProfileBSTR(element, L"name", name);
		FigurePtr figure = getFigure(name);
//		MY_TRACE(_T("%ws => 0x%08X\n"), (LPCWSTR)name, figure.get());
		if (figure) state->m_figures.push_back(figure);
	}
}

void Skin::loadIconFigure(const MSXML2::IXMLDOMElementPtr& parentElement, const StatePtr& state)
{
//	MY_TRACE(_T("Skin::loadIconFigure()\n"));

	// <IconFigure> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"IconFigure");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		_bstr_t name = L"";
		getPrivateProfileBSTR(element, L"name", name);
		IconFigurePtr iconFigure = getIconFigure(name);
//		MY_TRACE(_T("%ws => 0x%08X\n"), (LPCWSTR)name, iconFigure.get());
		if (iconFigure) state->m_iconFigures.push_back(iconFigure);
	}
}

void Skin::loadTextFigure(const MSXML2::IXMLDOMElementPtr& parentElement, const StatePtr& state)
{
//	MY_TRACE(_T("Skin::loadTextFigure()\n"));

	// <TextFigure> を読み込む。

	MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->getElementsByTagName(L"TextFigure");
	int c = nodeList->length;
	for (int i = 0; i < c; i++)
	{
		MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

		_bstr_t name = L"";
		getPrivateProfileBSTR(element, L"name", name);
		TextFigurePtr textFigure = getTextFigure(name);
//		MY_TRACE(_T("%ws => 0x%08X\n"), (LPCWSTR)name, textFigure.get());
		if (textFigure) state->m_textFigures.push_back(textFigure);
	}
}

FigurePtr Skin::getFigure(LPCWSTR name)
{
	return m_figureMap[name];
}

IconFigurePtr Skin::getIconFigure(LPCWSTR name)
{
	return m_iconFigureMap[name];
}

TextFigurePtr Skin::getTextFigure(LPCWSTR name)
{
	return m_textFigureMap[name];
}

VSClassPtr Skin::getVSClass(HTHEME theme)
{
	auto it = m_vsclassMap.find(theme);
	if (it == m_vsclassMap.end()) return 0;
	return it->second;
}

VSClassPtr Skin::getVSClass(HWND hwnd, LPCWSTR vsclass)
{
	HTHEME theme = ::OpenThemeData(hwnd, vsclass);
	::CloseThemeData(theme);
	return getVSClass(theme);
}

#define IF_RETURN(name) if (::lstrcmpiW(id, L#name) == 0) return name

int Skin::getPartId(LPCWSTR id)
{
	// Window

	IF_RETURN(WP_CAPTION);
	IF_RETURN(WP_SMALLCAPTION);
	IF_RETURN(WP_MINCAPTION);
	IF_RETURN(WP_SMALLMINCAPTION);
	IF_RETURN(WP_MAXCAPTION);
	IF_RETURN(WP_SMALLMAXCAPTION);
	IF_RETURN(WP_FRAMELEFT);
	IF_RETURN(WP_FRAMERIGHT);
	IF_RETURN(WP_FRAMEBOTTOM);
	IF_RETURN(WP_SMALLFRAMELEFT);
	IF_RETURN(WP_SMALLFRAMERIGHT);
	IF_RETURN(WP_SMALLFRAMEBOTTOM);
	IF_RETURN(WP_SYSBUTTON);
	IF_RETURN(WP_MDISYSBUTTON);
	IF_RETURN(WP_MINBUTTON);
	IF_RETURN(WP_MDIMINBUTTON);
	IF_RETURN(WP_MAXBUTTON);
	IF_RETURN(WP_CLOSEBUTTON);
	IF_RETURN(WP_SMALLCLOSEBUTTON);
	IF_RETURN(WP_MDICLOSEBUTTON);
	IF_RETURN(WP_RESTOREBUTTON);
	IF_RETURN(WP_MDIRESTOREBUTTON);
	IF_RETURN(WP_HELPBUTTON);
	IF_RETURN(WP_MDIHELPBUTTON);
	IF_RETURN(WP_HORZSCROLL);
	IF_RETURN(WP_HORZTHUMB);
	IF_RETURN(WP_VERTSCROLL);
	IF_RETURN(WP_VERTTHUMB);
	IF_RETURN(WP_DIALOG);
	IF_RETURN(WP_CAPTIONSIZINGTEMPLATE);
	IF_RETURN(WP_SMALLCAPTIONSIZINGTEMPLATE);
	IF_RETURN(WP_FRAMELEFTSIZINGTEMPLATE);
	IF_RETURN(WP_SMALLFRAMELEFTSIZINGTEMPLATE);
	IF_RETURN(WP_FRAMERIGHTSIZINGTEMPLATE);
	IF_RETURN(WP_SMALLFRAMERIGHTSIZINGTEMPLATE);
	IF_RETURN(WP_FRAMEBOTTOMSIZINGTEMPLATE);
	IF_RETURN(WP_SMALLFRAMEBOTTOMSIZINGTEMPLATE);
	IF_RETURN(WP_FRAME);
	IF_RETURN(WP_BORDER);

	IF_RETURN(WINDOW_WINDOWFACE);
	IF_RETURN(WINDOW_DIALOGFACE);
	IF_RETURN(WINDOW_WINDOWEDGE);
	IF_RETURN(WINDOW_CLIENTEDGE);
	IF_RETURN(WINDOW_STATICEDGE);
	IF_RETURN(WINDOW_BORDER);
	IF_RETURN(WINDOW_RAISEDOUTEREDGE);
	IF_RETURN(WINDOW_SUNKENOUTEREDGE);
	IF_RETURN(WINDOW_RAISEDEDGE);
	IF_RETURN(WINDOW_SUNKENEDGE);
	IF_RETURN(WINDOW_BUMPEDGE);
	IF_RETURN(WINDOW_ETCHEDEDGE);

	// CtlColor

	IF_RETURN(CTLCOLORDLG);
	IF_RETURN(CTLCOLORMSGBOX);
	IF_RETURN(CTLCOLORBTN);
	IF_RETURN(CTLCOLORSTATIC);
	IF_RETURN(CTLCOLOREDIT);
	IF_RETURN(CTLCOLORLISTBOX);
	IF_RETURN(CTLCOLORSCROLLBAR);

	// ExEdit

	IF_RETURN(EXEDIT_ROOT);
	IF_RETURN(EXEDIT_LAYER);
	IF_RETURN(EXEDIT_LONGGUAGE);
	IF_RETURN(EXEDIT_SHORTGUAGE);
	IF_RETURN(EXEDIT_LAYERBACKGROUND);
	IF_RETURN(EXEDIT_GROUPBACKGROUND);
	IF_RETURN(EXEDIT_SELECTION);
	IF_RETURN(EXEDIT_SELECTIONEDGE);
	IF_RETURN(EXEDIT_SELECTIONBK);
	IF_RETURN(EXEDIT_LAYERLEFT);
	IF_RETURN(EXEDIT_LAYERTOP);
	IF_RETURN(EXEDIT_LAYERRIGHT);
	IF_RETURN(EXEDIT_LAYERBOTTOM);
	IF_RETURN(EXEDIT_LAYERSEPARATOR);

	// Menu

	IF_RETURN(MENU_MENUITEM_TMSCHEMA);
	IF_RETURN(MENU_MENUDROPDOWN_TMSCHEMA);
	IF_RETURN(MENU_MENUBARITEM_TMSCHEMA);
	IF_RETURN(MENU_MENUBARDROPDOWN_TMSCHEMA);
	IF_RETURN(MENU_CHEVRON_TMSCHEMA);
	IF_RETURN(MENU_SEPARATOR_TMSCHEMA);
	IF_RETURN(MENU_BARBACKGROUND);
	IF_RETURN(MENU_BARITEM);
	IF_RETURN(MENU_POPUPBACKGROUND);
	IF_RETURN(MENU_POPUPBORDERS);
	IF_RETURN(MENU_POPUPCHECK);
	IF_RETURN(MENU_POPUPCHECKBACKGROUND);
	IF_RETURN(MENU_POPUPGUTTER);
	IF_RETURN(MENU_POPUPITEM);
	IF_RETURN(MENU_POPUPSEPARATOR);
	IF_RETURN(MENU_POPUPSUBMENU);
	IF_RETURN(MENU_SYSTEMCLOSE);
	IF_RETURN(MENU_SYSTEMMAXIMIZE);
	IF_RETURN(MENU_SYSTEMMINIMIZE);
	IF_RETURN(MENU_SYSTEMRESTORE);

	// ScrollBar

	IF_RETURN(SBP_ARROWBTN);
	IF_RETURN(SBP_THUMBBTNHORZ);
	IF_RETURN(SBP_THUMBBTNVERT);
	IF_RETURN(SBP_LOWERTRACKHORZ);
	IF_RETURN(SBP_UPPERTRACKHORZ);
	IF_RETURN(SBP_LOWERTRACKVERT);
	IF_RETURN(SBP_UPPERTRACKVERT);
	IF_RETURN(SBP_GRIPPERHORZ);
	IF_RETURN(SBP_GRIPPERVERT);
	IF_RETURN(SBP_SIZEBOX);
	IF_RETURN(SBP_SIZEBOXBKGND);

	// Static

	IF_RETURN(STAT_TEXT);
	IF_RETURN(STAT_ETCHEDEDGE);

	// Button

	IF_RETURN(BP_PUSHBUTTON);
	IF_RETURN(BP_RADIOBUTTON);
	IF_RETURN(BP_CHECKBOX);
	IF_RETURN(BP_GROUPBOX);
	IF_RETURN(BP_USERBUTTON);
	IF_RETURN(BP_COMMANDLINK);
	IF_RETURN(BP_COMMANDLINKGLYPH);
	IF_RETURN(BP_RADIOBUTTON_HCDISABLED);
	IF_RETURN(BP_CHECKBOX_HCDISABLED);
	IF_RETURN(BP_GROUPBOX_HCDISABLED);
	IF_RETURN(BP_PUSHBUTTONDROPDOWN);

	// Edit

	IF_RETURN(EP_EDITTEXT);
	IF_RETURN(EP_CARET);
	IF_RETURN(EP_BACKGROUND);
	IF_RETURN(EP_PASSWORD);
	IF_RETURN(EP_BACKGROUNDWITHBORDER);
	IF_RETURN(EP_EDITBORDER_NOSCROLL);
	IF_RETURN(EP_EDITBORDER_HSCROLL);
	IF_RETURN(EP_EDITBORDER_VSCROLL);
	IF_RETURN(EP_EDITBORDER_HVSCROLL);

	// ListBox

	IF_RETURN(LBCP_BORDER_HSCROLL);
	IF_RETURN(LBCP_BORDER_HVSCROLL);
	IF_RETURN(LBCP_BORDER_NOSCROLL);
	IF_RETURN(LBCP_BORDER_VSCROLL);
	IF_RETURN(LBCP_ITEM);

	// ComboBox

	IF_RETURN(CP_DROPDOWNBUTTON);
	IF_RETURN(CP_BACKGROUND);
	IF_RETURN(CP_TRANSPARENTBACKGROUND);
	IF_RETURN(CP_BORDER);
	IF_RETURN(CP_READONLY);
	IF_RETURN(CP_DROPDOWNBUTTONRIGHT);
	IF_RETURN(CP_DROPDOWNBUTTONLEFT);
	IF_RETURN(CP_CUEBANNER);
	IF_RETURN(CP_DROPDOWNITEM);

	// ToolTip

	IF_RETURN(TTP_STANDARD);
	IF_RETURN(TTP_STANDARDTITLE);
	IF_RETURN(TTP_BALLOON);
	IF_RETURN(TTP_BALLOONTITLE);
	IF_RETURN(TTP_CLOSE);
	IF_RETURN(TTP_BALLOONSTEM);
	IF_RETURN(TTP_WRENCH);

	// TrackBar

	IF_RETURN(TKP_TRACK);
	IF_RETURN(TKP_TRACKVERT);
	IF_RETURN(TKP_THUMB);
	IF_RETURN(TKP_THUMBBOTTOM);
	IF_RETURN(TKP_THUMBTOP);
	IF_RETURN(TKP_THUMBVERT);
	IF_RETURN(TKP_THUMBLEFT);
	IF_RETURN(TKP_THUMBRIGHT);
	IF_RETURN(TKP_TICS);
	IF_RETURN(TKP_TICSVERT);

	// Spin

	IF_RETURN(SPNP_UP);
	IF_RETURN(SPNP_DOWN);
	IF_RETURN(SPNP_UPHORZ);
	IF_RETURN(SPNP_DOWNHORZ);

	// Header

	IF_RETURN(HP_HEADERITEM);
	IF_RETURN(HP_HEADERITEMLEFT);
	IF_RETURN(HP_HEADERITEMRIGHT);
	IF_RETURN(HP_HEADERSORTARROW);
	IF_RETURN(HP_HEADERDROPDOWN);
	IF_RETURN(HP_HEADERDROPDOWNFILTER);
	IF_RETURN(HP_HEADEROVERFLOW);

	// ListView

	IF_RETURN(LVP_LISTITEM);
	IF_RETURN(LVP_LISTGROUP);
	IF_RETURN(LVP_LISTDETAIL);
	IF_RETURN(LVP_LISTSORTEDDETAIL);
	IF_RETURN(LVP_EMPTYTEXT);
	IF_RETURN(LVP_GROUPHEADER);
	IF_RETURN(LVP_GROUPHEADERLINE);
	IF_RETURN(LVP_EXPANDBUTTON);
	IF_RETURN(LVP_COLLAPSEBUTTON);
	IF_RETURN(LVP_COLUMNDETAIL);

	// TreeView

	IF_RETURN(TVP_TREEITEM);
	IF_RETURN(TVP_GLYPH);
	IF_RETURN(TVP_BRANCH);
	IF_RETURN(TVP_HOTGLYPH);

	// ToolBar

	IF_RETURN(TP_BUTTON);
	IF_RETURN(TP_DROPDOWNBUTTON);
	IF_RETURN(TP_SPLITBUTTON);
	IF_RETURN(TP_SPLITBUTTONDROPDOWN);
	IF_RETURN(TP_SEPARATOR);
	IF_RETURN(TP_SEPARATORVERT);
	IF_RETURN(TP_DROPDOWNBUTTONGLYPH);

	return wcstol(id, 0, 0);
}

int Skin::getStateId(LPCWSTR id)
{
	// Window

	// CtlColor

	IF_RETURN(CTLCOLOR_NORMAL);
	IF_RETURN(CTLCOLOR_DISABLED);
	IF_RETURN(CTLCOLOR_READONLY);

	// ExEdit

	IF_RETURN(EXEDIT_LAYER_ACTIVE);
	IF_RETURN(EXEDIT_LAYER_INACTIVE);
	IF_RETURN(EXEDIT_LAYERBACKGROUND_ACTIVE);
	IF_RETURN(EXEDIT_LAYERBACKGROUND_INACTIVE);
	IF_RETURN(EXEDIT_GROUPBACKGROUND_ACTIVE_1);
	IF_RETURN(EXEDIT_GROUPBACKGROUND_ACTIVE_2);
	IF_RETURN(EXEDIT_GROUPBACKGROUND_ACTIVE_3);
	IF_RETURN(EXEDIT_GROUPBACKGROUND_INACTIVE_1);
	IF_RETURN(EXEDIT_GROUPBACKGROUND_INACTIVE_2);
	IF_RETURN(EXEDIT_GROUPBACKGROUND_INACTIVE_3);

	// Menu

	IF_RETURN(MB_ACTIVE);
	IF_RETURN(MB_INACTIVE);

	IF_RETURN(MBI_NORMAL);
	IF_RETURN(MBI_HOT);
	IF_RETURN(MBI_PUSHED);
	IF_RETURN(MBI_DISABLED);
	IF_RETURN(MBI_DISABLEDHOT);
	IF_RETURN(MBI_DISABLEDPUSHED);

	IF_RETURN(MC_CHECKMARKNORMAL);
	IF_RETURN(MC_CHECKMARKDISABLED);
	IF_RETURN(MC_BULLETNORMAL);
	IF_RETURN(MC_BULLETDISABLED);

	IF_RETURN(MCB_DISABLED);
	IF_RETURN(MCB_NORMAL);
	IF_RETURN(MCB_BITMAP);

	IF_RETURN(MPI_NORMAL);
	IF_RETURN(MPI_HOT);
	IF_RETURN(MPI_DISABLED);
	IF_RETURN(MPI_DISABLEDHOT);

	IF_RETURN(MSM_NORMAL);
	IF_RETURN(MSM_DISABLED);

	IF_RETURN(MSYSC_NORMAL);
	IF_RETURN(MSYSC_DISABLED);

	IF_RETURN(MSYSMX_NORMAL);
	IF_RETURN(MSYSMX_DISABLED);

	IF_RETURN(MSYSMN_NORMAL);
	IF_RETURN(MSYSMN_DISABLED);

	IF_RETURN(MSYSR_NORMAL);
	IF_RETURN(MSYSR_DISABLED);

	// ScrollBar

	IF_RETURN(ABS_UPNORMAL);
	IF_RETURN(ABS_UPHOT);
	IF_RETURN(ABS_UPPRESSED);
	IF_RETURN(ABS_UPDISABLED);
	IF_RETURN(ABS_DOWNNORMAL);
	IF_RETURN(ABS_DOWNHOT);
	IF_RETURN(ABS_DOWNPRESSED);
	IF_RETURN(ABS_DOWNDISABLED);
	IF_RETURN(ABS_LEFTNORMAL);
	IF_RETURN(ABS_LEFTHOT);
	IF_RETURN(ABS_LEFTPRESSED);
	IF_RETURN(ABS_LEFTDISABLED);
	IF_RETURN(ABS_RIGHTNORMAL);
	IF_RETURN(ABS_RIGHTHOT);
	IF_RETURN(ABS_RIGHTPRESSED);
	IF_RETURN(ABS_RIGHTDISABLED);
	IF_RETURN(ABS_UPHOVER);
	IF_RETURN(ABS_DOWNHOVER);
	IF_RETURN(ABS_LEFTHOVER);
	IF_RETURN(ABS_RIGHTHOVER);

	IF_RETURN(SCRBS_NORMAL);
	IF_RETURN(SCRBS_HOT);
	IF_RETURN(SCRBS_PRESSED);
	IF_RETURN(SCRBS_DISABLED);
	IF_RETURN(SCRBS_HOVER);

	IF_RETURN(SZB_RIGHTALIGN);
	IF_RETURN(SZB_LEFTALIGN);
	IF_RETURN(SZB_TOPRIGHTALIGN);
	IF_RETURN(SZB_TOPLEFTALIGN);
	IF_RETURN(SZB_HALFBOTTOMRIGHTALIGN);
	IF_RETURN(SZB_HALFBOTTOMLEFTALIGN);
	IF_RETURN(SZB_HALFTOPRIGHTALIGN);
	IF_RETURN(SZB_HALFTOPLEFTALIGN);

	// Static

	// Button

	IF_RETURN(PBS_NORMAL);
	IF_RETURN(PBS_HOT);
	IF_RETURN(PBS_PRESSED);
	IF_RETURN(PBS_DISABLED);
	IF_RETURN(PBS_DEFAULTED);
	IF_RETURN(PBS_DEFAULTED_ANIMATING);

	IF_RETURN(RBS_UNCHECKEDNORMAL);
	IF_RETURN(RBS_UNCHECKEDHOT);
	IF_RETURN(RBS_UNCHECKEDPRESSED);
	IF_RETURN(RBS_UNCHECKEDDISABLED);
	IF_RETURN(RBS_CHECKEDNORMAL);
	IF_RETURN(RBS_CHECKEDHOT);
	IF_RETURN(RBS_CHECKEDPRESSED);
	IF_RETURN(RBS_CHECKEDDISABLED);

	IF_RETURN(CBS_UNCHECKEDNORMAL);
	IF_RETURN(CBS_UNCHECKEDHOT);
	IF_RETURN(CBS_UNCHECKEDPRESSED);
	IF_RETURN(CBS_UNCHECKEDDISABLED);
	IF_RETURN(CBS_CHECKEDNORMAL);
	IF_RETURN(CBS_CHECKEDHOT);
	IF_RETURN(CBS_CHECKEDPRESSED);
	IF_RETURN(CBS_CHECKEDDISABLED);
	IF_RETURN(CBS_MIXEDNORMAL);
	IF_RETURN(CBS_MIXEDHOT);
	IF_RETURN(CBS_MIXEDPRESSED);
	IF_RETURN(CBS_MIXEDDISABLED);
	IF_RETURN(CBS_IMPLICITNORMAL);
	IF_RETURN(CBS_IMPLICITHOT);
	IF_RETURN(CBS_IMPLICITPRESSED);
	IF_RETURN(CBS_IMPLICITDISABLED);
	IF_RETURN(CBS_EXCLUDEDNORMAL);
	IF_RETURN(CBS_EXCLUDEDHOT);
	IF_RETURN(CBS_EXCLUDEDPRESSED);
	IF_RETURN(CBS_EXCLUDEDDISABLED);

	IF_RETURN(GBS_NORMAL);
	IF_RETURN(GBS_DISABLED);

	IF_RETURN(CMDLS_NORMAL);
	IF_RETURN(CMDLS_HOT);
	IF_RETURN(CMDLS_PRESSED);
	IF_RETURN(CMDLS_DISABLED);
	IF_RETURN(CMDLS_DEFAULTED);
	IF_RETURN(CMDLS_DEFAULTED_ANIMATING);

	IF_RETURN(CMDLGS_NORMAL);
	IF_RETURN(CMDLGS_HOT);
	IF_RETURN(CMDLGS_PRESSED);
	IF_RETURN(CMDLGS_DISABLED);
	IF_RETURN(CMDLGS_DEFAULTED);

	IF_RETURN(PBDDS_NORMAL);
	IF_RETURN(PBDDS_DISABLED);

	// Edit

	IF_RETURN(ETS_NORMAL);
	IF_RETURN(ETS_HOT);
	IF_RETURN(ETS_SELECTED);
	IF_RETURN(ETS_DISABLED);
	IF_RETURN(ETS_FOCUSED);
	IF_RETURN(ETS_READONLY);
	IF_RETURN(ETS_ASSIST);
	IF_RETURN(ETS_CUEBANNER);

	IF_RETURN(EBS_NORMAL);
	IF_RETURN(EBS_HOT);
	IF_RETURN(EBS_DISABLED);
	IF_RETURN(EBS_FOCUSED);
	IF_RETURN(EBS_READONLY);
	IF_RETURN(EBS_ASSIST);

	IF_RETURN(EBWBS_NORMAL);
	IF_RETURN(EBWBS_HOT);
	IF_RETURN(EBWBS_DISABLED);
	IF_RETURN(EBWBS_FOCUSED);

	IF_RETURN(EPSN_NORMAL);
	IF_RETURN(EPSN_HOT);
	IF_RETURN(EPSN_FOCUSED);
	IF_RETURN(EPSN_DISABLED);

	IF_RETURN(EPSH_NORMAL);
	IF_RETURN(EPSH_HOT);
	IF_RETURN(EPSH_FOCUSED);
	IF_RETURN(EPSH_DISABLED);

	IF_RETURN(EPSV_NORMAL);
	IF_RETURN(EPSV_HOT);
	IF_RETURN(EPSV_FOCUSED);
	IF_RETURN(EPSV_DISABLED);

	IF_RETURN(EPSHV_NORMAL);
	IF_RETURN(EPSHV_HOT);
	IF_RETURN(EPSHV_FOCUSED);
	IF_RETURN(EPSHV_DISABLED);

	// ListBox

	IF_RETURN(LBPSH_NORMAL);
	IF_RETURN(LBPSH_FOCUSED);
	IF_RETURN(LBPSH_HOT);
	IF_RETURN(LBPSH_DISABLED);

	IF_RETURN(LBPSHV_NORMAL);
	IF_RETURN(LBPSHV_FOCUSED);
	IF_RETURN(LBPSHV_HOT);
	IF_RETURN(LBPSHV_DISABLED);

	IF_RETURN(LBPSN_NORMAL);
	IF_RETURN(LBPSN_FOCUSED);
	IF_RETURN(LBPSN_HOT);
	IF_RETURN(LBPSN_DISABLED);

	IF_RETURN(LBPSV_NORMAL);
	IF_RETURN(LBPSV_FOCUSED);
	IF_RETURN(LBPSV_HOT);
	IF_RETURN(LBPSV_DISABLED);

	IF_RETURN(LBPSI_HOT);
	IF_RETURN(LBPSI_HOTSELECTED);
	IF_RETURN(LBPSI_SELECTED);
	IF_RETURN(LBPSI_SELECTEDNOTFOCUS);

	// ComboBox

	IF_RETURN(CBXS_NORMAL);
	IF_RETURN(CBXS_HOT);
	IF_RETURN(CBXS_PRESSED);
	IF_RETURN(CBXS_DISABLED);

	IF_RETURN(CBXSR_NORMAL);
	IF_RETURN(CBXSR_HOT);
	IF_RETURN(CBXSR_PRESSED);
	IF_RETURN(CBXSR_DISABLED);

	IF_RETURN(CBXSL_NORMAL);
	IF_RETURN(CBXSL_HOT);
	IF_RETURN(CBXSL_PRESSED);
	IF_RETURN(CBXSL_DISABLED);

	IF_RETURN(CBTBS_NORMAL);
	IF_RETURN(CBTBS_HOT);
	IF_RETURN(CBTBS_DISABLED);
	IF_RETURN(CBTBS_FOCUSED);

	IF_RETURN(CBB_NORMAL);
	IF_RETURN(CBB_HOT);
	IF_RETURN(CBB_FOCUSED);
	IF_RETURN(CBB_DISABLED);

	IF_RETURN(CBRO_NORMAL);
	IF_RETURN(CBRO_HOT);
	IF_RETURN(CBRO_PRESSED);
	IF_RETURN(CBRO_DISABLED);

	IF_RETURN(CBCB_NORMAL);
	IF_RETURN(CBCB_HOT);
	IF_RETURN(CBCB_PRESSED);
	IF_RETURN(CBCB_DISABLED);

	IF_RETURN(CBDI_NORMAL);
	IF_RETURN(CBDI_HIGHLIGHTED);

	// ToolTip

	IF_RETURN(TTCS_NORMAL);
	IF_RETURN(TTCS_HOT);
	IF_RETURN(TTCS_PRESSED);

	IF_RETURN(TTSS_NORMAL);
	IF_RETURN(TTSS_LINK);

	IF_RETURN(TTBS_NORMAL);
	IF_RETURN(TTBS_LINK);

	IF_RETURN(TTBSS_POINTINGUPLEFTWALL);
	IF_RETURN(TTBSS_POINTINGUPCENTERED);
	IF_RETURN(TTBSS_POINTINGUPRIGHTWALL);
	IF_RETURN(TTBSS_POINTINGDOWNRIGHTWALL);
	IF_RETURN(TTBSS_POINTINGDOWNCENTERED);
	IF_RETURN(TTBSS_POINTINGDOWNLEFTWALL);

	IF_RETURN(TTWS_NORMAL);
	IF_RETURN(TTWS_HOT);
	IF_RETURN(TTWS_PRESSED);

	// TrackBar

	IF_RETURN(TKS_NORMAL);

	IF_RETURN(TRS_NORMAL);

	IF_RETURN(TRVS_NORMAL);

	IF_RETURN(TUS_NORMAL);
	IF_RETURN(TUS_HOT);
	IF_RETURN(TUS_PRESSED);
	IF_RETURN(TUS_FOCUSED);
	IF_RETURN(TUS_DISABLED);

	IF_RETURN(TUBS_NORMAL);
	IF_RETURN(TUBS_HOT);
	IF_RETURN(TUBS_PRESSED);
	IF_RETURN(TUBS_FOCUSED);
	IF_RETURN(TUBS_DISABLED);

	IF_RETURN(TUTS_NORMAL);
	IF_RETURN(TUTS_HOT);
	IF_RETURN(TUTS_PRESSED);
	IF_RETURN(TUTS_FOCUSED);
	IF_RETURN(TUTS_DISABLED);

	IF_RETURN(TUVS_NORMAL);
	IF_RETURN(TUVS_HOT);
	IF_RETURN(TUVS_PRESSED);
	IF_RETURN(TUVS_FOCUSED);
	IF_RETURN(TUVS_DISABLED);

	IF_RETURN(TUVLS_NORMAL);
	IF_RETURN(TUVLS_HOT);
	IF_RETURN(TUVLS_PRESSED);
	IF_RETURN(TUVLS_FOCUSED);
	IF_RETURN(TUVLS_DISABLED);

	IF_RETURN(TUVRS_NORMAL);
	IF_RETURN(TUVRS_HOT);
	IF_RETURN(TUVRS_PRESSED);
	IF_RETURN(TUVRS_FOCUSED);
	IF_RETURN(TUVRS_DISABLED);

	IF_RETURN(TSS_NORMAL);

	IF_RETURN(TSVS_NORMAL);

	// Spin

	IF_RETURN(UPS_NORMAL);
	IF_RETURN(UPS_HOT);
	IF_RETURN(UPS_PRESSED);
	IF_RETURN(UPS_DISABLED);

	IF_RETURN(DNS_NORMAL);
	IF_RETURN(DNS_HOT);
	IF_RETURN(DNS_PRESSED);
	IF_RETURN(DNS_DISABLED);

	IF_RETURN(UPHZS_NORMAL);
	IF_RETURN(UPHZS_HOT);
	IF_RETURN(UPHZS_PRESSED);
	IF_RETURN(UPHZS_DISABLED);

	IF_RETURN(DNHZS_NORMAL);
	IF_RETURN(DNHZS_HOT);
	IF_RETURN(DNHZS_PRESSED);
	IF_RETURN(DNHZS_DISABLED);

	// Header

	IF_RETURN(HBG_DETAILS);
	IF_RETURN(HBG_ICON);

	IF_RETURN(HIS_NORMAL);
	IF_RETURN(HIS_HOT);
	IF_RETURN(HIS_PRESSED);
	IF_RETURN(HIS_SORTEDNORMAL);
	IF_RETURN(HIS_SORTEDHOT);
	IF_RETURN(HIS_SORTEDPRESSED);
	IF_RETURN(HIS_ICONNORMAL);
	IF_RETURN(HIS_ICONHOT);
	IF_RETURN(HIS_ICONPRESSED);
	IF_RETURN(HIS_ICONSORTEDNORMAL);
	IF_RETURN(HIS_ICONSORTEDHOT);
	IF_RETURN(HIS_ICONSORTEDPRESSED);

	IF_RETURN(HILS_NORMAL);
	IF_RETURN(HILS_HOT);
	IF_RETURN(HILS_PRESSED);

	IF_RETURN(HIRS_NORMAL);
	IF_RETURN(HIRS_HOT);
	IF_RETURN(HIRS_PRESSED);

	IF_RETURN(HSAS_SORTEDUP);
	IF_RETURN(HSAS_SORTEDDOWN);

	IF_RETURN(HDDS_NORMAL);
	IF_RETURN(HDDS_SOFTHOT);
	IF_RETURN(HDDS_HOT);

	IF_RETURN(HDDFS_NORMAL);
	IF_RETURN(HDDFS_SOFTHOT);
	IF_RETURN(HDDFS_HOT);

	IF_RETURN(HOFS_NORMAL);
	IF_RETURN(HOFS_HOT);

	// ListView

	IF_RETURN(LISS_NORMAL);
	IF_RETURN(LISS_HOT);
	IF_RETURN(LISS_SELECTED);
	IF_RETURN(LISS_DISABLED);
	IF_RETURN(LISS_SELECTEDNOTFOCUS);
	IF_RETURN(LISS_HOTSELECTED);

	IF_RETURN(LVGH_OPEN);
	IF_RETURN(LVGH_OPENHOT);
	IF_RETURN(LVGH_OPENSELECTED);
	IF_RETURN(LVGH_OPENSELECTEDHOT);
	IF_RETURN(LVGH_OPENSELECTEDNOTFOCUSED);
	IF_RETURN(LVGH_OPENSELECTEDNOTFOCUSEDHOT);
	IF_RETURN(LVGH_OPENMIXEDSELECTION);
	IF_RETURN(LVGH_OPENMIXEDSELECTIONHOT);
	IF_RETURN(LVGH_CLOSE);
	IF_RETURN(LVGH_CLOSEHOT);
	IF_RETURN(LVGH_CLOSESELECTED);
	IF_RETURN(LVGH_CLOSESELECTEDHOT);
	IF_RETURN(LVGH_CLOSESELECTEDNOTFOCUSED);
	IF_RETURN(LVGH_CLOSESELECTEDNOTFOCUSEDHOT);
	IF_RETURN(LVGH_CLOSEMIXEDSELECTION);
	IF_RETURN(LVGH_CLOSEMIXEDSELECTIONHOT);

	IF_RETURN(LVGHL_OPEN);
	IF_RETURN(LVGHL_OPENHOT);
	IF_RETURN(LVGHL_OPENSELECTED);
	IF_RETURN(LVGHL_OPENSELECTEDHOT);
	IF_RETURN(LVGHL_OPENSELECTEDNOTFOCUSED);
	IF_RETURN(LVGHL_OPENSELECTEDNOTFOCUSEDHOT);
	IF_RETURN(LVGHL_OPENMIXEDSELECTION);
	IF_RETURN(LVGHL_OPENMIXEDSELECTIONHOT);
	IF_RETURN(LVGHL_CLOSE);
	IF_RETURN(LVGHL_CLOSEHOT);
	IF_RETURN(LVGHL_CLOSESELECTED);
	IF_RETURN(LVGHL_CLOSESELECTEDHOT);
	IF_RETURN(LVGHL_CLOSESELECTEDNOTFOCUSED);
	IF_RETURN(LVGHL_CLOSESELECTEDNOTFOCUSEDHOT);
	IF_RETURN(LVGHL_CLOSEMIXEDSELECTION);
	IF_RETURN(LVGHL_CLOSEMIXEDSELECTIONHOT);

	IF_RETURN(LVEB_NORMAL);
	IF_RETURN(LVEB_HOVER);
	IF_RETURN(LVEB_PUSHED);

	IF_RETURN(LVCB_NORMAL);
	IF_RETURN(LVCB_HOVER);
	IF_RETURN(LVCB_PUSHED);

	// TreeView

	IF_RETURN(TREIS_NORMAL);
	IF_RETURN(TREIS_HOT);
	IF_RETURN(TREIS_SELECTED);
	IF_RETURN(TREIS_DISABLED);
	IF_RETURN(TREIS_SELECTEDNOTFOCUS);
	IF_RETURN(TREIS_HOTSELECTED);

	IF_RETURN(GLPS_CLOSED);
	IF_RETURN(GLPS_OPENED);

	IF_RETURN(HGLPS_CLOSED);
	IF_RETURN(HGLPS_OPENED);

	// ToolBar

	IF_RETURN(TS_NORMAL);
	IF_RETURN(TS_HOT);
	IF_RETURN(TS_PRESSED);
	IF_RETURN(TS_DISABLED);
	IF_RETURN(TS_CHECKED);
	IF_RETURN(TS_HOTCHECKED);
	IF_RETURN(TS_NEARHOT);
	IF_RETURN(TS_OTHERSIDEHOT);

	return wcstol(id, 0, 0);
}

#undef IF_RETURN

StatePtr Skin::getState(HTHEME theme, int partId, int stateId)
{
	VSClassPtr vsclass = getVSClass(theme);
	if (!vsclass) return 0;

	PartPtr part = vsclass->getPart(partId);
	if (!part) return 0;

	return part->getState(stateId);
}

void Skin::getStateAttributes(HTHEME theme, int partId, int stateId, StateAttributes* stateAttributes)
{
	StatePtr state = getState(theme, partId, stateId);

	if (state)
	{
		stateAttributes->m_fillColor = state->m_fillColor;
		stateAttributes->m_textForeColor = state->m_textForeColor;
		stateAttributes->m_textBackColor = state->m_textBackColor;
	}
}

BOOL Skin::drawBackground(HDC dc, HTHEME theme, int partId, int stateId, LPRECT rc)
{
	StatePtr state = getState(theme, partId, stateId);
	if (!state) return FALSE;

	BOOL retValue = FALSE;
	for (auto figure : state->m_figures)
		figure->draw(dc, rc), retValue = TRUE;
	return retValue;
}

BOOL Skin::drawIcon(HDC dc, HTHEME theme, int partId, int stateId, LPRECT rc)
{
	StatePtr state = getState(theme, partId, stateId);
	if (!state) return FALSE;

	BOOL retValue = FALSE;
	for (auto iconFigure : state->m_iconFigures)
		iconFigure->draw(dc, rc), retValue = TRUE;
	return retValue;
}

BOOL Skin::drawText(HDC dc, HTHEME theme, int partId, int stateId, LPRECT rc, LPCWSTR text, int c, UINT format)
{
	StatePtr state = getState(theme, partId, stateId);
	if (!state) return FALSE;

	BOOL retValue = FALSE;
	for (auto textFigure : state->m_textFigures)
		textFigure->drawText(dc, rc, text, c, format), retValue = TRUE;
	return retValue;
}

BOOL Skin::textOut(HDC dc, HTHEME theme, int partId, int stateId, int x, int y, UINT options, LPRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	StatePtr state = getState(theme, partId, stateId);
	if (!state) return FALSE;

	BOOL retValue = FALSE;
	for (auto textFigure : state->m_textFigures)
		textFigure->textOut(dc, x, y, options, rc, text, c, dx), retValue = TRUE;
	return retValue;
}

BOOL Skin::onDrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc)
{
	RECT rc2 = *rc;
	BOOL result = FALSE;
	if (drawBackground(dc, theme, partId, stateId, &rc2)) result = TRUE;
	if (drawIcon(dc, theme, partId, stateId, &rc2)) result = TRUE;
	return result;
}

BOOL Skin::onDrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPCRECT rc)
{
	RECT rc2 = *rc;
	return drawText(dc, theme, partId, stateId, &rc2, text, c, textFlags);
}

BOOL Skin::onExtTextOut(HTHEME theme, HDC dc, int partId, int stateId, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx)
{
	RECT rc2 = {};
	if (rc) rc2 = *rc;
	return textOut(dc, theme, partId, stateId, x, y, options, &rc2, text, c, dx);
}

HTHEME Skin::getTheme(THEMES theme)
{
	return m_themes[theme];
}

int Skin::getCtlColorPartId(UINT message)
{
	switch (message)
	{
	case WM_CTLCOLORDLG: return CTLCOLORDLG;
	case WM_CTLCOLORMSGBOX: return CTLCOLORMSGBOX;
	case WM_CTLCOLORBTN: return CTLCOLORBTN;
	case WM_CTLCOLORSTATIC: return CTLCOLORSTATIC;
	case WM_CTLCOLOREDIT: return CTLCOLOREDIT;
	case WM_CTLCOLORLISTBOX: return CTLCOLORLISTBOX;
	case WM_CTLCOLORSCROLLBAR: return CTLCOLORSCROLLBAR;
	}

	return 0;
}

void Skin::setDwm(HWND hwnd, BOOL active)
{
//	MY_TRACE(_T("Skin::setDwm(0x%08X, %d)\n"), hwnd, active);

	enum MY_DWMWINDOWATTRIBUTE
	{
		DWMWA_USE_IMMERSIVE_DARK_MODE = 20,
		DWMWA_WINDOW_CORNER_PREFERENCE = 33,
		DWMWA_BORDER_COLOR = 34,
		DWMWA_CAPTION_COLOR = 35,
		DWMWA_TEXT_COLOR = 36,
		DWMWA_VISIBLE_FRAME_BORDER_THICKNESS,
		DWMWA_LAST
	};

	if (active)
	{
		if (m_dwm.m_activeBorderColor != -1)
		{
			HRESULT hr = ::DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR , &m_dwm.m_activeBorderColor, sizeof(m_dwm.m_activeBorderColor));
			MY_TRACE_COM_ERROR(hr);
		}

		if (m_dwm.m_activeCaptionColor != -1)
		{
			HRESULT hr = ::DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR , &m_dwm.m_activeCaptionColor, sizeof(m_dwm.m_activeCaptionColor));
			MY_TRACE_COM_ERROR(hr);
		}

		if (m_dwm.m_activeTextColor != -1)
		{
			HRESULT hr = ::DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR , &m_dwm.m_activeTextColor, sizeof(m_dwm.m_activeTextColor));
			MY_TRACE_COM_ERROR(hr);
		}
	}
	else
	{
		if (m_dwm.m_inactiveBorderColor != -1)
		{
			HRESULT hr = ::DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR , &m_dwm.m_inactiveBorderColor, sizeof(m_dwm.m_inactiveBorderColor));
			MY_TRACE_COM_ERROR(hr);
		}

		if (m_dwm.m_inactiveCaptionColor != -1)
		{
			HRESULT hr = ::DwmSetWindowAttribute(hwnd, DWMWA_CAPTION_COLOR , &m_dwm.m_inactiveCaptionColor, sizeof(m_dwm.m_inactiveCaptionColor));
			MY_TRACE_COM_ERROR(hr);
		}

		if (m_dwm.m_inactiveTextColor != -1)
		{
			HRESULT hr = ::DwmSetWindowAttribute(hwnd, DWMWA_TEXT_COLOR , &m_dwm.m_inactiveTextColor, sizeof(m_dwm.m_inactiveTextColor));
			MY_TRACE_COM_ERROR(hr);
		}
	}

	if (m_dwm.m_darkMode != -1)
	{
		HRESULT hr = ::DwmSetWindowAttribute(hwnd, DWMWA_USE_IMMERSIVE_DARK_MODE , &m_dwm.m_darkMode, sizeof(m_dwm.m_darkMode));
		MY_TRACE_COM_ERROR(hr);
	}

	if (m_dwm.m_cornerMode != -1)
	{
		HRESULT hr = ::DwmSetWindowAttribute(hwnd, DWMWA_WINDOW_CORNER_PREFERENCE , &m_dwm.m_cornerMode, sizeof(m_dwm.m_cornerMode));
		MY_TRACE_COM_ERROR(hr);
	}
}

HICON Skin::editIcon(HICON originalIcon)
{
//	MY_TRACE(_T("Skin::editIcon(0x%08X)\n"), originalIcon);

	ICONINFO ii = {};
	::GetIconInfo(originalIcon, &ii);

	BITMAP bm = {};
	::GetObject(ii.hbmColor, sizeof(bm), &bm);
	int w = bm.bmWidth;
	int h = bm.bmHeight;

	HDC colorDC = ::CreateCompatibleDC(0);
	HBITMAP oldColorBitmap = (HBITMAP)::SelectObject(colorDC, ii.hbmColor);
	HDC maskDC = ::CreateCompatibleDC(0);
	HBITMAP oldMaskBitmap = (HBITMAP)::SelectObject(maskDC, ii.hbmMask);
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++)
		{
			if (::GetPixel(maskDC, x, y))
				continue;

			COLORREF color = ::GetPixel(colorDC, x, y);

			for (auto iconColor : m_iconColorArray)
			{
				if (color == iconColor.m_src)
					::SetPixel(colorDC, x, y, iconColor.m_dst);
			}
		}
	}
	::SelectObject(maskDC, oldMaskBitmap);
	::DeleteDC(maskDC);
	::SelectObject(colorDC, oldColorBitmap);
	::DeleteDC(colorDC);

	HICON icon = ::CreateIconIndirect(&ii);
	::DeleteObject(ii.hbmColor);
	::DeleteObject(ii.hbmMask);

	return icon;
}

//--------------------------------------------------------------------
} // namespace Dark
//--------------------------------------------------------------------
