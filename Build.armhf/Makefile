include rules.mk

ifeq ($(ARCH),armhf)
MAKEFILE = Makefile.$(ARCH)
else
MAKEFILE = Makefile
endif

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
	make -C ../mainpanel -f $(MAKEFILE)
	@cp -p ../mainpanel/mainpanel bin/
	@cp -p ../scripts/appstart bin/
	@cp -p ../scripts/runsc bin/
	@cp -p ../ntpdate/ntpdate.$(ARCH) bin/ntpdate
	@cp -p ../mainpanel/newsc_movie.sh bin/

felicalib:
	(cd ../felica-sdk/arch/linux/build && ./build.$(ARCH).sh)
	(cd ../nfc-extension/arch/linux/build && ./build.$(ARCH).sh)
	@cp -p ../felica-sdk/arch/linux/build/*.a ../libs.$(ARCH)/
	@cp -p ../nfc-extension/arch/linux/build/*.a ../libs.$(ARCH)/

tp-grid:
	make -C ../tp-grid -f $(MAKEFILE)
	@cp -p ../tp-grid/tp-grid bin/

cpprogress:
	make -C ../cpprogress -f $(MAKEFILE)
	@cp -p ../cpprogress/cpprogress bin/

log-battery:
	make -C ../log-battery -f $(MAKEFILE)
	@cp -p ../log-battery/log-battery bin/

xlib_lcdcheck:
	make -C ../lcd -f $(MAKEFILE)
	@cp -p ../lcd/xlib_lcdcheck bin/

demoapp:
	make -C ../prismapp/demoapp -f $(MAKEFILE)
	@cp -p ../prismapp/demoapp/demoapp-arm bin/
	@cp -p ../prismapp/newsc_demoapp.sh bin/

wlantool:
	make -C ../wlantool -f $(MAKEFILE)
	@cp -p ../wlantool/wlantool.$(ARCH) bin/wlantool
	@cp -p ../wlantool/newsc_wlantool.sh bin/

clean:
	make -C ../lcd clean -f $(MAKEFILE)
	make -C ../prismapp/demoapp clean -f $(MAKEFILE)
	make -C ../mainpanel clean -f $(MAKEFILE)
	make -C ../tp-grid clean -f $(MAKEFILE)
	make -C ../cpprogress clean -f $(MAKEFILE)
	make -C ../log-battery clean -f $(MAKEFILE)
	@rm -rf bin
#	@rm -f $(TARGET) demoapp-arm runsc appstart newsc_demoapp.sh newsc_wlantool.sh

felicaclean:
	(cd ../felica-sdk/arch/linux/build && ./build.$(ARCH).sh clean)
	(cd ../nfc-extension/arch/linux/build && ./build.$(ARCH).sh clean)

svnclean :
	make clean
	find -name '.svn' | xargs rm -rf
