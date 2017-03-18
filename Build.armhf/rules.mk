ARCH=armhf

#CROSS_COMPILE = arm-linux-gnueabihf-
CROSS_COMPILE = /opt/gcc-linaro-4.9-2015.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
SYSROOT = /opt/arm-dev/armhf

CC = $(CROSS_COMPILE)gcc
CFLAGS += -march=armv7-a -mtune=cortex-a9 -mfpu=neon -mhard-float
CFLAGS += -mfloat-abi=hard -ftree-vectorize -mvectorize-with-neon-quad
CFLAGS += -mthumb-interwork

STRIP = ${CROSS_COMPILE}strip
STRIP_OPT = --remove-section=.comment --remove-section=.note

LD = $(CROSS_COMPILE)ld
AR = $(CROSS_COMPILE)ar

LDFLAGS += --sysroot=${SYSROOT}
PKGCONFIG=../scripts/pkg-config-armhf.sh
