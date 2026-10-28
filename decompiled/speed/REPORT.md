# Abschlussbericht: Speed.exe (Need for Speed Underground)

**Status: Teilergebnis.** Bearbeitet sind der Einstieg, Ebene 1 und Ebene 2 unter dem Einstieg: 93 Funktionen, benannt, kommentiert und als C-Code in `src/`. Das Programm hat 10 398 Prozeduren. Die übrigen 10 292 sind nicht dekompiliert.

## Zweck und Plattform
- **Programm:** `Speed.exe`, Hauptprogramm des Rennspiels Need for Speed Underground. Hinweise: Ordner `Tracks/`, `Cars/`, `FrontEnd/`, Strings `LoadingFrontEnd`, `LoadingTrackCommon`, `UnloadingFrontEnd`, `NFSUNDER\ZDIR.BIN`.
- **Format:** PE32, Intel 386, GUI-Subsystem, Image Base 0x400000, Entry VA 0x664245.
- **Profil:** `pc-windows`.
- **SHA-256:** `1f72abe1e5c527c0a161e382d489605f483dd36c4ce7a2f4d561a0964c5db235`, identisch mit `uniws/Speed.exe`.

## Schutzprüfung
Ohne Befund. `.text` hat Entropie 6,74 (unter 7,5), der Einstieg liegt in `.text`, und es gibt keine Packer-Abschnitte. Die SafeDisc-Begleitdateien sind im Binary nicht belegt.

## Architekturübersicht
Der Ablauf ist ein statisch gelinkter MSVC-Start, der das Spiel aufruft:

```
crt_startup (0x664245, CRT-Stub)
 ├─ crt_cinit (0x664641)                     C-Initialisierung
 ├─ game_main (0x4448b0)                     Spiel-Hauptfunktion
 │   ├─ check_process_instances (0x4019f0)   Doppelstart-Sperre
 │   ├─ game_init_subsystems (0x4446f0)      Subsysteme, Pools, Archive, Frontend, Zufall
 │   │    └─ Ebene 2: init_memory_system, mount_game_archives, load_global_*, init_render_memory, …
 │   ├─ start_track_load (0x4ab860)          Streckenstart oder Entladen [nur bei gesetztem Flag]
 │   ├─ game_frame_loop (0x444bd0)           Hauptschleife
 │   │    └─ Ebene 2: update_frame_systems, online_manager_update, pump_window_messages, …
 │   └─ game_shutdown (0x410540)             Abbau
 │        └─ Ebene 2: release_*, free_list_*, shutdown_scene, shutdown_stream_buffers, …
 └─ crt_exit (0x664779)                      Exit-Wrapper
```

**Zustandsmaschine:** Die Hauptschleife führt einen Zustandszeiger (`g_state_fn`) aus. Jede Zustandsfunktion ersetzt sich durch den nächsten Zustand (`LoadingFrontEnd` → `LoadingTrackCommon` → `UnloadingFrontEnd`). Die Zustandsfunktionen selbst liegen außerhalb des Umfangs.

**Module in `src/`:**

| Datei | Inhalt |
|---|---|
| `startup.c` | Einstieg (Kommentar), Doppelstart-Sperre, `game_main` |
| `game.c` | Initialisierung, Hauptschleife, Abbau, Streckenstart, Globals des Ablaufs |
| `init.c` | Speicher, Partikel-Pools, Sitzungsindex, Zustandsblock, Frontend, a4d8-Subsystem |
| `frontend.c` | Frontend-Schleife und Datenträgerprüfung |
| `system.c` | Zufallszahlen, Ereignissystem, Listen, Versionsstring, Zeichenkettensync |
| `savestate.c` | Spielstand lesen und schreiben (31 Werte) |
| `assets.c` | Archive, Ladepakete, Pools für geladene Assets |
| `render.c` | Render-Speicher und Renderobjekt-Pools |
| `memory.c` | Speicherzuteilung mit Namen und Flags |
| `frame.c` | Pro-Bild-Aktualisierung, Online-Verwaltung, Frame-Zeit, Prüfsumme, HUD, Welt |
| `messages.c` | Win32-Nachrichtenpumpe, Warteschlange, Objektzeitgeber |
| `clock.c` | Frame-Zeit, Bildzähler, Renderzeit |
| `stream.c` | Ereignisliste mit dem Datenstrom, Zustandsfolge |
| `world.c` | Ladeliste, Empfänger, Fahrzeug, Kamerapfad, UI-Zähler, Hilfen |
| `util.c` | Pools, Zeichenketten, Wartezeit, zeitgesteuerte Rückrufe |
| `track.c` | Streckenplätze, Teilnehmer, Fahrzeugmodelle |
| `release.c` | Freigaben beim Abbau, Fahrzeug-Zustandsmaschine |
| `game.h`, `platform.h` | gemeinsame Typen, Globals, Prototypen, minimale Win32-Deklarationen |

## Umfang
| Kategorie | Anzahl |
|---|---|
| Prozeduren gesamt (REA-Überblick) | 10 398 |
| davon `FUN_…` vor der Bearbeitung | 8 707 |
| davon Externe (Imports) | 253 |
| **Bearbeitet (Umfang)** | **93**: Einstieg (1), Ebene 1 (7), Ebene 2 (84) |
| CRT-Stubs (nicht rekonstruiert) | 13 `FUN_…` (2 zusätzlich benannt: `crt_cinit`, `crt_exit`) |
| Außerhalb des Umfangs | 10 292 |

**Quote gesamt:** `FUN_…` 8 707 → 8 615. **Quote Umfang:** 93 von 93 Funktionen benannt und in `src/`. Im Umfang bleibt kein `FUN_…` außer den CRT-Stubs.

**Begründung der Grenze:** Tiefe 3 ab Einstieg wären mehrere Tausend Funktionen. Die Anleitung verlangt bei mehr als 300 eigenen Funktionen eine Einschränkung. Der Umfang ist deshalb auf Ebene 2 begrenzt.

## Was verifiziert wurde
- **PE-Kopf:** Machine 0x14c, PE32, Entry-RVA 0x264245, drei Abschnitte. Mit Python gelesen.
- **Hash:** Original und `uniws/Speed.exe` identisch.
- **Schutz:** Entropien und Abschnittsnamen geprüft.
- **CRT-Belege:** Strings `Microsoft Visual C++ Runtime Library`, `R6002`, `Runtime Error!` über `rea_search_strings`.
- **Aufrufbaum:** `rea_procedure_callees` für Einstieg, `game_main`, `game_init_subsystems`, `game_frame_loop`, `game_shutdown`, `start_track_load`.
- **Namensreadback:** Alle 93 Namen über die gespeicherten Antworten des Namensaufrufs geprüft. Bei `rea_set_address_name` und `rea_inspect_address_context` direkt bestätigt.
- **Stichprobe im Pseudo-C:** `rea_procedure_pseudo_code` für `game_main` zeigt die vergebenen Namen.
- **Syntaxprüfung:** `clang -fsyntax-only -Wall -Wextra -std=c11 src/*.c`: exit 0, keine Fehler und keine Warnungen.
- **Inventar:** REA-CLI `search` für Prozeduren und Strings nach `inventar/`, Prozedurzahl 10 398 mit dem Überblick übereinstimmend.

**Nicht verifiziert:** Ein lauffähiger Nachbau. Das Spiel ist ein Windows-GUI-Programm und wird nicht ausgeführt (Regel: keine Ausführung unbekannter Binaries). Die Listen-Funktionen rechnen mit 32-Bit-Zeigern und sind auf einem 64-Bit-Host nur syntaktisch geprüft.

## Grenzen der Werkzeuge
- **Kommentare:** Der Ghidra-Provider unterstützt `set_comment` nicht. Kommentare sind deshalb über `annotate_native_function` gesetzt. Dabei entstehen sehr große Antworten (bis 2,6 MB), die ausgelagert werden.
- **Sitzungsbindung:** Namen und Kommentare in Ghidra werden beim Schließen verworfen. Das dauerhafte Ergebnis steht in `NOTES.md` und `src/`.
- **Inventar:** Die CLI-Auswertung spiegelt die Umbenennungen nicht. Die Quoten stammen aus der laufenden REA-Sitzung.
- **Terminal-Aufrufe:** Ghidra meldet Terminal-Aufrufe ohne aufgelöstes Ziel bei `0x444921` und `0x410611`. Der Grund ist nicht belegt.
- **PDB:** `SpeedR.pdb` ist referenziert, aber nicht geladen.

## Offene Fragen und Unsicherheiten
1. **Aufrufe, die `FUN_…` heißen:** In `game.c` bleiben `FUN_004200e0` und `FUN_00420180` (Warteschlange, Objekt nicht passend typisiert) und `FUN_00501700` (= `render_scene`, Argument unklar).
2. **`start_track_load`:** Der Kopierblock aus ca. 50 Globals ist nicht aufgelöst.
3. **`game_frame_loop`:** Zeitscheiben und die Objektschleife bei `0x72ce08` sind ausgelassen.
4. **Vereinfachte Module:** `process_event_queue`, `sync_event_stream`, `update_car_dynamics`, `update_interpolators`, `render_scene` und die Listen-Funktionen in `track.c` sind nur teilweise übernommen. Die Kommentare geben die Stellen an.
5. **Strings:** CLI 5 572 gegen REA-Überblick 5 574. Nicht geklärt.
6. **`uniws.exe`:** eigenes Binary im Ordner, nicht Teil dieser Analyse.

## Nächster Schritt (Entscheidung nötig)
Ebene 3 umfasst über 10 000 Funktionen. Eine vollständige Dekompilierung ist in dieser Sitzung nicht machbar. Sinnvoll sind zwei Richtungen: entweder ein Subsystem nach Strings auswählen (z. B. Streckenladen über `Tracks`, Fahrzeugmodelle über `Cars`, Menü über `FrontEnd`), oder Ebene 3 unter den bekannten Modulen weiterführen. Bitte um Auswahl.


## Nachtrag: Ebene 3 (abgeschlossen)
- **Umfang:** 258 direkte Aufrufziele der Ebenen 1 und 2, Verteilung nach Modul in `NOTES.md`.
- **Ergebnis:** 66 exakte Übersetzungen, 177 Skizzen (benannt und dokumentiert, Logik unvollständig), 1 Rumpf, 9 aus Ebene 2 benannt, 3 Thunks und 2 CRT-Funktionen unbenannt.
- **Prüfung:** Syntax ohne Warnungen, keine doppelten Symbole, 437 offene Verweise (404 Ebene-4-Ziele, 33 Daten und Win32).
- **Wichtigste Einschränkung:** Die Skizzen sind keine rekonstruierte Logik. Sie sind eine ehrliche Gerüstebene: Namen, Zweck, Aufrufreihenfolge und die Zuordnung der Ebene-4-Ziele. Für Spielverhalten reicht das nicht.
- **Nächster Schritt:** Skizzen im Modul `frame` ausarbeiten, Vtable-Slots als Typen festlegen, dann Ebene 4.
