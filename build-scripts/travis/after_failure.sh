#!/usr/bin/env sh

find tests/ -type f -name "*.log" | while read file do; do
	printf '\n%s << EOF\n%s\nEOF\n' "$file" "$(cat $file)"
done

return 1