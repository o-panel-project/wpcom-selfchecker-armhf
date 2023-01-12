#
# $Id: Makefile 237 2013-04-03 04:15:43Z r-hara $
#

ARCH = armhf
CROSSTOOLS = arm-linux-gnueabihf-
CROSSDEVDIR = /opt/arm-dev/armhf

TARGET = demoapp-arm
SRCS = tctl.c
CPPSRCS = mainpanel.cpp oglanimwin.cpp

SRCDIR = ../demoapp

CC = ${CROSSTOOLS}gcc
LD = ${CROSSTOOLS}gcc
AR = ${CROSSTOOLS}ar
STRIP = ${CROSSTOOLS}strip
STRIP_OPT = --remove-section=.comment --remove-section=.note

CFLAGS += -Wall
ifeq ($(ARCH),armhf)
CFLAGS += -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mhard-float
CFLAGS += -mfloat-abi=hard -ftree-vectorize -mvectorize-with-neon-quad
CFLAGS += -mthumb-interwork
endif
CFLAGS += -D__O_PANEL__

ifeq ($(BUILD_DEBUG),yes)
CFLAGS += -g -D_DEBUG
else
CFLAGS += -Os
endif

OBJS = ${SRCS:%.c=%.o} ${CPPSRCS:%.cpp=%.o}

VPATH = ${SRCDIR}
CFLAGS += -I../demoapp -I../include
CFLAGS += -I${CROSSDEVDIR}/usr/include
LDFLAGS += -L${CROSSDEVDIR}/usr/lib
LDFLAGS += --sysroot=${CROSSDEVDIR}

CFLAGS += -I$(CROSSDEVDIR)/usr/include/freetype2
LDFLAGS += -L../lib.${ARCH}
LIBS += -lprism
LIBS += -lstdc++ -lpthread -lfreetype -lm
LIBS += -lX11 -lXext -lXi
LIBS += -lEGL -lGLES_CM
DEPLIBS += ../lib.${ARCH}/libprism.a
LDFLAGS += ${LIBS}

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
