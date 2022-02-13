![黒窓 1 0 0](https://user-images.githubusercontent.com/96464759/150677221-3531b5ab-c607-4d96-b420-92ba121481a4.png)

# AviUtl プラグイン - 黒窓

* version 1.1.4 by 蛇色 - 2022/02/13 フィルタを追加したときの描画の乱れを修正
* version 1.1.3 by 蛇色 - 2022/02/12 タイムラインの目盛りの色に対応
* version 1.1.2 by 蛇色 - 2022/02/01 ファイル選択ダイアログのリストビューに対応
* version 1.1.1 by 蛇色 - 2022/01/30 eclipse_fast と競合していた問題などを修正
* version 1.1.0 by 蛇色 - 2022/01/28 リストビューとツリービューに対応
* version 1.0.0 by 蛇色 - 2022/01/23 初版

AviUtl の見た目をダークモードにします。

## 導入方法

1. 以下のファイルを AviUtl のフォルダに配置します。
	* aviutl_dark.exe.manifest
2. 以下のファイルを AviUtl の Plugins フォルダに配置します。
	* DarkenWindow.aul
3. aviutl.exe をリソースエディタ (RisohEditor など) で開きます。
4. マニフェスト (RT_MANIFEST のような名前の項目) を削除します。
5. ファイル名を aviutl_dark.exe にして保存します。

## 使い方

1. aviutl_dark.exe を起動します。

## 設定

今のところありません。

## 動作確認

* (必須) AviUtl 1.10 & 拡張編集 0.92 http://spring-fragrance.mints.ne.jp/aviutl/
* (共存確認) patch.aul r11 https://www.nicovideo.jp/watch/sm40027189
* (共存確認) eclipse_fast 1.00 https://www.nicovideo.jp/watch/sm39756003
