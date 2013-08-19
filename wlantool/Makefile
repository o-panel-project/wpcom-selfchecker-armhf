HOST = arm-none-linux-gnueabi

ifdef HOST
  CC = $(HOST)-gcc
  PREFIX  = /opt/sysroots/armv7a-none-linux-gnueabi
else
  CC = gcc
endif

PROGRAM  = wlantool
OBJS     = ReadWLAN.o WLANtool.o
PING_DIR = netkit-base
PING     = ping

PKG_CONFIG = PKG_CONFIG_SYSROOT_DIR=$(PREFIX) pkg-config

#CFLAGS  = -Wall -Wno-unused-result -g -O0 -I$(PREFIX)/usr/include $(shell $(PKG_CONFIG) --cflags pangoxft)
CFLAGS  = -Wall -g -O0 -I$(PREFIX)/usr/include $(shell $(PKG_CONFIG) --cflags pangoxft)
LDFLAGS = -L$(PREFIX)/usr/lib -Wl,--rpath-link,$(PREFIX)/usr/lib $(shell $(PKG_CONFIG) --libs pangoxft)

all : $(PROGRAM) $(PING)

$(PROGRAM) : $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

.c.o :
	$(CC) -c $< $(CFLAGS)

$(PING) :
	cd $(PING_DIR) ; ./configure --with-c-compiler=$(CC)
	cd $(PING_DIR) ; make SUB=$(PING)
	cp $(PING_DIR)/$(PING)/$(PING) .

clean :
	rm -rf *.o *.a $(PROGRAM) $(PING)
	cd $(PING_DIR) ; make distclean
	
