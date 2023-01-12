include ../../Build.${ARCH}/rules.mk
PKGCONFIG=../../scripts/pkg-config-${ARCH}.sh

PROGRAM	= cpprogress
SRCS	= cpprogress.c
OBJS	= ${SRCS:%.c=%.o}

VPATH = ../

CFLAGS += -Wall -O2
CFLAGS += -I.. -I../.. -I../../common -I$(SYSROOT)/usr/include

LDFLAGS += -L$(SYSROOT)/usr/lib
LDFLAGS += -Wl,--rpath-link,$(SYSROOT)/usr/lib

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
