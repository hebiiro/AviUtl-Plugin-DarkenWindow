#include "pch.h"
#include "dark.h"

Dark::Api dark;
constexpr  UINT timer_id = 1000;
UINT prev_id = 0;

// 背景の塗りつぶしとテキストの描画を行います。
void draw_text(HDC dc, LPCRECT rc, LPCTSTR text, COLORREF fill_color, COLORREF text_color)
{
	HBRUSH brush = ::CreateSolidBrush(fill_color);
	::FillRect(dc, rc, brush);
	::DeleteObject(brush);

	::SetTextColor(dc, text_color);
	::SetBkMode(dc, TRANSPARENT);
	::DrawText(dc, text, ::lstrlen(text), (LPRECT)rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
}

BOOL func_WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, AviUtl::EditHandle* editp, AviUtl::FilterPlugin* fp)
{
	switch (message)
	{
	case AviUtl::FilterPlugin::WindowMessage::Init:
		{
			// ダークモードのAPIを取得します。
			dark.init();

			::SetTimer(hwnd, timer_id, 1000, 0);

			break;
		}
	case WM_PAINT:
		{
			HDC dc = ::GetDC(hwnd);
			RECT rc; ::GetClientRect(hwnd, &rc);

			// ダークモードのカラーセットを取得します。
			Dark::ColorSet* c2 = dark.getNamedColor(L"c2_normal");

			if (c2)
			{
				draw_text(dc, &rc,
					TEXT("ダークモードで描画します"),
					c2->fillColor, c2->textForeColor);
			}
			else
			{
				draw_text(dc, &rc,
					TEXT("ダークモードが無効です"),
					::GetSysColor(COLOR_BTNFACE), ::GetSysColor(COLOR_BTNTEXT));
			}

			::ReleaseDC(hwnd, dc);

			break;
		}
	case WM_TIMER:
		{
			if (wParam == timer_id)
			{
				// ダークモードのスキンが更新されたか確認します。
				UINT current_id = dark.getCurrentId();

				if (prev_id != current_id)
				{
					prev_id = current_id;

					::InvalidateRect(hwnd, 0, FALSE);
				}
			}

			break;
		}
	}

	return FALSE;
}

EXTERN_C AviUtl::FilterPluginDLL* CALLBACK GetFilterTable()
{
	LPCSTR name = "ダークモードのテスト";
	LPCSTR information = "ダークモードのテスト 1.0.0 by 蛇色";

	static AviUtl::FilterPluginDLL filter =
	{
		.flag =
//			AviUtl::FilterPlugin::Flag::NoConfig | // このフラグを指定するとウィンドウが作成されなくなってしまう。
			AviUtl::FilterPlugin::Flag::AlwaysActive | // このフラグがないと「フィルタ」に ON/OFF を切り替えるための項目が追加されてしまう。
			AviUtl::FilterPlugin::Flag::DispFilter | // このフラグがないと「設定」の方にウィンドウを表示するための項目が追加されてしまう。
			AviUtl::FilterPlugin::Flag::WindowThickFrame |
			AviUtl::FilterPlugin::Flag::WindowSize |
			AviUtl::FilterPlugin::Flag::ExInformation,
		.x = 400,
		.y = 200,
		.name = name,
		.func_WndProc = func_WndProc,
		.information = information,
	};

	return &filter;
}
