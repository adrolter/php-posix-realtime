#!/usr/bin/env sh

phpize

if [ "$?" != "0" ]; then
	echo "Phpize failed."
	exit 1
fi

./configure

if [ "$?" != "0" ]; then
	echo "Configure failed."
	exit 1
fi

make

if [ "$?" != "0" ]; then
	echo "Make failed."
	exit 1
fi

PSXCLK_MAKE_TEST=$(echo "n" | make test) 

echo "${PSXCLK_MAKE_TEST}" | grep "Tests passed.*(100.0%)$"