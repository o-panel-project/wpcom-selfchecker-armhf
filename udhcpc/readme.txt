j-panel selfcheck用dhcpcのインストールについて 2011/8/26

１．ファイルについて
busybox-1.19.0.tarbz2
dot.config
wpcom.patch
udhcpc  selfcheck用dhcp

２．インストール方法
添付したバイナリを使用する場合は(5)から作業を行う。

(1) busyboxの取得
　ソースコードは次のように取得したものを使用する。
　# wget http://www.busybox.net/downloads/busybox-1.19.0.tar.bz2

(2) 展開
　次のように展開する。
　# tar xf busybox-1.19.0.tar.bz2
  # cd busybox-1.19.0
  # patch -p1 < ../wpcom.patch

(3) .config
　用意したdot.configはbusyboxのソースコードを展開した
　ディレクトリへ.configとしてコピーしておく。
　.configを作成する場合は次のように行う。
　# cd busybox-1.19.0
　# make allnoconfig
　# make menunoconfig
　メニュー画面が出てきたら次の項目を有効にしてメニュー画面を終了する。
　　Networking Utilities  --->
　　　[*] udhcp client (udhcpc)

(4) ビルド
　次のようにビルドする。
　# cd busybox-1.19.0
　# make CROSS_COMPILE=arm-none-linux-gnueabi-
　# cp busybox ../udhcpc

(5) コピー
　USBメモリのbinディレクトリに作成したudhcpcをコピーする。

以上。
