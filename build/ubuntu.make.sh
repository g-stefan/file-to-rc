#!/bin/sh
# Public domain
# http://unlicense.org/
# Created by Grigore Stefan <g_stefan@yahoo.com>

ACTION=$1
if [ "$ACTION" = "" ]; then
	ACTION=make
fi

echo "-> $ACTION file-to-rc"

cmdX(){
	if ! "$@" ; then
		echo "Error: $ACTION"
		exit 1
	fi
}

cmdX xyo-cc --mode=$ACTION @build/source/file-to-rc.compile

