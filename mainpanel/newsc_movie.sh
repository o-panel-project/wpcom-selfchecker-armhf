#!/bin/sh
#######################################################
###                                                 ###
### new SelfCheck movie start script                ###
### newsc_movie.sh -fFILE [-xX] [-yY] [-loop]       ###
### FILE : play file                                ###
### X    : play x pos (optional)                    ###
### Y    : play y pos (optional)                    ###
### -loop : loop play (optional)                    ###
### 2015/10/15 WPC                                  ###
### 2016/01/07 WPC                                  ###
### 2022/07/20 WPC video out to XV                  ###
###                                                 ###
#######################################################

MOVIEFILE=
POSX=460
POSY=60
LOOPPLAY=0

while getopts "f:x:y:l:" opts; do
  case $opts in
  f) MOVIEFILE="$OPTARG";;
  x) POSX="$OPTARG";;
  y) POSY="$OPTARG";;
  l) LOOPPLAY=1
  esac
done

if [ -z ${MOVIEFILE} ]; then
  echo "Usage : $0 -f file [-x posx] [-y posy] [-loop]"
  exit 0
fi

echo "play file = ${MOVIEFILE}"
echo "play pos = ${POSX}x${POSY}"
echo "loop play = ${LOOPPLAY}"

export DISPLAY=:0
if [ ${LOOPPLAY} -eq 1 ]; then
  while :
  do
  mplayer -ao alsa -vo xv -geometry ${POSX}:${POSY} ${MOVIEFILE}
  done
else
  mplayer -ao alsa -vo xv -geometry ${POSX}:${POSY} ${MOVIEFILE}
fi

exit 1
#Script finish
