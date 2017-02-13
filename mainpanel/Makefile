include ../Build/rules.mk

PROGRAM	= mainpanel
SRCS	= mainpanel.c sdram.c common.c common-gtk.c audio_mov.c wifi.c md5.c \
		  lcd-pwm.c battery.c strategy.c led.c dipsw.c tpoff.c i2c-cradle.c \
		  felica.c usbmem.c updown.c date.c rcs956scan.c tctlprof.c \
		  dmesg.c reset.c wifisub.c
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
LDFLAGS += $(shell $(PKGCONFIG) --libs gtk+-2.0 alsa x11 gthread-2.0)

FELICA_SDK_LIB0 = \
	libfelica_cc \
	librcs956_usb \
	librcs956 \
	libutl \
	libfelica_cc_stub_rcs956
FELICA_SDK_LIB  = $(FELICA_SDK_LIB0:%=../libs/%.a)
LDFLAGS += $(FELICA_SDK_LIB)

NFCEXT_LIB0 = librcs956_typea librcs956_typeb
NFCEXT_LIB  = $(NFCEXT_LIB0:%=../libs/%.a)
LDFLAGS += $(NFCEXT_LIB)

all: depend.inc $(PROGRAM)

$(PROGRAM): $(OBJS) $(FELICA_SDK_LIB) $(NFCEXT_LIB)
	$(CC) -c $(CFLAGS) -o version.o version.c
	$(CC) -o $@ $(OBJS) version.o $(LDFLAGS)
	$(STRIP) $(STRIP_OPT) $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	-rm -rf $(OBJS) $(PROGRAM) depend.inc

depend.inc: $(SRCS)
	$(CC) -MM $(CFLAGS) $^ > depend.inc

-include depend.inc
