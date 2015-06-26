#!/bin/sh

SYSROOT=/opt/j4-panel/buildroot/output/host/usr/arm-buildroot-linux-gnueabi/sysroot
export PKG_CONFIG_DIR=
export PKG_CONFIG_LIBDIR=${SYSROOT}/usr/lib/pkgconfig
export PKG_CONFIG_SYSROOT_DIR=${SYSROOT}

exec /opt/j4-panel/buildroot/output/host/usr/bin/pkg-config "$@"
