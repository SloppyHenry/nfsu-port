# WP-16 Videos (EA MAD)

**Phase 2 · Modell: Sonnet · Umfang: 1 Session · Voraussetzungen: WP-10, ideal WP-15 (Tonspur)**

## Befund
`Movies/*.mad` sind EA MAD (Madcow), 512×256, 30,3 fps, Ton als EA-XA-ADPCM. FFmpeg dekodiert sie (`eamad`). Im Original steckt der EA-RCMP/AV-Player (`src/av/cmn/avplayer.cpp`, `rcmp_mad_codec.cpp`, Untertitel `avsubtitle.cpp`).

## Schritte
1. Entscheidung im ADR `port/docs/ADR-002-video.md`:
   - (a) eigener MAD-Decoder (MPEG-1-ähnliche Intra-Blöcke plus Bewegung, mittlere Komplexität)
   - (b) libavcodec dynamisch einbinden (LGPL)
   Empfehlung: (a), wenn der Aufwand ≤ 2 Sessions ist, sonst (b).
2. Abspieler in `port/src/platform/video.*`: YUV → Textur (SDL_GPU), Ton über die Audioausgabe, Abbruch per Taste.
3. Untertitel: Format der Untertitel des AV-Players aus dem Original klären (avsubtitle). Falls es sie gibt, anzeigen.
4. `nfsu-tool play Movies/01_DAY.mad` (relativ zum Datenpfad `game/pc`) zum Test.

## Fertig, wenn
- Alle Filme laufen mit synchronem Ton, und bei (a) ist der Decoder gegen FFmpeg-Einzelbilder geprüft (PSNR > 50 dB oder bitgleich).
