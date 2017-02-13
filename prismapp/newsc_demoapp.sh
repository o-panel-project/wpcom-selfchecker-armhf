#!/bin/sh
#######################################################
###                                                 ###
### new SelfCheck demoapp start script              ###
### newsc_demoapp.sh                                ###
### 2011/08/04 VACS                                 ###
### 2011/08/11 Change vacslab                       ###
### 2011/08/20 Change vacslab                       ###
### 2013/05/31 Change WPC                           ###
### 2016/01/08 Change WPC                           ###
###                                                 ###
#######################################################

if [ $# -ne 4 ]; then
  echo "Usage $0 pos_x pos_y lcd_width lcd_height"
  exit 0
fi

cd /mnt1/demoapp
export DISPLAY=:0
../bin/demoapp-arm -x$1 -y$2 -bbg$3x$4.jpg -iimages
cd /

#Script finish
