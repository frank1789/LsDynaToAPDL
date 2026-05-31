#!/usr/bin/env sh

# make uniform file names by replacing dots with underscores
# headers with must end .h and sources with .cc

find /Users/francesco/Documents/Projects/LsDynaToAPDL/src -type f -name "*.*" | while read file; do
    dir=$(dirname "$file")
    base=$(basename "$file")
    ext="${base##*.}"
    name="${base%.*}"

    # Only process .h and .cc files
    if [ "$ext" = "h" ] || [ "$ext" = "cc" ]; then
        new_name="${name//./_}.$ext"
        git mv "$file" "$dir/$new_name"
    fi
done
