# WP-18 Strecken-Viewer (erster sichtbarer Meilenstein)

**Phase 2 · Modell: Opus · Umfang: 1–2 Sessions · Voraussetzungen: WP-13, WP-14, WP-17 (mind. world.efx)**

## Ziel
`apps/viewer` lädt eine Strecke mit Geometrie und Texturen aus den Originaldateien und zeigt sie auf dem Mac mit freier Kamera.

## Schritte
1. Klären, welche Dateien zu einer Strecke gehören: `TRACKB<id>.lzc`, `STREAML<x>.BUN` bzw. `_<id>.BUN` und `TEX<id>TRACK.BIN`. Die Logik steht im Original in `start_track_load` (0x4ab860) und den zugehörigen Ladefunktionen.
2. Streaming-Abschnitte zunächst alle auf einmal laden. Echtes Streaming kommt in Phase 3.
3. Rendern mit SDL_GPU (Metal), Kamera mit WASD und Maus, Abschnitte ein- und ausblenden.
4. Bildschirmfoto nach `refs/screens/viewer-<strecke>.png`, Beschreibung im Log.

## Fertig, wenn
- Mindestens drei verschiedene Strecken werden korrekt texturiert angezeigt.
- Bekannte Fehler (Transparenz, Beleuchtung, Himmel) sind im Log aufgelistet.
