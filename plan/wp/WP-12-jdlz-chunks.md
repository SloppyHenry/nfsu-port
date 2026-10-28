# WP-12 JDLZ-Dekompression und Chunk-Container

**Phase 2 · Modell: Sonnet · Umfang: 1–2 Sessions · Voraussetzungen: WP-10, ideal WP-05 und WP-01**

> **Hinweis für die Fortsetzung (Recherche 2026-10-27):** `refs/nfsmw/symbols/bchunks.txt` ordnet Chunk-IDs Namen zu. Von den 92 IDs in `chunks.csv`, die noch keinen Namen haben, sind dort **68 benannt**, darunter `0x30201` FEngFont, `0x34101–0x34108` Scenery, `0x34146/0x3414a` TrackPositionMarkers/TrackPathZones, `0x34151–0x34153` VisibleSection, `0x34201` TrackInfos, `0x34600/0x34601` CarTypeInfos/CarSkins, `0x34a0x` GCareer, `0x35001–0x35101` Lichtquellen, `0x36001–0x36003` EventTrigger und `0x39000` STRBlocks. Die 24 schon benannten IDs (WP-13, WP-14) stimmen inhaltlich alle mit nfsmw überein. Ohne Namen bleiben 24 IDs, u. a. `0x34121–0x34134`, `0x34035/36`, `0x39200–0x39202`, `0x3a000`, `0x3b300` und `0x80034020`. Die Namen trägt WP-03 als Spalte `name_mw` ein. Diese Session sucht danach die Handler gezielt über die Namen: `gc_functions.csv` enthält 57 Funktionen `Loader…(bChunk *)`, z. B. `LoaderScenery`, `LoaderVisibleSections`, `LoaderTrackInfo`, `LoaderTrackGeometry`, `LoaderCollisionVolumes`. Über `pc_gc_map.csv` kommt man von dort zur PC-Adresse. Das ist schneller als die Suche über Konstanten. Die Layouts kommen trotzdem aus NFSU (`gc_types.h`), siehe WP-03, „Grenze“.

## Ziel
Alle Spieldateien lassen sich entpacken und als Chunk-Baum lesen. Jede Chunk-ID ist katalogisiert, mit der Codestelle, die sie verarbeitet.

## Schritte
1. **JDLZ**:
   - Originalfunktion finden (Konstante 0x5A4C444A = `'JDLZ'` in `consts.tsv`, oder die Aufrufer der `.lzc`-Pfadstrings)
   - nach `port/src/formats/jdlz.cpp` rekonstruieren
   - Kopf dokumentieren: Magic, Version `02 10`, entpackte und gepackte Länge
   - Nach WP-05 gegen das Original im Emulator auf allen `.lzc`-Dateien prüfen, sonst auf Plausibilität (Längen, Chunk-Struktur danach)
2. **Chunks**:
   - Kopf `uint32 id, uint32 size`; prüfen, ob das hohe Bit einen Container kennzeichnet
   - Ausrichtungs- und Füllchunks (z. B. ID 0) erkennen
   - Parser `port/src/formats/chunk.{h,cpp}`, nur lesend und mit Grenzprüfung
3. **Werkzeug**: `nfsu-tool chunks <datei> [--depth N]` gibt den Baum aus. `nfsu-tool unpack <datei.lzc> <aus>` entpackt.
4. **Katalog** `symbols/chunks.csv`: `id,name,dateien,anzahl,min_size,max_size,container,handler_addr,zeiger,note`
   - Quellen: alle `.BUN`, `.BIN` und entpackten `.lzc`-Dateien, die Konstanten in `consts.tsv` (Funktion ↔ ID), Namen aus nfsmw (WP-03), falls vorhanden
5. Tests: Jede Datei im Spielordner wird ohne Fehler und ohne Restbytes geparst.

## Fertig, wenn
- 100 % der Dateien parsen, und alle `.lzc`-Dateien entpacken (Längen stimmen).
- `chunks.csv` hat für jede ID einen `handler_addr` oder den Eintrag „nicht gefunden“.
- Der JDLZ-Vergleich gegen das Original ist bestanden, sobald WP-05 fertig ist (sonst als offen vermerken).
