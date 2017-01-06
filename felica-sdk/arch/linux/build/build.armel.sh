#!/bin/sh
#
# Description:
#	Script for building armhf libraries
#

TOOL=arm-none-linux-gnueabi-

export AR=${TOOL}ar
export CC=${TOOL}gcc
export LD=${TOOL}ld
#export CFLAGS=-mfloat-abi=hard

make -f Makefile.basic_rcs956_usb_all $*

#if [ $# -eq 0 ]; then
#  rm -rf libs.armel
#  mkdir -p libs.armel
#  cp *.a libs.armel
#fi
