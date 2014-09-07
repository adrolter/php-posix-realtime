#!/usr/bin/env sh

echo "${PSXRT_MAKE_TEST}"

echo -e "\n"

find tests/ -name "*.diff" -print | xargs sh -c 'for f; do echo -e "$f\n------" && cat "$f" && echo -e "\n------\n"; done' fnord
