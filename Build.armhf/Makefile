include rules.mk

export AR
export CC
export LD
export BOARD_TYPE

#TARGET=tp-grid mainpanel log-battery cpprogress
TARGET= mainpanel tp-grid log-battery cpprogress xlib_lcdcheck demoapp wlantool

.PHONY: $(TARGET)
all: $(TARGET)

mainpanel:
	-@mkdir bin >/dev/null 2>&1
	make -C ../mainpanel -f Makefile.armhf
	@cp -p ../mainpanel/mainpanel bin/
	@cp -p ../scripts/appstart bin/
	@cp -p ../scripts/runsc bin/
	@cp -p ../ntpdate/ntpdate.armhf bin/ntpdate
	@cp -p ../mainpanel/newsc_movie.sh bin/

felicalib:
	(cd ../felica-sdk/arch/linux/build && ./build.armhf.sh)
	(cd ../nfc-extension/arch/linux/build && ./build.armhf.sh)
#	make -C ../felica-sdk/arch/linux/build -f Makefile.basic_rcs956_usb_all
#	make -C ../nfc-extension/arch/linux/build -f Makefile.nfc_rcs956_all
	@cp -p ../felica-sdk/arch/linux/build/*.a ../libs.armhf/
	@cp -p ../nfc-extension/arch/linux/build/*.a ../libs.armhf/

tp-grid:
	make -C ../tp-grid -f Makefile.armhf
	@cp -p ../tp-grid/tp-grid bin/

cpprogress:
	make -C ../cpprogress -f Makefile.armhf
	@cp -p ../cpprogress/cpprogress bin/

log-battery:
	make -C ../log-battery -f Makefile.armhf
	@cp -p ../log-battery/log-battery bin/

xlib_lcdcheck:
	make -C ../lcd -f Makefile.armhf
	@cp -p ../lcd/xlib_lcdcheck bin/

demoapp:
	make -C ../prismapp/demoapp -f Makefile.armhf
	@cp -p ../prismapp/demoapp/demoapp-arm bin/
	@cp -p ../prismapp/newsc_demoapp.sh bin/

wlantool:
	make -C ../wlantool -f Makefile.armhf
	@cp -p ../wlantool/wlantool.armhf bin/wlantool
	@cp -p ../wlantool/newsc_wlantool.sh bin/

clean:
	make -C ../lcd clean -f Makefile.armhf
	make -C ../prismapp/demoapp clean -f Makefile.armhf
	make -C ../mainpanel clean -f Makefile.armhf
	make -C ../tp-grid clean -f Makefile.armhf
	make -C ../cpprogress clean -f Makefile.armhf
	make -C ../log-battery clean -f Makefile.armhf
	@rm -rf bin
#	@rm -f $(TARGET) demoapp-arm runsc appstart newsc_demoapp.sh newsc_wlantool.sh

felicaclean:
	(cd ../felica-sdk/arch/linux/build && ./build.armhf.sh clean)
	(cd ../nfc-extension/arch/linux/build && ./build.armhf.sh clean)

svnclean :
	make clean
	find -name '.svn' | xargs rm -rf