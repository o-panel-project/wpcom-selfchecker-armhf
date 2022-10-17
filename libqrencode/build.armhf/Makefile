include ../../Build.armhf/rules.mk

PROGRAM	= libqrencode.a
SRCS = bitstream.c mmask.c qrinput.c rsecc.c mask.c mqrspec.c \
	   qrencode.c qrspec.c split.c
OBJS	= ${SRCS:%.c=%.o}

VPATH = ../src

CFLAGS += -Wall -O2 -DHAVE_CONFIG_H
CFLAGS += -I. -I../include -I$(SYSROOT)/usr/include
#CFLAGS += $(shell $(PKGCONFIG) --cflags libpng)

#LDFLAGS += -L$(SYSROOT)/usr/lib
#LDFLAGS += -Wl,--rpath-link,$(SYSROOT)/usr/lib
#LDFLAGS += $(shell $(PKGCONFIG) --libs libpng)

all: depend.inc $(PROGRAM)

$(PROGRAM): $(OBJS)
	$(AR) r $@ $(OBJS)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	-rm -rf $(OBJS) $(PROGRAM) depend.inc

depend.inc: $(SRCS)
	$(CC) -MM $(CFLAGS) $^ > depend.inc

-include depend.inc
