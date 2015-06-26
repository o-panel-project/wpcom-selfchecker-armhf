ifeq ($(BOARD_TYPE),j4)
include ../Build/rules-j4.mk
else
include ../Build/rules.mk
endif

PROGRAM	= cpprogress
SRCS	= cpprogress.c
OBJS	= ${SRCS:%.c=%.o}

VPATH = ./

CFLAGS += -Wall -O2
CFLAGS += -I. -I.. -I../common -I$(SYSROOT)/usr/include

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
