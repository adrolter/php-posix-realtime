#!/usr/bin/env sh

echo "${PSXRT_MAKE_TEST}"

echo "\n"

find tests/ -name "*.diff" -print | xargs sh -c 'for f; do echo "$f\n------" && cat "$f" && echo "\n------\n"; done' fnord
