# この資料について
- ETロボコンに開発に役立つ情報を記載します
- 実現したいことから逆引きできる形で項目を追加していきます
- メモ程度で良いので、追加変更してもらえると嬉しいです

# Tips

## MDファイルの使い方が知りたい
- mdファイルとは、プレーンテキストで簡単にHTML形式の文章が作成できる記法
- ソフトウェア開発において、READMEの作成やwikiの作成などに使用される
- VSCodeでは、mdファイルを開いた状態で「Ctrl + Shift + V」でプレビュー表示できる
- 主に使用するのは、見出し「#」とリスト「-」
- インデント（タブ）を入れると、入れ子構造にすることができる
- [Markdown記法一覧](https://qiita.com/oreo/items/82183bfbaac69971917f)

## ビルド及びシミュレータを起動したい
- workspaceディレクトリにあるcurrentappの中身を起動したいプロジェクト名に変更する
- make currentapp sim up

## コピーしたプロジェクトの名前が編集できない場合
- コマンドラインからプロジェクトをコピーする
  - cd workspace
  - cp -r [コピー元ファイル名] [コピー先ファイル名]
  - cd ..
  - VSCodeエクスプローラーの更新ボタン

## 使用できるデータ型が知りたい
- [TOPPERS第3世代カーネル仕様書](https://toppers.jp/docs/tech/tgki_spec-342.pdf)
- 85ページ参照

## 実機で使用できるAPIが知りたい
[RasPikeAPI一覧](https://qiita.com/purakenji/private/bc6938cc9a4a8749aaf1)

## タスクを追加したい
- タスク格納用のフォルダを作成
- 他のタスクのソースファイルとヘッダファイルをコピー
- 関数名などにタスク名を反映し、不要なコードを削除
- ソースファイルとヘッダファイルの名称を追加するタスク名に変更
- app.cfgを編集してタスクを追加（次項で説明）
- app.hに必要な定義を追加（app.cfgで使用するもの）
- app.hにタスクのプロトタイプ宣言を追加
- タスクを起動するコードを任意の他のタスクに追加
- Makefile.incを編集してビルド対象にタスクを追加

## app.cfgの編集方法（タスクの追加方法）
- [TOPPERSのFAQ（アプリケーションの開発方法に関する質問）](https://dev.toppers.jp/trac_user/ev3pf/wiki/FAQ#%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3%E3%81%AE%E9%96%8B%E7%99%BA%E6%96%B9%E6%B3%95%E3%81%AB%E9%96%A2%E3%81%99%E3%82%8B%E8%B3%AA%E5%95%8F)

## タスクの制御方法
- リアルタイムOSとタスクスケジューリングについて
- [リアルタイムOSとμT-Kernel](https://www.tron.org/ja/page-722/rtos01_1/)
- μT-Kernelは関係なし
- ETロボコンではEV3RTと呼ばれるLEGO Mindstorms EV3向けのリアルタイムOSを使用している
- EV3RTはTOPPERSのリアルタイムOSの派生の一つ
- TOPPERSに準拠しているため、TOPPERSのAPIが使用できる
- [TOPPERS第3世代カーネル（ITRON系） 統合仕様書](https://www.toppers.jp/docs/tech/tgki_spec-321_richtext.pdf)
- 「第４章 カーネルAPI仕様」には、タスク制御に使用するAPIが記載されている
- ext_tsk()　自タスクを終了する
- sta_cyc(タスクID)　周期タスクを開始する
- stp_cyc(タスクID)　周期タスクを停止する
- dly_tsk(マイクロ秒)　タスクを指定時間停止する（停止時間が仕様通りにいかなかったので、ラッパー（wait_msec関数）を作成しました）
- 
## イベントフラグの使い方
- [TOPPERS/ASPのイベントフラグの使い方](https://qiita.com/Yukiya_Ishioka/items/58ea868dbc1594968938)


