ARCH = armel

CROSS_COMPILE = /opt/arm-dev/arm-2009q3/bin/arm-none-linux-gnueabi-
SYSROOT = /opt/arm-dev/sysroots/armv7a-none-linux-gnueabi

CC = $(CROSS_COMPILE)gcc
CFLAGS += -march=armv7-a -mtune=cortex-a8 -mfpu=neon -msoft-float
CFLAGS += -mfloat-abi=softfp -ftree-vectorize -mvectorize-with-neon-quad
CFLAGS += -mthumb-interwork -mno-thumb

STRIP = ${CROSS_COMPILE}strip
STRIP_OPT = --remove-section=.comment --remove-section=.note

LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar

PKGCONFIG=../scripts/pkg-config-$(ARCH).sh
