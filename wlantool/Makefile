ifeq ($(BOARD_TYPE),j4)
include ../Build/rules-j4.mk
else
include ../Build/rules.mk
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
LDFLAGS += $(shell $(PKGCONFIG) --libs pangoxft x11)

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
