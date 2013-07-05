j-panel selfcheck用dhcpcのインストールについて 2011/8/26

１．ファイルについて
busybox-1.19.0.tarbz2
.config
udhcpc  selfcheck用dhcp

２．インストール方法
添付したバイナリを使用する場合は(5)から作業を行う。

(1) busyboxの取得
　ソースコードは次のように取得したものを使用する。
　# wget http://www.busybox.net/downloads/busybox-1.19.0.tar.bz2
　
(2) 展開
　次のように展開する。
　# tar xf busybox-1.19.0.tar.bz2
　
(3) .config
　用意した.configはbusyboxのソースコードを展開した
　ディレクトリへコピーしておく。
　.configを作成する場合は次のように行う。
　# cd busybox-1.19.0
　# make allnoconfig
　# make menunoconfig
　メニュー画面が出てきたら次の項目を有効にしてメニュー画面を終了する。
　　Networking Utilities  --->
　　　[*] udhcp client (udhcpc)
　
(4) ビルド
　次のようにビルドする。
　# make CROSS_COMPILE=arm-none-linux-gnueabi-
　# mv busybox udhcpc
　
(5) コピー
　USBメモリにudhcpcディレクトリを作成し、
　そこに作成したudhcpcをコピーする。
　
(6) dhcpスクリプトの修正
　/mnt/usb/script/wifi-set-infra-dhcpを直す
　次のような行がある。
　udhcpc -i $SC_WLAN_IFACE &
　これを次のように変更する。
　/mnt1/udhcpc/udhcpc -i $SC_WLAN_IFACE &
　
以上。
