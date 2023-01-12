include ../../Build.${ARCH}/rules.mk
PKGCONFIG=../../scripts/pkg-config-${ARCH}.sh

PROGRAM	= wlantool
SRCS = ReadWLAN.c WLANtool.c version.c common.c md5.c tctlprof.c
OBJS = ${SRCS:%.c=%.o}

VPATH = ../:../../common

CFLAGS += -Wall -O2 -D_C99_SOURCE
CFLAGS += -I.. -I../../common -I$(SYSROOT)/usr/include
CFLAGS += $(shell $(PKGCONFIG) --cflags pangoxft)

LDFLAGS += -L$(SYSROOT)/usr/lib
LDFLAGS += -Wl,--rpath-link,$(SYSROOT)/usr/lib
LDFLAGS += $(shell $(PKGCONFIG) --libs pangoxft x11)

PING_DIR = ../netkit-base
PING     = ping

all: $(PROGRAM)

$(PROGRAM): $(OBJS) depend.inc
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	$(STRIP) $(STRIP_OPT) $@

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	-rm -rf $(OBJS) $(PROGRAM) depend.inc

$(PING): $(PING_DIR)/ping/ping
	cd $(PING_DIR) ; ./configure --with-c-compiler=$(CC)
	cd $(PING_DIR) ; make SUB=ping
	cp -p $(PING_DIR)/ping/ping ./$(PING)

clean-ping:
	rm -rf $(PING)
	cd $(PING_DIR) ; make distclean

depend.inc: $(SRCS)
	$(CC) -MM $(CFLAGS) $^ > depend.inc

-include depend.inc
