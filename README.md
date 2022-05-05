# AviUtl プラグイン - 黒窓

* 4.1.0 by 蛇色 - 2022/05/05 タイトルバーの色などに対応 (Win11 のみ)
* 4.0.0 by 蛇色 - 2022/05/04 リファクタリング
* 3.2.1 by 蛇色 - 2022/04/14 拡張編集ウィンドウの文字列描画処理を修正
* 3.2.0 by 蛇色 - 2022/04/13 メッセージボックスの背景色に対応
* 3.1.0 by 蛇色 - 2022/03/26 レイヤーの線の色などを変更できるように修正
* 3.0.0 by 蛇色 - 2022/03/24 スキンを変更できるように修正
* 2.0.4 by 蛇色 - 2022/02/26 Win11 でも動作するように修正
* 2.0.3 by 蛇色 - 2022/02/21 トラックバーとスピンボタンのチラツキを軽減
* 2.0.2 by 蛇色 - 2022/02/20 ごちゃまぜドロップスでエラーが出る問題を修正
* 2.0.1 by 蛇色 - 2022/02/18 設定ダイアログ画面サイズ固定化プラグインに対応
* 2.0.0 by 蛇色 - 2022/02/14 タイムラインのグループ制御などの色に対応
* 1.1.4 by 蛇色 - 2022/02/13 フィルタを追加したときの描画の乱れを修正
* 1.1.3 by 蛇色 - 2022/02/12 タイムラインの目盛りの色に対応
* 1.1.2 by 蛇色 - 2022/02/01 ファイル選択ダイアログのリストビューに対応
* 1.1.1 by 蛇色 - 2022/01/30 eclipse_fast と競合していた問題などを修正
* 1.1.0 by 蛇色 - 2022/01/28 リストビューとツリービューに対応
* 1.0.0 by 蛇色 - 2022/01/23 初版

AviUtl の見た目をダークモードにします。

![黒窓 2 0 1](https://user-images.githubusercontent.com/96464759/154665614-90c6a2fb-d9d3-42b5-bcd0-292497c8b5d9.png)

## 導入方法

1. 以下のファイルを AviUtl のフォルダに配置します。
	* aviutl_dark.exe.manifest
2. 以下のファイルとフォルダを AviUtl の Plugins フォルダに配置します。
	* DarkenWindow.aul
	* DarkenWindow (フォルダ)
3. aviutl.exe をリソースエディタ (RisohEditor など) で開きます。
4. マニフェスト (RT_MANIFEST のような名前の項目) を削除します。
5. ファイル名を aviutl_dark.exe にして保存します。

## 使い方

1. aviutl_dark.exe を起動します。

## 設定

DarkenWindow フォルダ内の xml ファイルをテキストエディタで編集します。AviUtl 起動中でも編集が反映されます。

## 動作確認

* (必須) AviUtl 1.10 & 拡張編集 0.92 http://spring-fragrance.mints.ne.jp/aviutl/
* (共存確認) patch.aul r21 https://scrapbox.io/ePi5131/patch.aul

## クレジット

* Microsoft Research Detours Package https://github.com/microsoft/Detours
* aviutl_exedit_sdk https://github.com/ePi5131/aviutl_exedit_sdk
* Common Library https://github.com/hebiiro/Common-Library
