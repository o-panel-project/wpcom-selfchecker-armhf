ARCH = armel
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
	make -C ../mainpanel/build.${ARCH}
	@cp -p ../mainpanel/build.${ARCH}/mainpanel bin/
	@cp -p ../scripts/appstart bin/
	@cp -p ../scripts/runsc bin/
	@cp -p ../ntpdate/ntpdate bin/
	@cp -p ../mainpanel/newsc_movie.sh bin/

felicalib:
	(cd ../felica-sdk/arch/linux/build && ./build.${ARCH}.sh)
	(cd ../nfc-extension/arch/linux/build && ./build.${ARCH}.sh)
	@cp -p ../felica-sdk/arch/linux/build/*.a ../libs.${ARCH}/
	@cp -p ../nfc-extension/arch/linux/build/*.a ../libs.${ARCH}/

tp-grid:
	make -C ../tp-grid/build.${ARCH}
	@cp -p ../tp-grid/build.${ARCH}/tp-grid bin/

cpprogress:
	make -C ../cpprogress/build.${ARCH}
	@cp -p ../cpprogress/build.${ARCH}/cpprogress bin/

log-battery:
	make -C ../log-battery/build.${ARCH}
	@cp -p ../log-battery/build.${ARCH}/log-battery bin/

xlib_lcdcheck:
	make -C ../lcd/build.${ARCH}
	@cp -p ../lcd/build.${ARCH}/xlib_lcdcheck bin/

demoapp:
	make -C ../prismapp/build.${ARCH}
	@cp -p ../prismapp/build.${ARCH}/demoapp-arm bin/
	@cp -p ../prismapp/newsc_demoapp.sh bin/

wlantool:
	make -C ../wlantool/build.${ARCH}
	@cp -p ../wlantool/build.${ARCH}/wlantool bin/
	@cp -p ../wlantool/newsc_wlantool.sh bin/

libqrencode:
	make -C ../libqrencode/build.${ARCH}
	@cp -p ../libqrencode/build.${ARCH}/libqrencode.a ../libs.${ARCH}/

clean:
	make -C ../lcd/build.${ARCH} clean
	make -C ../prismapp/build.${ARCH} clean
	make -C ../mainpanel/build.${ARCH} clean
	make -C ../tp-grid/build.${ARCH} clean
	make -C ../cpprogress/build.${ARCH} clean
	make -C ../log-battery/build.${ARCH} clean
	make -C ../wlantool/build.${ARCH} clean
	@rm -rf bin
#	@rm -f $(TARGET) demoapp-arm runsc appstart newsc_demoapp.sh newsc_wlantool.sh

felicaclean:
	(cd ../felica-sdk/arch/linux/build && ./build.$(ARCH).sh clean)
	(cd ../nfc-extension/arch/linux/build && ./build.$(ARCH).sh clean)

svnclean :
	make clean
	find -name '.svn' | xargs rm -rf
