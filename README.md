# AviUtl プラグイン - 黒窓

AviUtl の見た目をダークモードにします。
[最新バージョンをダウンロード](../../releases/latest/)

![黒窓 2 0 1](https://user-images.githubusercontent.com/96464759/154665614-90c6a2fb-d9d3-42b5-bcd0-292497c8b5d9.png)

## 導入方法

1. 以下のファイルとフォルダを AviUtl の Plugins フォルダに配置します。
	* DarkenWindow.aul
	* DarkenWindow (フォルダ)
2. aviutl_dark.exe を作成します。手順は [Wiki](https://github.com/hebiiro/AviUtl-Plugin-DarkenWindow/wiki/aviutl_dark.exe-%E3%81%AE%E4%BD%9C%E6%88%90%E6%96%B9%E6%B3%95) にまとめてあります。

## 使用方法

1. aviutl_dark.exe を起動します。

### スキンセレクタ

1. メニューバーを右クリックします。
2. ポップアップメニューが出くるのでその中からスキンを選択します。
* 「影を付ける」のチェックを外すとテキストの影が強制的にオフになります。
* 「丸くする」のチェックを外すと丸角矩形の描画が強制的にオフになります。

## 設定方法

DarkenWindow フォルダ内の xml ファイルをテキストエディタで編集します。AviUtl 起動中でも編集が反映されます。

## 更新履歴

* 6.5.0 - 2022/06/19 現在フレームの縦線を変更できるように修正
* 6.4.0 - 2022/06/15 色の設定コモンダイアログが正常に描画されなかった問題を修正
* 6.3.0 - 2022/06/10 タブコントロールに対応
* 6.2.2 - 2022/06/06 patch.aul 確認用メッセージボックスを修正
* 6.2.1 - 2022/06/04 patch.aul 確認用メッセージボックスを修正
* 6.2.0 - 2022/06/03 スキンファイルのリファクタリング
* 6.1.3 - 2022/05/30 「スクリプト並び替えプラグイン」からのドラッグアンドドロップが正しく処理されない問題を修正
* 6.1.2 - 2022/05/30 影付き文字だとエディットボックスが正しく描画されなかった問題を修正
* 6.1.1 - 2022/05/30 影付き文字だとリストビューが正しく描画されなかった問題を修正
* 6.1.0 - 2022/05/29 スキンセレクタを実装
* 6.0.0 - 2022/05/24 アイコンの色を動的に変えられるように修正
* 5.5.0 - 2022/05/24 CreateDark.exe を追加
* 5.4.2 - 2022/05/23 無効状態のエディットボックスが正しく描画されなかった問題を修正
* 5.4.1 - 2022/05/19 「設定ダイアログ画面サイズ固定化プラグイン」で描画が乱れる問題を修正
* 5.4.0 - 2022/05/19 「White」スキンを追加
* 5.3.0 - 2022/05/16 「Dark」スキンを追加
* 5.2.0 - 2022/05/14 「ごちゃまぜドロップス」でエラーが出る問題を修正
* 5.1.0 - 2022/05/13 外部プロセスでもある程度動作するように修正
* 5.0.0 - 2022/05/13 「RoundDark」スキンを実装
* 4.2.0 - 2022/05/12 アイコンの色を指定できるように修正
* 4.1.1 - 2022/05/08 リストボックスが正常に描画されなかった問題を修正
* 4.1.0 - 2022/05/05 タイトルバーの色などに対応 (Win11 のみ)
* 4.0.0 - 2022/05/04 リファクタリング
* 3.2.1 - 2022/04/14 拡張編集ウィンドウの文字列描画処理を修正
* 3.2.0 - 2022/04/13 メッセージボックスの背景色に対応
* 3.1.0 - 2022/03/26 レイヤーの線の色などを変更できるように修正
* 3.0.0 - 2022/03/24 スキンを変更できるように修正
* 2.0.4 - 2022/02/26 Win11 でも動作するように修正
* 2.0.3 - 2022/02/21 トラックバーとスピンボタンのチラツキを軽減
* 2.0.2 - 2022/02/20 「ごちゃまぜドロップス」でエラーが出る問題を修正
* 2.0.1 - 2022/02/18 「設定ダイアログ画面サイズ固定化プラグイン」に対応
* 2.0.0 - 2022/02/14 タイムラインのグループ制御などの色に対応
* 1.1.4 - 2022/02/13 フィルタを追加したときの描画の乱れを修正
* 1.1.3 - 2022/02/12 タイムラインの目盛りの色に対応
* 1.1.2 - 2022/02/01 ファイル選択ダイアログのリストビューに対応
* 1.1.1 - 2022/01/30 「eclipse_fast」と競合していた問題などを修正
* 1.1.0 - 2022/01/28 リストビューとツリービューに対応
* 1.0.0 - 2022/01/23 初版

## 動作確認

* (必須) AviUtl 1.10 & 拡張編集 0.92 http://spring-fragrance.mints.ne.jp/aviutl/
* (必須) patch.aul r18 以降 https://scrapbox.io/ePi5131/patch.aul

## クレジット

* Microsoft Research Detours Package https://github.com/microsoft/Detours
* aviutl_exedit_sdk https://github.com/ePi5131/aviutl_exedit_sdk
* Common Library https://github.com/hebiiro/Common-Library

## 作成者情報
 
* 作成者 - 蛇色 (へびいろ)
* GitHub - https://github.com/hebiiro
* Twitter - https://twitter.com/io_hebiiro

## 免責事項

このプラグインおよび同梱物を使用したことによって生じたすべての障害・損害・不具合等に関しては、私と私の関係者および私の所属するいかなる団体・組織とも、一切の責任を負いません。各自の責任においてご使用ください。
