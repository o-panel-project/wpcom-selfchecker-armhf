CROSS_COMPILE = arm-none-linux-gnueabi

ifdef CROSS_COMPILE
  CC     = $(CROSS_COMPILE)-gcc
  LD     = $(CROSS_COMPILE)-ld
  AR     = $(CROSS_COMPILE)-ar
  PREFIX = /opt/arm-dev/sysroots/armv7a-none-linux-gnueabi
else
  CC     = gcc
  LD     = ld
  AR     = ar
  PREFIX =
endif

export AR
export CC
export LD

#TARGET=tp-grid mainpanel log-battery cpprogress
TARGET= mainpanel tp-grid log-battery cpprogress xlib_lcdcheck demoapp

.PHONY: $(TARGET)
all: $(TARGET)

mainpanel: felicalib
	make -C ../mainpanel
	@cp -p ../mainpanel/mainpanel .

felicalib:
	make -C ../felica-sdk/arch/linux/build -f Makefile.basic_rcs956_usb_all
	make -C ../nfc-extension/arch/linux/build -f Makefile.nfc_rcs956_all

tp-grid:
	make -C ../tp-grid
	@cp -p ../tp-grid/tp-grid .

cpprogress:
	make -C ../cpprogress
	@cp -p ../cpprogress/cpprogress .

log-battery:
	make -C ../log-battery
	@cp -p ../log-battery/log-battery .

xlib_lcdcheck:
	make -C ../lcd
	@cp -p ../lcd/xlib_lcdcheck .

demoapp:
	make -C ../prismapp/demoapp
	@cp -p ../prismapp/demoapp/demoapp-arm .

clean:
	make -C ../lcd clean
	make -C ../demoapp clean
	make -C ../mainpanel clean
	make -C ../tp-grid clean
	make -C ../cpprogress clean
	make -C ../log-battery clean
	@rm -f $(TARGET) demoapp-arm

svnclean :
	make clean
	find -name '.svn' | xargs rm -rf
