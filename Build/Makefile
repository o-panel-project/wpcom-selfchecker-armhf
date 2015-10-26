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
	make -C ../mainpanel
	@cp -p ../mainpanel/mainpanel bin/
	@cp -p ../scripts/appstart bin/
	@cp -p ../scripts/runsc bin/
	@cp -p ../ntpdate/ntpdate bin/
	@cp -p ../mainpanel/newsc_movie.sh bin/

felicalib:
	make -C ../felica-sdk/arch/linux/build -f Makefile.basic_rcs956_usb_all
	make -C ../nfc-extension/arch/linux/build -f Makefile.nfc_rcs956_all
	@cp -p ../felica-sdk/arch/linux/build/*.a ../libs/
	@cp -p ../nfc-extension/arch/linux/build/*.a ../libs/

tp-grid:
	make -C ../tp-grid
	@cp -p ../tp-grid/tp-grid bin/

cpprogress:
	make -C ../cpprogress
	@cp -p ../cpprogress/cpprogress bin/

log-battery:
	make -C ../log-battery
	@cp -p ../log-battery/log-battery bin/

xlib_lcdcheck:
	make -C ../lcd
	@cp -p ../lcd/xlib_lcdcheck bin/

demoapp:
	make -C ../prismapp/demoapp
	@cp -p ../prismapp/demoapp/demoapp-arm bin/
	@cp -p ../prismapp/newsc_demoapp.sh bin/

wlantool:
	make -C ../wlantool
	@cp -p ../wlantool/wlantool bin/
	@cp -p ../wlantool/newsc_wlantool.sh bin/

clean:
	make -C ../lcd clean
	make -C ../prismapp/demoapp clean
	make -C ../mainpanel clean
	make -C ../tp-grid clean
	make -C ../cpprogress clean
	make -C ../log-battery clean
	@rm -rf bin
#	@rm -f $(TARGET) demoapp-arm runsc appstart newsc_demoapp.sh newsc_wlantool.sh

svnclean :
	make clean
	find -name '.svn' | xargs rm -rf
