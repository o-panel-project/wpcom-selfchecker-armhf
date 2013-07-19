#
# $Id: Makefile 237 2013-04-03 04:15:43Z r-hara $
#

ARCH = armel
CROSSTOOLS = arm-none-linux-gnueabi-
CROSSDEVDIR = /opt/arm-dev/sysroots/armv7a-none-linux-gnueabi

TARGET = demoapp-arm
SRCS =
CPPSRCS = mainpanel.cpp ovganimwin.cpp

SRCDIR = ./

CC = ${CROSSTOOLS}gcc
LD = ${CROSSTOOLS}gcc
AR = ${CROSSTOOLS}ar
STRIP = ${CROSSTOOLS}strip
STRIP_OPT = --remove-section=.comment --remove-section=.note

CFLAGS += -Wall
ifeq ($(ARCH),armel)
CFLAGS += -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mhard-float
CFLAGS += -mfloat-abi=softfp -ftree-vectorize -mvectorize-with-neon-quad
CFLAGS += -mthumb-interwork -mno-thumb
endif

ifeq ($(BUILD_DEBUG),yes)
CFLAGS += -g -D_DEBUG
else
CFLAGS += -Os
endif

OBJS = ${SRCS:%.c=%.o} ${CPPSRCS:%.cpp=%.o}

VPATH = ${SRCDIR}
CFLAGS += -I.
CFLAGS += -I${CROSSDEVDIR}/usr/include
LDFLAGS += -L${CROSSDEVDIR}/usr/lib

#include ../../buildroot/makerules/rules.com

#USE_TCTLAPI = yes
#USE_TCTLXAPI = yes
#include ../../buildroot/makerules/rules.api
#USE_PRISM = yes
#BUILD_LIB = static
#include ../../buildroot/makerules/rules.prism

CFLAGS += -I../include
CFLAGS += -I$(CROSSDEVDIR)/usr/include/freetype2
LDFLAGS += -L../lib
LIBS += -lprism
LIBS += -lpthread -lfreetype -lz
LIBS += -lX11 -lXext -lXi -lxcb -lXdmcp -lXau
LIBS += -lEGL -lOpenVG -lIMGegl -lpvr2d -lsrv_um
DEPLIBS += ../lib/libprism.a
LDFLAGS += ${LIBS} -lstdc++

CFLAGS += ${OPTCADD}

CXXFLAGS =

LDFLAGS += ${OPTLADD}

all: depend.inc $(TARGET)

$(TARGET): $(OBJS) $(DEPLIBS)
	@echo "=== linking " ${CC} " : " $@
	$(CC) -o $@ $(OBJS) $(LDFLAGS)
	@echo "=== striping " ${CC} " : " $@
	$(STRIP) $(STRIP_OPT) $@

%.o: %.c
	@echo "=== compiling " ${CC} " : " $@
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	@echo "=== compiling " ${CC} " : " $@
	$(CC) -c $(CFLAGS) $(CXXFLAGS) -o $@ $<

clean:
	@echo "=== cleaning ==="
	-rm -f $(OBJS) $(TARGET) depend.inc

# depend header file
depend.inc: $(SRCS) $(CPPSRCS)
	@echo "=== header file dependency resolv ==="
	$(CC) -MM $(CFLAGS) $^ > depend.inc

-include depend.inc
