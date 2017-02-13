#!/bin/sh

SYSROOT=/opt/arm-dev/sysroots/armv7a-none-linux-gnueabi
export PKG_CONFIG_DIR=
export PKG_CONFIG_LIBDIR=${SYSROOT}/usr/lib/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=${SYSROOT}

exec /opt/arm-dev/sysroots/i686-linux/usr/bin/pkg-config "$@"
