#pragma once

//--------------------------------------------------------------------

enum MY_STATICPARTS {
	STAT_ETCHEDEDGE = 2,
};

//--------------------------------------------------------------------
namespace Dark {\
//--------------------------------------------------------------------

enum THEMES {
	THEME_MENU,
	THEME_SCROLLBAR,
	THEME_STATIC,
	THEME_BUTTON,
	THEME_EDIT,
	THEME_LISTBOX,
	THEME_COMBOBOX,
	THEME_TOOLTIP,
	THEME_TRACKBAR,
	THEME_SPIN,
	THEME_HEADER,
	THEME_LISTVIEW,
	THEME_TREEVIEW,
	THEME_TOOLBAR,
	THEME_WINDOW,
	THEME_DIALOG,
	THEME_CTLCOLOR,
	THEME_EXEDIT,
	THEME_MAXSIZE,
};

enum CUSTOM_PARTS {
	CTLCOLORDLG = 51,
	CTLCOLORMSGBOX,
	CTLCOLORBTN,
	CTLCOLORSTATIC,
	CTLCOLOREDIT,
	CTLCOLORLISTBOX,
	CTLCOLORSCROLLBAR,
	WINDOW_WINDOWFACE,
	WINDOW_DIALOGFACE,
	WINDOW_WINDOWEDGE,
	WINDOW_CLIENTEDGE,
	WINDOW_STATICEDGE,
	WINDOW_BORDER,
	WINDOW_RAISEDOUTEREDGE,
	WINDOW_SUNKENOUTEREDGE,
	WINDOW_RAISEDEDGE,
	WINDOW_SUNKENEDGE,
	WINDOW_BUMPEDGE,
	WINDOW_ETCHEDEDGE,
	EXEDIT_ROOT,
	EXEDIT_LAYER,
	EXEDIT_LONGGUAGE,
	EXEDIT_SHORTGUAGE,
	EXEDIT_LAYERBACKGROUND,
	EXEDIT_GROUPBACKGROUND,
	EXEDIT_SELECTION,
	EXEDIT_SELECTIONEDGE,
	EXEDIT_SELECTIONBK,
	EXEDIT_LAYERLEFT,
	EXEDIT_LAYERTOP,
	EXEDIT_LAYERRIGHT,
	EXEDIT_LAYERBOTTOM,
	EXEDIT_LAYERSEPARATOR,
};

enum CTLCOLOR_STATES {
	CTLCOLOR_NORMAL = 1,
	CTLCOLOR_DISABLED = 2,
	CTLCOLOR_READONLY = 3,
};

enum EXEDIT_LAYER_STATES {
	EXEDIT_LAYER_ACTIVE = 1,
	EXEDIT_LAYER_INACTIVE = 2,
};

enum EXEDIT_LAYERBACKGROUND_STATES {
	EXEDIT_LAYERBACKGROUND_ACTIVE = 1,
	EXEDIT_LAYERBACKGROUND_INACTIVE = 2,
};

enum EXEDIT_GROUPBACKGROUND_STATES {
	EXEDIT_GROUPBACKGROUND_ACTIVE_1 = 1,
	EXEDIT_GROUPBACKGROUND_ACTIVE_2 = 2,
	EXEDIT_GROUPBACKGROUND_ACTIVE_3 = 3,
	EXEDIT_GROUPBACKGROUND_INACTIVE_1 = 4,
	EXEDIT_GROUPBACKGROUND_INACTIVE_2 = 5,
	EXEDIT_GROUPBACKGROUND_INACTIVE_3 = 6,
};

//--------------------------------------------------------------------

class Figure
{
protected:

	_bstr_t m_name;
	int m_ix, m_iy;
	int m_ox, m_oy;
	int m_left, m_top, m_right, m_bottom;
	int m_addLeft, m_addTop, m_addRight, m_addBottom;

public:

	Figure();
	virtual ~Figure();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);

	LPCWSTR getName() const { return m_name; }
};

//--------------------------------------------------------------------

class Null : public Figure
{
public:

	Null();
	virtual ~Null();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class FillRect : public Figure
{
protected:

	COLORREF m_fillColor;

public:

	FillRect();
	virtual ~FillRect();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class FrameRect : public Figure
{
protected:

	COLORREF m_edgeColor;
	int m_edgeWidth;

public:

	FrameRect();
	virtual ~FrameRect();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawAlphaRectangle : public Figure
{
protected:

	COLORREF m_fillColor;
	COLORREF m_edgeColor;
	int m_edgeWidth;

public:

	DrawAlphaRectangle();
	virtual ~DrawAlphaRectangle();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawSingleRaisedEdge : public Figure
{
public:

	static COLORREF m_topLeftColor;
	static COLORREF m_bottomRightColor;

	DrawSingleRaisedEdge();
	virtual ~DrawSingleRaisedEdge();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawSingleSunkenEdge : public Figure
{
public:

	static COLORREF m_topLeftColor;
	static COLORREF m_bottomRightColor;

	DrawSingleSunkenEdge();
	virtual ~DrawSingleSunkenEdge();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawDoubleRaisedEdge : public Figure
{
public:

	static COLORREF m_innerTopLeftColor;
	static COLORREF m_innerBottomRightColor;
	static COLORREF m_outerTopLeftColor;
	static COLORREF m_outerBottomRightColor;

	DrawDoubleRaisedEdge();
	virtual ~DrawDoubleRaisedEdge();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawDoubleSunkenEdge : public Figure
{
public:

	static COLORREF m_innerTopLeftColor;
	static COLORREF m_innerBottomRightColor;
	static COLORREF m_outerTopLeftColor;
	static COLORREF m_outerBottomRightColor;

	DrawDoubleSunkenEdge();
	virtual ~DrawDoubleSunkenEdge();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawDoubleBumpEdge : public Figure
{
public:

	static COLORREF m_innerTopLeftColor;
	static COLORREF m_innerBottomRightColor;
	static COLORREF m_outerTopLeftColor;
	static COLORREF m_outerBottomRightColor;

	DrawDoubleBumpEdge();
	virtual ~DrawDoubleBumpEdge();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawDoubleEtchedEdge : public Figure
{
public:

	static COLORREF m_innerTopLeftColor;
	static COLORREF m_innerBottomRightColor;
	static COLORREF m_outerTopLeftColor;
	static COLORREF m_outerBottomRightColor;

	DrawDoubleEtchedEdge();
	virtual ~DrawDoubleEtchedEdge();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class TextFigure : public Figure
{
protected:

	_bstr_t m_fontName;
	int m_fontWeight;
	BOOL m_fontItalic;
	BOOL m_fontStroke;
	COLORREF m_fillColor;
	COLORREF m_foreColor;
	COLORREF m_backColor;
	UINT m_format;

public:

	TextFigure();
	virtual ~TextFigure();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void drawText(HDC dc, LPRECT rc, LPCWSTR text, int c, UINT format);
	virtual void textOut(HDC dc, int x, int y, UINT options, LPRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
};

//--------------------------------------------------------------------

class DrawText : public TextFigure
{
public:

	DrawText();
	virtual ~DrawText();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void drawText(HDC dc, LPRECT rc, LPCWSTR text, int c, UINT format);
	virtual void textOut(HDC dc, int x, int y, UINT options, LPRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
};

//--------------------------------------------------------------------

class DrawShadowText : public TextFigure
{
};

//--------------------------------------------------------------------

class IconFigure : public TextFigure
{
protected:

	_bstr_t m_text;

public:

	IconFigure();
	virtual ~IconFigure();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawIcon : public IconFigure
{
public:

	DrawIcon();
	virtual ~DrawIcon();
	virtual void load(const MSXML2::IXMLDOMElementPtr& element);
	virtual void draw(HDC dc, LPRECT rc);
};

//--------------------------------------------------------------------

class DrawShadowIcon : public IconFigure
{
};

//--------------------------------------------------------------------

typedef std::shared_ptr<Figure> FigurePtr;
typedef std::map<_bstr_t, FigurePtr> FigureMap;
typedef std::vector<FigurePtr> FigureArray;

typedef std::shared_ptr<IconFigure> IconFigurePtr;
typedef std::map<_bstr_t, IconFigurePtr> IconFigureMap;
typedef std::vector<IconFigurePtr> IconFigureArray;

typedef std::shared_ptr<TextFigure> TextFigurePtr;
typedef std::map<_bstr_t, TextFigurePtr> TextFigureMap;
typedef std::vector<TextFigurePtr> TextFigureArray;

//--------------------------------------------------------------------

class State
{
public:

	COLORREF m_fillColor;
	COLORREF m_textBkColor;
	COLORREF m_textForeColor;
	COLORREF m_textBackColor;
	HBRUSH m_fillBrush;

	FigureArray m_figures;
	IconFigureArray m_iconFigures;
	TextFigureArray m_textFigures;

	State();
	~State();

	HBRUSH getFillBrush();
	void deleteFillBrush();
};

typedef std::shared_ptr<State> StatePtr;
typedef std::vector<StatePtr> StateArray;

//--------------------------------------------------------------------

class Part
{
private:

	StateArray m_stateArray;

public:

	Part();
	StatePtr addState(int stateId);
	StatePtr getState(int stateId);
};

typedef std::shared_ptr<Part> PartPtr;
typedef std::vector<PartPtr> PartArray;

//--------------------------------------------------------------------

class VSClass
{
private:

	_bstr_t m_id;
	PartArray m_partArray;

public:

	VSClass();
	LPCWSTR getId() const;
	void setId(LPCWSTR id);
	PartPtr addPart(int partId);
	PartPtr getPart(int partId);
};

typedef std::shared_ptr<VSClass> VSClassPtr;
typedef std::map<HTHEME, VSClassPtr> VSClassMap;

//--------------------------------------------------------------------

struct StateAttributes
{
	COLORREF m_fillColor;
	COLORREF m_textForeColor;
	COLORREF m_textBackColor;
};

//--------------------------------------------------------------------

class Skin
{
private:

	HINSTANCE m_instance;
	HWND m_hwnd;
	FileUpdateCheckers m_fileUpdateCheckers;
	FigureMap m_figureMap;
	IconFigureMap m_iconFigureMap;
	TextFigureMap m_textFigureMap;
	VSClassMap m_vsclassMap;

	HTHEME m_themes[THEME_MAXSIZE];

public:

	Skin();
	~Skin();

	static void CALLBACK timerProc(HWND hwnd, UINT message, UINT_PTR timerId, DWORD time);
	static BOOL CALLBACK enumWindowsProc(HWND hwnd, LPARAM lParam);
	void init(HINSTANCE instance, HWND hwnd);
	BOOL reloadSettings(BOOL force);
	void reloadSettingsInternal(LPCWSTR fileName);
	void reloadExeditSettings();
	void reloadSkin(LPCWSTR fileName);
	void loadAttributes(const MSXML2::IXMLDOMElementPtr& parentElement);
	void loadFigures(const MSXML2::IXMLDOMElementPtr& parentElement);
	void loadVSClasses(const MSXML2::IXMLDOMElementPtr& parentElement);
	void loadVSClass(const MSXML2::IXMLDOMElementPtr& parentElement);
	void loadName(const MSXML2::IXMLDOMElementPtr& parentElement, VSClassPtr& vsclass);
	void loadPart(const MSXML2::IXMLDOMElementPtr& parentElement, const VSClassPtr& vsclass);
	void loadState(const MSXML2::IXMLDOMElementPtr& parentElement, const PartPtr& part);
	void loadFigure(const MSXML2::IXMLDOMElementPtr& parentElement, const StatePtr& state);
	void loadIconFigure(const MSXML2::IXMLDOMElementPtr& parentElement, const StatePtr& state);
	void loadTextFigure(const MSXML2::IXMLDOMElementPtr& parentElement, const StatePtr& state);

	FigurePtr getFigure(LPCWSTR name);
	IconFigurePtr getIconFigure(LPCWSTR name);
	TextFigurePtr getTextFigure(LPCWSTR name);

	VSClassPtr getVSClass(HTHEME theme);
	VSClassPtr getVSClass(HWND hwnd, LPCWSTR vsclass);
	int getPartId(LPCWSTR id);
	int getStateId(LPCWSTR id);

	template<class T>
	void loadFigure(const MSXML2::IXMLDOMElementPtr& parentElement, LPCWSTR tagName)
	{
		MY_TRACE(_T("Skin::loadFigures(%ws)\n"), tagName);

		MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->selectNodes(tagName);
		int c = nodeList->length;
		for (int i = 0; i < c; i++)
		{
			MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

			FigurePtr figure(new T());
			figure->load(element);
			m_figureMap[figure->getName()] = figure;
			MY_TRACE(_T("figure = %ws, %ws\n"), tagName, figure->getName());
		}
	}

	template<class T>
	void loadIconFigure(const MSXML2::IXMLDOMElementPtr& parentElement, LPCWSTR tagName)
	{
		MY_TRACE(_T("Skin::loadIconFigures(%ws)\n"), tagName);

		MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->selectNodes(tagName);
		int c = nodeList->length;
		for (int i = 0; i < c; i++)
		{
			MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

			IconFigurePtr figure(new T());
			figure->load(element);
			m_iconFigureMap[figure->getName()] = figure;
			MY_TRACE(_T("icon figure = %ws, %ws\n"), tagName, figure->getName());
		}
	}

	template<class T>
	void loadTextFigure(const MSXML2::IXMLDOMElementPtr& parentElement, LPCWSTR tagName)
	{
		MY_TRACE(_T("Skin::loadTextFigures(%ws)\n"), tagName);

		MSXML2::IXMLDOMNodeListPtr nodeList = parentElement->selectNodes(tagName);
		int c = nodeList->length;
		for (int i = 0; i < c; i++)
		{
			MSXML2::IXMLDOMElementPtr element = nodeList->item[i];

			TextFigurePtr figure(new T());
			figure->load(element);
			m_textFigureMap[figure->getName()] = figure;
			MY_TRACE(_T("text figure = %ws, %ws\n"), tagName, figure->getName());
		}
	}

	StatePtr getState(HTHEME theme, int partId, int stateId);
	void getStateAttributes(HTHEME theme, int partId, int stateId, StateAttributes* stateAttributes);
	BOOL drawBackground(HDC dc, HTHEME theme, int partId, int stateId, LPRECT rc);
	BOOL drawIcon(HDC dc, HTHEME theme, int partId, int stateId, LPRECT rc);
	BOOL drawText(HDC dc, HTHEME theme, int partId, int stateId, LPRECT rc, LPCWSTR text, int c, UINT format);
	BOOL textOut(HDC dc, HTHEME theme, int partId, int stateId, int x, int y, UINT options, LPRECT rc, LPCWSTR text, UINT c, CONST INT* dx);
	BOOL onDrawThemeBackground(HTHEME theme, HDC dc, int partId, int stateId, LPCRECT rc);
	BOOL onDrawThemeText(HTHEME theme, HDC dc, int partId, int stateId, LPCWSTR text, int c, DWORD textFlags, LPCRECT rc);
	BOOL onExtTextOut(HTHEME theme, HDC dc, int partId, int stateId, int x, int y, UINT options, LPCRECT rc, LPCWSTR text, UINT c, CONST INT* dx);

	HTHEME getTheme(THEMES theme);
	static int getCtlColorPartId(UINT message);
};

//--------------------------------------------------------------------
} // namespace Dark

//--------------------------------------------------------------------

extern Dark::Skin g_skin;

//--------------------------------------------------------------------
