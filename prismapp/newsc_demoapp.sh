#!/bin/sh
#######################################################
###                                                 ###
### new SelfCheck demoapp start script              ###
### newsc_demoapp.sh                                ###
### 2011/08/04 VACS                                 ###
### 2011/08/11 Change vacslab                       ###
### 2011/08/20 Change vacslab                       ###
### 2013/05/31 Change WPC                           ###
###                                                 ###
#######################################################

cd /mnt1/demoapp
export DISPLAY=:0
../bin/demoapp-arm -x0 -y200 -d300 -bbg.jpg -z2 -iimages
cd /

#Script finish
