#!/bin/sh
#######################################################
###                                                 ###
### new SelfCheck wlantool start script             ###
### newsc_wlantool.sh                               ###
### 2011/08/20 VACS                                 ###
### 2013/07/22 WPC                                  ###
### 2015/09/04 WPC                                  ###
###                                                 ###
#######################################################
STR_CURRENT_DIR=`pwd`

#Script start
	cd /mnt1/wlantoolapp

	if [ ! -d /mnt1/wlantoolapp/tmp ]; then
		mkdir /mnt1/wlantoolapp/tmp
	fi

	../bin/wlantool

	cd $STR_CURRENT_DIR
#Script finish
