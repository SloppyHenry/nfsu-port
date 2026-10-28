# WP-00 Repository

**Phase 0 · Modell: Sonnet · Umfang: klein (< 1 Session) · Voraussetzungen: keine**

## Ausgangslage (aufgeräumt am 2026-10-20)
```
NFSU/
  CLAUDE.md      Session-Regeln
  plan/          Roadmap, Status, Pakete, Logs, Fakten
  game/pc/       PC-Spieldaten + Speed.exe (nur lesen)
  game/gc/       GameCube-ISO (nur lesen)
  decompiled/    full/ (Ghidra-Vollexport, Skripte), speed/ (frühere Handarbeit)
  symbols/ tools/ port/ refs/   leer, je mit README
```

## Ziel
Ein Git-Repository über `NFSU/`, das nur Projektarbeit versioniert, nicht die Spieldaten und nicht die erzeugten Exporte.

## Schritte
1. `git init` in `/Users/henryroth/Projekte/Recompilations/NFSU`.
2. `.gitignore`:
   ```
   .DS_Store
   /game/
   /refs/*
   !/refs/README.md
   /decompiled/full/out*/
   /decompiled/full/build*/
   /decompiled/full/project*/
   /decompiled/full/*.log
   /decompiled/gc/project*/
   /decompiled/gc/out*/
   /decompiled/speed/inventar/snapshot/
   /port/build*/
   ```
3. Die Größe der übrigen Dateien prüfen: `decompiled/speed/inventar/*.json` sind evtl. mehrere MB. Über 1 MB nicht committen, stattdessen ignorieren und im Log vermerken.
4. Prüfen: `git status --ignored` zeigt keine Spieldatei (`*.BUN`, `*.BIN`, `*.lzc`, `*.exe`, `*.mad`, `*.ast`, `*.iso`, `*.elf`) als versioniert.
5. Erster Commit „Projektstruktur und Planung“.

## Fertig, wenn
- `git ls-files` enthält keine Spieldatei und keine Datei über 1 MB.
- `plan/STATUS.md` steht auf WP-00 `erledigt`.

## Ergebnis (2026-10-20)
Erledigt. Remote: privates GitHub-Repo `SloppyHenry/NFSU` (auf Wunsch des Nutzers). Die tatsächliche `.gitignore` liegt in der Wurzel. Zusätzlich ausgeschlossen sind `decompiled/speed/inventar/*.json`, weil sie aus der Exe extrahierten Text enthalten und sich neu erzeugen lassen.
