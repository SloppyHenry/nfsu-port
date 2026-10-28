#!/bin/sh
# Spielt symbols/ ins dauerhafte Ghidra-Projekt ein und exportiert nach decompiled/full/out3 (WP-01).
# Dauer ca. 10 min, im Hintergrund starten: tools/reexport.sh > decompiled/full/reexport.log 2>&1 &
set -e
ROOT=$(cd "$(dirname "$0")/.." && pwd)
export JAVA_HOME=/opt/homebrew/Cellar/openjdk@21/21.0.12.1/libexec/openjdk.jdk/Contents/Home
export PATH=$JAVA_HOME/bin:$PATH
OUT=${1:-$ROOT/decompiled/full/out3}
cd "$ROOT/decompiled/full"
rm -rf "$OUT.tmp"
"$HOME/rea-stack/ghidra_12.1.4_PUBLIC/support/analyzeHeadless" project_persist nfsu \
  -process Speed.exe -noanalysis -scriptPath ghidra_scripts \
  -preScript ApplySymbols.java "$ROOT/symbols" \
  -preScript ApplyTypes.java "$ROOT/symbols" \
  -postScript ExportAll3.java "$OUT.tmp" 2>&1 | tee "$OUT.log"
if grep -q "SCRIPT ERROR\|APPLY DONE.*errors=[1-9]\|TYPES DONE.*errors=[1-9]" "$OUT.log" || ! grep -q "APPLY DONE" "$OUT.log" || ! grep -q "TYPES DONE" "$OUT.log"; then
  echo "REEXPORT FEHLER (siehe $OUT.log)"; exit 1
fi
grep -q . "$OUT.tmp/functions.tsv"
rm -rf "$OUT" && mv "$OUT.tmp" "$OUT"
rm -f "$OUT/.fnindex"
echo "REEXPORT OK -> $OUT"
