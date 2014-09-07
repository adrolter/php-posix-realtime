#!/usr/bin/env sh

while : ; do
	. ./build-scripts/clean.sh
	. ./build-scripts/travis.sh
	if [ "$?" != "0" ]; then break; fi
done
