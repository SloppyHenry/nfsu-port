# WP-15 Audioformate (Streams und Soundbänke)

**Phase 2 · Modell: Sonnet · Umfang: 1–2 Sessions · Voraussetzungen: WP-10**

## Befund
`SNDSTREAMS/*.ast` und `SpeechStreams/*.ast` sind EA-SCHl-Streams mit EA-XA-ADPCM (`adpcm_ea_r2`, 22 050 Hz). FFmpeg dekodiert sie. Die Soundbänke in `Sound/*` haben ein unbekanntes Format.

## Schritte
1. **Eigener Decoder** für SCHl/SCCl/SCDl/SCEl und EA-XA R2 nach `port/src/formats/ea_audio.{h,cpp}`. Eigener Code statt libavcodec, damit das `.app` später ohne FFmpeg-Abhängigkeit und ohne Lizenzfragen auskommt.
2. Prüfen: **bitgleich** mit `ffmpeg -i x.ast -f s16le` auf allen Streams.
3. **Soundbänke**:
   - Format bestimmen (Kopf, Tabellen, Sample-Formate)
   - Ladecode im Export suchen (Strings `SNDSYS_*`, `EAXS_*`, `Csis SlotPools`)
   - Ergebnis nach `plan/FAKTEN.md` und `symbols/types.h`
4. `nfsu-tool snd <datei> <aus>` exportiert WAV. Stichproben anhören (Motor, Nitro, Menü).
5. Die Struktur der Streams aufschreiben: Spuren und Unterstreams in `STRM_Music_UG0_MB.ast` (1:45 h, mehrere Titel?), Sprachdateien je Sprache.

## Fertig, wenn
- Der Decoder ist bitgleich zu FFmpeg auf allen `.ast`-Dateien.
- Alle Soundbänke lassen sich exportieren, oder das ungelöste Teilformat ist genau beschrieben.
