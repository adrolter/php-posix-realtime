#!/usr/bin/env sh

phpize

./configure

make

PSXCLK_MAKE_TEST=$(echo "n" | make test) 

echo "${PSXCLK_MAKE_TEST}" | grep "Tests passed.*(100.0%)$"