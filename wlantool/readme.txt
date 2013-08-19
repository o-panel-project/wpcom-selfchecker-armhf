wlantool for j-panel readme.txt 2012/2/10 vacs

・ファイルについて
　以下のファイル及びディレクトリが含まれる。
　- build.sh           ビルド実行スクリプト
　- ReadWLAN.c         ソースコード
　- ReadWLAN.h         ソースコード
　- WLANtool.c         ソースコード
　- WLANtool.h         ソースコード
　- sokuteicfg         設定ファイル（selfcheck用）
　- sokuteicfg.menuapp 設定ファイル（メニューアプリ用）
　- wlantool           実行ファイル
　- netkit-base        pingソースコード
　- ping               ping実行ファイル
　
・ビルド方法
　make を実行する
　
　# make
　
・起動方法
　wlantoolとsokuteicfgを同じディレクトリに配置する。
　pingはsokuteicfgに設定したディレクトリに配置する。
　次のようにwlantoolを起動する。
　
　# export DISPLAY=:0
　# ./wlantool

・pingについて
　rootfsに含まれているpingは必要とする機能が不足するため、
　別途ビルドしたもの添付した。
　ソースコードはFR-Vで使用していたnetkit-baseを使用。
　ビルドはwlantoolをビルドすると行われ、makeを実行したディレクトリに
　作成される。
　
・設定ファイルsokuteicfgについて
　各行で設定項目が決定されている。
　各行の設定項目は次の通り。
　
　　１行目： IP アドレス
　　２行目： result.dat
　　３行目： /mnt1/wlantoolapp//tmp/testfile1.csv
　　４行目： /mnt1/wlantoolapp//tmp/testfile2.csv
　　５行目： /mnt1/wlantoolapp//tmp/testfile3.csv
　　６行目： /mnt1/wlantoolapp//tmp/tempfile.dat
　　７行目： /mnt1/wlantoolapp//tmp/ftptemp.dat
　　８行目： FTPサーバ上のファイル名（ex:/dummy.dat）
　　９行目： iwconfig コマンド(FR-V版のwl assoc コマンド)
　１０行目： 未使用
　１１行目： iwconfig [i/f]（FR-V版のwl rssi コマンド）
　１２行目： ifconfig [i/f]（FR-V版のwl pktcnt コマンド）
　１３行目： iwlist [i/f] channel
　１４行目： iwlist [i/f] scanning（FR-V版のwl scanresults コマンド）
　１５行目： ifconfig [i/f] コマンド
　１６行目： wget コマンド
　１７行目： wget のオプション
　１８行目： pingコマンド１
　１９行目： pingコマンド２
　２０行目： ifupコマンド
　２１行目： ifdownコマンド
　２２行目： 「終了」ボタンを"exit_button_on"で表示、それ以外か空行で非表示

・更新履歴
　2012/2/10 v1.1
　　ネットワークディレイとダウンストリームのインジケータオフ
　　ネットワークディレイタイムアウト時の表示を最大値のみ10000
　　RSSI取得時、失敗したときのsegmet falutを修正
　　（file_analysis()関数内case:FOR_RSSIの辺り）
　　電界強度13chの表示の間違いを修正
　2012/2/9
　　スループットが800以下のときグラフが赤になるよう修正
　　ネットワークディレイタイムアウト時の表示は0ではなく10000に修正
　　ネットワークディレイの最低値が大きな値になるのを修正
　2012/2/3 v1.0
　　エラーレート閾値を0-2-5-100から0-20-50-100に修正
　　スループット計算方法修正
　　ダウンストリームテストのエラー修正
　2012/2/2
　　ネットワークディレイテストの平均値計算方法変更
　　sokuteicfgの終了ボタンON設定を1からexit_button_onに修正
　　ネットワークディレイテストとダウンロードストリームテストに
　　ステータス表示追加
　2012/ 1/18
　　ifup/ifdown追加
　　「測定結果ファイル～」メッセージの表示ずれ修正
　　ビルド方法をmakeに変更
　2011/ 8/20
　　文字表示についてpango(xft)に対応
　2011/ 7/11
　　暫定リリース

以上

