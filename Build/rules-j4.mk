CROSS_COMPILE = /opt/arm-dev/gcc-linaro-arm-linux-gnueabi-2012.04-20120426_linux/bin/arm-linux-gnueabi-
SYSROOT = /opt/j4-panel/buildroot/output/host/usr/arm-buildroot-linux-gnueabi/sysroot

CC = $(CROSS_COMPILE)gcc
CFLAGS += -march=armv7-a -mtune=cortex-a8 -mfpu=neon -msoft-float
CFLAGS += -mfloat-abi=softfp -ftree-vectorize -mvectorize-with-neon-quad
CFLAGS += -mthumb-interwork
CFLAGS += -D__J4PANEL__

STRIP = ${CROSS_COMPILE}strip
STRIP_OPT = --remove-section=.comment --remove-section=.note

LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar

PKGCONFIG=../scripts/pkg-config-j4.sh
