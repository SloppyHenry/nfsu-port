# WP-14 Geometrie (Solids, Meshes, Materialien)

**Phase 2 · Modell: Opus · Umfang: 1–2 Sessions · Voraussetzungen: WP-12, WP-13**

> **Hinweis für die Fortsetzung (Strecken, Recherche 2026-10-27):** Die 0x134xxx-Namen aus WP-14 stimmen mit `refs/nfsmw/symbols/bchunks.txt` überein (`MeshVertexBuffer`, `MeshShaderInfos` = StripEntry, `MeshPolygons` …). Für die offenen Punkte:
> - **Streckentopologie:** nfsmw benennt die Familie 0x34xxx: `0x34101–0x34103` ScenerySectionHeader/SceneryInfos/SceneryInstances (Platzierung der Solids, „Objekt-Transforms“), `0x34107/0x34108` Preculler/SceneryOverride, `0x34151–0x34153` VisibleSection (ManageInfo, Boundaries, DrivableScenerySections), `0x34146` TrackPositionMarkers, `0x3414a` TrackPathZones. Unbenannt bleiben `0x34121–0x34134` (Kandidat für Topologie/Kollision). Einstieg über die GC-Namen `LoaderTrackGeometry`, `LoaderCollisionVolumes`, `LoaderScenery` und `LoaderVisibleSections` (`gc_functions.csv` → `pc_gc_map.csv`).
> - **Drittes Vertexformat (~1 000 übersprungene Streckensolids):** Den Stride aus dem `MeshShaderInfos`-Eintrag bzw. der Vertexdeklaration lesen statt raten. Als Gegenprobe taugt OpenUG2 (`whoismept/OpenUG2`, UG2-Nachbau, liest STREAM*.BUN). Dessen Layouts gelten aber für UG2, nicht für NFSU.
> - Die Namen trägt WP-03 in `chunks.csv` ein (Spalte `name_mw`). Die Layouts kommen weiter nur aus NFSU (siehe die eigene `eSolid`-Prüfung).

## Ziel
Auto- und Streckengeometrie wird gelesen und als glTF mit Texturen exportiert.

## Schritte
1. Die Handler der Geometrie-Chunks finden (Solid-Listen, Objekte, Vertexdaten, Indizes, Materialien bzw. Texturverweise über Hashes). Namen und Layout mit nfsmw (eSolid/eModel) abgleichen.
2. Vertexformate bestimmen: Position, Normale, Farbe, UV, Gewichte. Dabei auf die Deklarationen bzw. FVF-Werte achten, die der Renderer an D3D9 übergibt.
3. `port/src/formats/geometry.{h,cpp}` und `nfsu-tool mesh <datei> <aus.gltf>`.
4. Prüfen: Ein Auto (`game/pc/Cars/350Z/GEOMETRY.BIN`) und ein Streckenabschnitt werden exportiert und im Blender-Import kontrolliert. Wenn das Blender-MCP verbunden ist, kann man es darüber ansehen.

## Fertig, wenn
- Alle Autos und alle Streckenpakete werden ohne Fehler exportiert.
- Zwei Beispiele sind im Log mit Bildern bzw. Beschreibung belegt.
