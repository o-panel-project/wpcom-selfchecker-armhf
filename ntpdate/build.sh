#!/bin/sh

tar xf ntp-4.2.6p5.tar.gz
cd ntp-4.2.6p5
./configure --host=arm-none-linux-gnueabi
cd ntpdate
make
cp ntpdate ../..

