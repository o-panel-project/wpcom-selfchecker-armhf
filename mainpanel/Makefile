CROSS_COMPILE = arm-none-linux-gnueabi-

ifdef CROSS_COMPILE
  CC = $(CROSS_COMPILE)gcc
  SYSROOT=/opt/arm-dev/sysroots/armv7a-none-linux-gnueabi
  PKGCONFIG=../scripts/pkg-config.sh
  CFLAGS += -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mhard-float
  CFLAGS += -mfloat-abi=softfp -ftree-vectorize -mvectorize-with-neon-quad
  CFLAGS += -mthumb-interwork -mno-thumb -fno-strict-aliasing
  STRIP = ${CROSS_COMPILE}strip
  STRIP_OPT = --remove-section=.comment --remove-section=.note
else
  CC = gcc
  SYSROOT=
  PKGCONFIG=/usr/bin/pkg-config
  STRIP = strip
  STRIP_OPT =
endif

PROGRAM	= mainpanel
SRCS	= mainpanel.c sdram.c common.c common-gtk.c audio_mov.c wifi.c md5.c \
		  lcd-pwm.c battery.c strategy.c led.c dipsw.c tpoff.c i2c-cradle.c \
		  felica.c usbmem.c updown.c date.c rcs956scan.c tctlprof.c
OBJS	= ${SRCS:%.c=%.o}

VPATH = ./ ../common

CFLAGS += -Wall -O2
CFLAGS += -I. -I.. -I../common -I$(SYSROOT)/usr/include
CFLAGS += -I../felica-sdk/common/include \
		  -I../felica-sdk/arch/linux/include \
		  -I../nfc-extension/common/include
CFLAGS += $(shell $(PKGCONFIG) --cflags gtk+-2.0)

LDFLAGS += -L$(SYSROOT)/usr/lib
LDFLAGS += -Wl,--rpath-link,$(SYSROOT)/usr/lib
LDFLAGS += $(shell $(PKGCONFIG) --libs gtk+-2.0 alsa)

FELICA_SDK_LIB0 = \
	libfelica_cc \
	librcs956_usb \
	librcs956 \
	libutl \
	libfelica_cc_stub_rcs956
FELICA_SDK_LIB  = $(FELICA_SDK_LIB0:%=../felica-sdk/arch/linux/build/%.a)
LDFLAGS += $(FELICA_SDK_LIB)

NFCEXT_LIB0 = librcs956_typea librcs956_typeb
NFCEXT_LIB  = $(NFCEXT_LIB0:%=../nfc-extension/arch/linux/build/%.a)
LDFLAGS += $(NFCEXT_LIB)

all: depend.inc $(PROGRAM)

$(PROGRAM): $(OBJS) $(FELICA_SDK_LIB) $(NFCEXT_LIB)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(STRIP) $(STRIP_OPT) $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	-rm -rf $(OBJS) $(PROGRAM) depend.inc

depend.inc: $(SRCS)
	$(CC) -MM $(CFLAGS) $^ > depend.inc

-include depend.inc
