#!/bin/sh
#
# Description:
#	Script for building armhf libraries
#

TOOL=arm-linux-gnueabihf-

export AR=${TOOL}ar
export CC=${TOOL}gcc
export LD=${TOOL}ld
#export CFLAGS=-mfloat-abi=hard

make -f Makefile.nfc_rcs956_all $*

#if [ $# -eq 0 ]; then
#  rm -rf libs.armhf
#  mkdir -p libs.armhf
#  cp *.a libs.armhf
#fi
