#!/usr/bin/env sh

printf '%s' "${PSXRT_MAKE_TEST}"

printf '\n'

find tests/ -name "*.diff" | while read diffFile do; do
	printf '\n%s << EOF\n%s\nEOF\n' "$diffFile" "$(cat $diffFile)"
done
