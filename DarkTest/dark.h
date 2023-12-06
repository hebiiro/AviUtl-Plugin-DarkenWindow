#pragma once

namespace Dark
{
	struct ColorSet
	{
		COLORREF fillColor;
		COLORREF edgeColor;
		COLORREF textBkColor;
		COLORREF textForeColor;
		COLORREF textBackColor;

		struct Type
		{
			static const int fillColor = 0;
			static const int edgeColor = 1;
			static const int textBkColor = 2;
			static const int textForeColor = 3;
			static const int textBackColor = 4;
		};
	};

	struct Api
	{
		UINT (WINAPI* dark_getCurrentId)() = 0;
		ColorSet* (WINAPI* dark_getNamedColor)(LPCWSTR name) = 0;

		//
		// ダークモードプラグインのエクスポート関数を取得します。
		// 最初に1回だけ呼び出してください。
		//
		BOOL init()
		{
			// Dark.auaを取得します。
			HMODULE dark = ::GetModuleHandleA("Dark.aua");

			// Dark.auaが取得できなかった場合はDarkenWindow.aulを取得します。
			if (!dark) dark = ::GetModuleHandleA("DarkenWindow.aul");

			// どちらも取得できなかった場合は失敗します。
			if (!dark) return FALSE;

			// dark_getCurrentIdを取得します。
			dark_getCurrentId = (decltype(dark_getCurrentId))::GetProcAddress(dark, "dark_getCurrentId");
			if (!dark_getCurrentId) return FALSE; // エクスポート関数を取得できなかった場合は失敗します。

			// dark_getNamedColorを取得します。
			dark_getNamedColor = (decltype(dark_getNamedColor))::GetProcAddress(dark, "dark_getNamedColor");
			if (!dark_getNamedColor) return FALSE; // エクスポート関数を取得できなかった場合は失敗します。

			return TRUE;
		}

		//
		// ダークモードプラグインが有効ならTRUEを返します。
		//
		BOOL isEnabled()
		{
			return !!dark_getNamedColor;
		}

		//
		// 現在のスキンのIDを返します。
		//
		UINT getCurrentId()
		{
			if (!dark_getCurrentId) return 0;

			return dark_getCurrentId();
		}

		//
		// 指定された名前のカラーセットを返します。
		// 指定された名前のカラーセットが存在しない場合は0を返します。
		//
		ColorSet* getNamedColor(LPCWSTR name)
		{
			if (!dark_getNamedColor) return 0;

			return dark_getNamedColor(name);
		}
	};
}
