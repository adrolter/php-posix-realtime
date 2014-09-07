#!/usr/bin/env sh

# script:
. ./build-scripts/travis/main.sh

# Disable for now as main.sh has taken over previous functionality
#if [ "$?" != "0" ]; then
#	# after_failure:
#	. ./build-scripts/travis/after_failure.sh
#	return 1
#fi
