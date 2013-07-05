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

PROGRAM	= tp-grid
SRCS	= tp-grid.c
OBJS	= ${SRCS:%.c=%.o}

VPATH = ./

CFLAGS += -Wall -O2
CFLAGS += -I. -I.. -I../common -I$(SYSROOT)/usr/include
CFLAGS += $(shell $(PKGCONFIG) --cflags gtk+-2.0)

LDFLAGS += -L$(SYSROOT)/usr/lib
LDFLAGS += -Wl,--rpath-link,$(SYSROOT)/usr/lib
LDFLAGS += $(shell $(PKGCONFIG) --libs gtk+-2.0)

all: depend.inc $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(STRIP) $(STRIP_OPT) $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	-rm -rf $(OBJS) $(PROGRAM) depend.inc

depend.inc: $(SRCS)
	$(CC) -MM $(CFLAGS) $^ > depend.inc

-include depend.inc
