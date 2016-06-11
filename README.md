# Assimp Loader Sample with Cinder 0.9.0
Assimpで読み込んだモデルデータ(アニメーション含む)をCinder 0.9.0で表示するサンプルです。

スキンメッシュの計算はシェーダーでおこなっています。

[参考資料](https://gist.github.com/kumar8600/7615693)

自分の学習向けなので内容は雑です。

以下のサンプルを含んでいます。

+ Assimpでモデルデータを読み込む
+ メッシュデータ
+ 階層アニメーション
+ スケルタルアニメーション
+ ダイアログによる簡易プレビューワ設定変更
+ iOS対応(OpenGL ES 3.0)


## How to build
Cinderはもとより、Assimpのインクルードファイルとライブラリファイルをプロジェクトから参照できるようにしておいてください。

あと、テストデータは自前で用意してください。

## Attention
+ Windows環境でテクスチャのファイル名に２バイト文字が含まれている場合、Assimpの当該インポーターのパス変換処理に手を入れる必要があります。
+ ダイアログ(cinder::params)の実装にVisualStudio2010向けのワークアラウンドが含まれています。これを外してCinderライブラリを再ビルドしてください。
+ iOSはOpenGL ES3.0で動いています。プリプロセッサにCINDER_GL_ES_3を追加してCinderを再ビルドしてください。


## Liense
License All source code files are licensed under the MPLv2.0 license

[MPLv2.0](https://www.mozilla.org/MPL/2.0/)