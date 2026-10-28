#!/bin/sh
# WP-16: vergleicht die Bild-MD5 von nfsu-tool movie-md5 mit ffmpeg -f framemd5 für alle Filme.
T=$(mktemp -d); ok=0; bad=0
for f in game/pc/Movies/*.mad; do b=$(basename "$f"); n=${b%.mad}
  port/build/release/nfsu-tool movie-md5 "Movies/$b" 2>/dev/null | awk '{print $2}' > "$T/o"
  ffmpeg -v error -i "$f" -map 0:v -pix_fmt yuv420p -f framemd5 - | grep -v '^#' | awk -F, '{gsub(/ /,"",$6);print $6}' > "$T/r"
  if cmp -s "$T/o" "$T/r" && [ -s "$T/o" ]; then ok=$((ok+1)); else bad=$((bad+1)); echo "ABWEICHUNG $n"; fi
done; echo "gleich=$ok abweichend=$bad"
