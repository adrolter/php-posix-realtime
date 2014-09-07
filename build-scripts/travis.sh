#!/usr/bin/env sh

# script:
. ./build-scripts/travis/main.sh

if [ "$?" != "0" ]; then
	# after_failure:
	. ./build-scripts/travis/after_failure.sh
	return 1
fi
