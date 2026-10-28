# ADR-001: 64-Bit-Strategie für geladene Daten

Status: angenommen (WP-11, 2026-10-21)

## Kontext
Das Original ist 32-Bit-Code und lädt Chunks als Rohpuffer. Die Lader (`Loader…(bChunk*)`) rufen sich über
`TraverseChunks` auf, vertauschen auf GC die Byte-Reihenfolge und **setzen Zeiger in den Puffer oder auf ihn**.
Unter arm64 sind Zeiger 8 Byte groß; Strukturlayouts mit 4-Byte-Zeigerfeldern passen nicht mehr.

## Befund (am PC geprüft)
Es gibt drei Arten von Zeigern, nicht eine:

1. **Zeiger auf Chunkdaten, abgelegt in Laufzeitobjekten** (der überwiegende Fall). Beispiele: `TrackStreamer::Loader`
   (`this+0 = chunk+1`), `TopologyTree::Loader` (`pNodeTable`, `pSquareTable`). Die Datenstruktur im Chunk enthält
   Indizes, keine Zeiger. Das Laufzeitobjekt liegt außerhalb der Datei, also ist ein nativer 64-Bit-Zeiger kein Problem.
2. **Reservierte Zeigerfelder in dateigebundenen Strukturen, in der Datei 0, vom Lader gefüllt.**
   `TextureInfo` (Chunk 0x33310004, 0x7c Byte): `PlatInfo`, `bNode::Next/Prev` (+0..+8) und `pTexturePack`,
   `pImageParent`, `ImageData`, `PaletteData` (+0x68..+0x74). In allen 1 820 TextureInfo der Auto-Texturpakete
   (`Cars/*/TEXTURES.BIN`) sind diese 7 Felder 0. `FUN_00529910` (`TexturePack::AttachTextureTable`) füllt sie.
   Ebenso `eSolidListHeader` (Chunk 0x134002): `bNode` und `SolidIndexEntryTable`/`SolidStreamEntryTable`
   (+0x78/+0x7c), gesetzt in `InternalLoaderSolidHeaderChunks` (0x523f30). Auch `TexturePackHeader` (0x33310001).
3. **Intrusive Listen** (`bNode`/`bList`): Knoten stecken in den Strukturen aus 2.

Die Zeiger stehen also nie fertig in der Datei. Eine Relozierung („in place fixup“ von Dateioffsets) kommt in den
geprüften Chunks nicht vor. Die Strukturen müssen nur ihre **Größe und ihre Feldoffsets** behalten, weil Lader
Tabellen mit `size / 0x7c` zerlegen und Felder per Offset lesen.

## Optionen
| | Bewertung |
|---|---|
| (a) Deserialisieren | sicher, aber doppelter Speicher und Abweichung vom Ablauf; nur sinnvoll, wo Layout frei sein darf |
| **(b) `ptr32<T>`** | Layout bleibt gleich, `static_assert` prüft es. Prototyp besteht (`port/src/core/ptr32.hpp`, `formats/texinfo.hpp`, `tests/test_ptr32.cpp`) |
| (c) Speicher unter 4 GiB | **nicht machbar**: `mmap` mit `MAP_FIXED` unter 4 GiB scheitert (Pagezero 4 GiB), mit `-Wl,-pagezero_size,0x4000` wird das Programm beendet (Exit 137). Geprüft 2026-10-21 |

## Entscheidung
- **Dateigebundene Strukturen** (Layout durch Datei oder Lader-Aufteilung festgelegt): `#pragma pack`, `ptr32<T>` für
  jedes Zeigerfeld, `static_assert` auf `sizeof` und die Offsets der Felder, die Lader oder Code ansprechen. Namen der
  Felder wie im GC-Typ.
- **`ptr32<T>`** = 32-Bit-Abstand zu `g_arena.base`, 0 ist null. Alles, worauf ein `ptr32` zeigt, muss in der Arena
  liegen (Chunkpuffer, Pool-Allokationen). Die Arena ist ein einziger `mmap`-Bereich unter 4 GiB Größe (Original
  hatte einen Speicherpool, ca. 32–64 MB).
- **Zeiger auf Dinge außerhalb der Arena** (Texturobjekte der GPU, SDL-Handles, `TexturePack`-Laufzeitobjekte) gehören
  nie in ein `ptr32`-Feld. Stattdessen: Handle- oder Indexfeld (`uint32_t`) in der Struktur, native Tabelle daneben.
  Die Felder `PlatInfo`, `ImageData` usw. behalten die 4 Byte, werden aber als Index in eine native Tabelle benutzt.
- **Reine Laufzeitobjekte** (nicht in Dateien, nicht über `size / N` zerlegt): native Zeiger, freies Layout.
  Das ist die Vorgabe für Objekte wie Auto, Fahrer, Renderkontext.
- **`bNode`/`bList`**: zwei Varianten. Knoten in dateigebundenen Strukturen: `bNode32 { ptr32<bNode32> next, prev; }`,
  Elemente liegen in der Arena. Knoten in Laufzeitobjekten: `bNode` mit nativen Zeigern. Die Listen-API ist dieselbe
  Vorlage `bTList<T, NodeT>`.
- **Chunk-Lader** behalten die Signatur `bool Loader(bChunk*)` und lesen den Puffer weiter in place. Der Puffer wird in
  die Arena geladen, nicht in separate `malloc`-Blöcke.
- **Endianness**: PC-Daten sind little endian wie arm64, es gibt keine Vertauschung (GC-Lader tauschen, PC nicht).
- Was Option (a) rechtfertigt: Strukturen, die das Original in der Datei in einem anderen Format hat als im Speicher
  (Vertex-Puffer, Streifen). Diese werden beim Laden in GPU-Puffer umgesetzt.

## Folgen
- WP-12 trägt in `symbols/chunks.csv`, Spalte `zeiger`, je Chunk-ID 0 (keine), `leer` (Slots, Lader füllt) oder `index`
  (Lader speichert Zeiger auf Chunkdaten) ein. Bisher bekannt: `leer` für 0x33310001, 0x33310004, 0x134002.
- Die Arena braucht eine Obergrenze (Fehler statt stilles Überlaufen bei `off` > 4 GiB), sie wird in WP-30 (bWare-Pool) angelegt.
- Offen: ob Solid-Objektchunks (0x134010) und Streckenabschnitte weitere Zeiger enthalten. Prüfung in WP-14.
