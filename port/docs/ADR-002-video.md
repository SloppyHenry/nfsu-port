# ADR-002 Video (EA MAD)

**Entscheidung: (b) libavformat/libavcodec/libswresample.** Der eigene MAD-Decoder (a) ist nicht Teil von WP-16.

## Befund
- 45 Dateien in `Movies/`, 325 MB: 39 × 512×256, 6 × 512×512, 30,303 fps, Ton `adpcm_ea_r2` 44,1 kHz Stereo.
- Container: EA-Multimedia (`MADk`-Kopf, `SCHl`-Block). FFmpeg liest ihn mit `ea`, Bild mit `eamad`.
- Keine Untertitel-Dateien in `Movies/`; im Original-Player (`RCMP_Player.cpp`) bisher kein Untertitelpfad gefunden.

## Gründe
- Der Quelltext des FFmpeg-`eamad`-Decoders liegt nicht lokal vor, MPEG-Tabellen (VLC, Quantisierer, IDCT) aus dem Gedächtnis nachzubauen ist fehleranfällig; als Referenz wäre nur FFmpeg selbst da.
- Aufwand eines eigenen Decoders > 1 Session (Roadmap-Grenze: ≤ 2 Sessions, aber ohne Quelltext unsicher).
- Beim Spielen der Filme sieht niemand den Unterschied; Verhalten ist durch die FFmpeg-Bitgleichheit festgelegt.

## Folgen
- Abhängigkeit: Homebrew-FFmpeg (heute 9.0.2) per pkg-config (`PKG_CONFIG_PATH=/opt/homebrew/lib/pkgconfig`). Für das App-Bundle (WP-53) müssen die Dylibs mitgeliefert werden; LGPL verlangt dynamisches Linken und Hinweis. Homebrew-FFmpeg ist GPL-konfiguriert möglich → vor WP-53 Lizenz der gebündelten Build-Variante prüfen oder eigenen LGPL-Build mit nur `ea`/`eamad`/`adpcm_ea_r2` bauen.
- Rückbau offen: Wer die Abhängigkeit loswerden will, ersetzt `nfsu_video` durch eigenen Container-Parser plus Decoder; Prüfmaßstab bleibt `tools/movie_check.sh`.
