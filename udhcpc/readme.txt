j-panel selfcheck�pdhcpc�̃C���X�g�[���ɂ��� 2011/8/26

�P�D�t�@�C���ɂ���
busybox-1.19.0.tarbz2
.config
udhcpc  selfcheck�pdhcp

�Q�D�C���X�g�[�����@
�Y�t�����o�C�i�����g�p����ꍇ��(5)�����Ƃ��s���B

(1) busybox�̎擾
�@�\�[�X�R�[�h�͎��̂悤�Ɏ擾�������̂��g�p����B
�@# wget http://www.busybox.net/downloads/busybox-1.19.0.tar.bz2
�@
(2) �W�J
�@���̂悤�ɓW�J����B
�@# tar xf busybox-1.19.0.tar.bz2
�@
(3) .config
�@�p�ӂ���.config��busybox�̃\�[�X�R�[�h��W�J����
�@�f�B���N�g���փR�s�[���Ă����B
�@.config���쐬����ꍇ�͎��̂悤�ɍs���B
�@# cd busybox-1.19.0
�@# make allnoconfig
�@# make menunoconfig
�@���j���[��ʂ��o�Ă����玟�̍��ڂ�L���ɂ��ă��j���[��ʂ��I������B
�@�@Networking Utilities  --->
�@�@�@[*] udhcp client (udhcpc)
�@
(4) �r���h
�@���̂悤�Ƀr���h����B
�@# make CROSS_COMPILE=arm-none-linux-gnueabi-
�@# mv busybox udhcpc
�@
(5) �R�s�[
�@USB��������udhcpc�f�B���N�g�����쐬���A
�@�����ɍ쐬����udhcpc���R�s�[����B
�@
(6) dhcp�X�N���v�g�̏C��
�@/mnt/usb/script/wifi-set-infra-dhcp�𒼂�
�@���̂悤�ȍs������B
�@udhcpc -i $SC_WLAN_IFACE &
�@��������̂悤�ɕύX����B
�@/mnt1/udhcpc/udhcpc -i $SC_WLAN_IFACE &
�@
�ȏ�B
