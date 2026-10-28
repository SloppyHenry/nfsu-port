#!/bin/bash
# check.sh builddir : Syntaxcheck aller Quellen, Fehlerstatistik nach build/errors.txt
B=${1:-build}
ls $B/src/*.c | xargs -P 8 -I{} sh -c 'clang -fsyntax-only -w -ferror-limit=0 -std=gnu17 -target i686-pc-windows-msvc -fms-extensions {} 2>&1' > $B/errors.txt
echo "errors: $(grep -c 'error:' $B/errors.txt)"
grep 'error:' $B/errors.txt | sed -E 's/^[^ ]+ //; s/'"'"'[^'"'"']*'"'"'/Q/g; s/[0-9]+/N/g' | sort | uniq -c | sort -rn | head -${2:-25}
