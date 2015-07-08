ifeq ($(BOARD_TYPE),j4)
include rules-j4.mk
else
include rules.mk
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
	make -C ../mainpanel
ifeq ($(BOARD_TYPE),j4)
	@cp -p ../mainpanel/mainpanel mainpanel.j4obj
else
	@cp -p ../mainpanel/mainpanel .
endif

felicalib:
	make -C ../felica-sdk/arch/linux/build -f Makefile.basic_rcs956_usb_all
	make -C ../nfc-extension/arch/linux/build -f Makefile.nfc_rcs956_all
ifeq ($(BOARD_TYPE),j4)
	@cp -p ../felica-sdk/arch/linux/build/*.a ../libs.j4/
	@cp -p ../nfc-extension/arch/linux/build/*.a ../libs.j4/
else
	@cp -p ../felica-sdk/arch/linux/build/*.a ../libs/
	@cp -p ../nfc-extension/arch/linux/build/*.a ../libs/
endif

tp-grid:
	make -C ../tp-grid
ifeq ($(BOARD_TYPE),j4)
	@cp -p ../tp-grid/tp-grid tp-grid.j4obj
else
	@cp -p ../tp-grid/tp-grid .
endif

cpprogress:
	make -C ../cpprogress
ifeq ($(BOARD_TYPE),j4)
	@cp -p ../cpprogress/cpprogress cpprogress.j4obj
else
	@cp -p ../cpprogress/cpprogress .
endif

log-battery:
	make -C ../log-battery
ifeq ($(BOARD_TYPE),j4)
	@cp -p ../log-battery/log-battery log-battery.j4obj
else
	@cp -p ../log-battery/log-battery .
endif

xlib_lcdcheck:
	make -C ../lcd
ifeq ($(BOARD_TYPE),j4)
	@cp -p ../lcd/xlib_lcdcheck xlib_lcdcheck.j4obj
else
	@cp -p ../lcd/xlib_lcdcheck .
endif

demoapp:
	make -C ../prismapp/demoapp
ifeq ($(BOARD_TYPE),j4)
	@cp -p ../prismapp/demoapp/demoapp-arm demoapp-arm.j4obj
else
	@cp -p ../prismapp/demoapp/demoapp-arm .
endif

wlantool:
	make -C ../wlantool
ifeq ($(BOARD_TYPE),j4)
	@cp -p ../wlantool/wlantool wlantool.j4obj
else
	@cp -p ../wlantool/wlantool .
endif

clean:
	make -C ../lcd clean
	make -C ../prismapp/demoapp clean
	make -C ../mainpanel clean
	make -C ../tp-grid clean
	make -C ../cpprogress clean
	make -C ../log-battery clean
	@rm -f $(TARGET) demoapp-arm *.j4obj

svnclean :
	make clean
	find -name '.svn' | xargs rm -rf
