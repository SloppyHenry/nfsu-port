# WP-13 Texturpakete

**Phase 2 · Modell: Sonnet · Umfang: 1 Session · Voraussetzungen: WP-12**

## Ziel
Alle Texturen aus Texturpaketen (`TEXTURES.BIN`, `TEX*TRACK.BIN`, `*.BUN`, `HUDTEX.BIN` …) werden gelesen und als PNG exportiert.

## Schritte
1. Den Ladecode der Texturpakete im Export finden (Handler aus `chunks.csv`). Kopf, Hash- bzw. Namenstabelle und TextureInfo-Struktur mit Feldnamen nach `symbols/types.h`.
2. Formate: DXT1/3/5, unkomprimiert (ARGB8888, 565, 4444), Palette (P8), Mipmaps. Im Original zu `D3DFORMAT` zuordnen.
3. `port/src/formats/texture_pack.{h,cpp}`, dazu `nfsu-tool tex <datei> <ausgabeordner>` (PNG, z. B. über stb_image_write; Einbindung als Einzelheader nach Rückfrage, oder eigener minimaler PNG-Schreiber).
4. Kontaktbogen einer Strecke und eines Autos rendern und prüfen.

## Fertig, wenn
- Mehr als 99 % der Texturen werden exportiert, die übrigen stehen mit Grund im Log.
- Die TextureInfo-Struktur ist in `types.h` (mit `static_assert`) und hält sich an ADR-001.
