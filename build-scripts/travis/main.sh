#!/usr/bin/env sh

header () {
	printf '\n\n\n\n*************************************\n\t%s\n*************************************\n' "$1"
}


header "PHPIZE"
phpize
if [ "$?" != "0" ]; then
	printf '\n\nPhpize failed.\n\n'
	return 1
fi


header "CONFIGURE"
./configure
if [ "$?" != "0" ]; then
	printf '\n\nConfigure failed.\n\n'
	return 1
fi


header "MAKE"
make
if [ "$?" != "0" ]; then
	printf '\n\nMake failed.\n\n'
	return 1
fi


header "TEST"
# Store the result of make test
PSXRT_MAKE_TEST=$(TEST_PHP_ARGS="-q --show-diff" make test)
# Print "make test" output
printf '%s' "${PSXRT_MAKE_TEST}"
# grep returns non-0 exit code if not matched
printf '%s' "${PSXRT_MAKE_TEST}" | grep -q "Tests passed.*(100.0%)$"
if [ "$?" != "0" ]; then
	printf '\n\nTest(s) failed.\n\n'
	return 1
fi