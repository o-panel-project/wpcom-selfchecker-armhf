CROSS_COMPILE = arm-none-linux-gnueabi-

ifdef CROSS_COMPILE
  CC = $(CROSS_COMPILE)gcc
  SYSROOT=/opt/arm-dev/sysroots/armv7a-none-linux-gnueabi
  PKGCONFIG=../scripts/pkg-config.sh
  CFLAGS += -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mhard-float
  CFLAGS += -mfloat-abi=softfp -ftree-vectorize -mvectorize-with-neon-quad
  CFLAGS += -mthumb-interwork -mno-thumb
  STRIP = ${CROSS_COMPILE}strip
  STRIP_OPT = --remove-section=.comment --remove-section=.note
else
  CC = gcc
  SYSROOT=
  PKGCONFIG=/usr/bin/pkg-config
  STRIP = strip
  STRIP_OPT =
endif

PROGRAM  = wlantool
SRCS     = ReadWLAN.c WLANtool.c version.c common.c md5.c
OBJS     = ${SRCS:%.c=%.o}

VPATH = ./:../common

PING_DIR = netkit-base
PING     = ping

CFLAGS += -Wall -O2
CFLAGS += -I. -I../common -I$(SYSROOT)/usr/include
CFLAGS += $(shell $(PKGCONFIG) --cflags pangoxft)

LDFLAGS += -L$(SYSROOT)/usr/lib
LDFLAGS += -Wl,--rpath-link,$(SYSROOT)/usr/lib
LDFLAGS += $(shell $(PKGCONFIG) --libs pangoxft)

all: $(PROGRAM) $(PING)

$(PROGRAM): $(OBJS) depend.inc
	$(CC) -c $(CFLAGS) -o version.o version.c
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(STRIP) $(STRIP_OPT) $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

$(PING):
	cd $(PING_DIR) ; ./configure --with-c-compiler=$(CC)
	cd $(PING_DIR) ; make SUB=$(PING)
	cp $(PING_DIR)/$(PING)/$(PING) .

clean:
	rm -rf *.o *.a $(PROGRAM)

clean-ping:
	rm -rf $(PING)
	cd $(PING_DIR) ; make distclean

depend.inc: $(SRCS)
	$(CC) -MM $(CFLAGS) $^ > depend.inc

-include depend.inc
