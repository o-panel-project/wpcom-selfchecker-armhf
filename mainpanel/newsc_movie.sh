#!/bin/sh
#######################################################
###                                                 ###
### new SelfCheck movie start script                ###
### newsc_movie.sh $1 [$2]                          ###
### $1 : play file                                  ###
### $2 : -loop : loop play (optional)               ###
### 2015/10/15 WPC                                  ###
###                                                 ###
#######################################################

if [ $# -lt 1 ]; then
  echo "Usage : $0 file [-loop]"
  exit 1
fi
MOVIEFILE=$1

LOOPPLAY=0
if [ $# -eq 2 ]; then
  if [ "$2" = "-loop" ]; then
    LOOPPLAY=1
  fi
fi

echo ${MOVIEFILE}
echo "loop play = ${LOOPPLAY}"

if [ ${LOOPPLAY} -eq 1 ]; then
  while :
  do
  mplayer -ao alsa -vo x11 -geometry 460:60 ${MOVIEFILE}
  done
else
  mplayer -ao alsa -vo x11 -geometry 460:60 ${MOVIEFILE}
fi

#Script finish
