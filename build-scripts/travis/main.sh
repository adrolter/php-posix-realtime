#!/usr/bin/env sh

phpize

if [ "$?" != "0" ]; then
	printf 'Phpize failed.'
	exit 1
fi

./configure

if [ "$?" != "0" ]; then
	printf 'Configure failed.'
	exit 1
fi

make

if [ "$?" != "0" ]; then
	printf 'Make failed.'
	exit 1
fi

PSXRT_MAKE_TEST=$(echo "n" | make test)

printf '%s' "${PSXRT_MAKE_TEST}" | grep "Tests passed.*(100.0%)$"
