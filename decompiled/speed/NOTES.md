# Analyse-Log: Speed.exe (Need for Speed Underground)

## Status
- Original: `/Users/henryroth/Projekte/Recompilations/NFSU/Speed.exe`
- SHA-256: `1f72abe1e5c527c0a161e382d489605f483dd36c4ce7a2f4d561a0964c5db235` (identisch mit `uniws/Speed.exe`)
- Profil: `pc-windows` (PE32, i386, Machine 0x14c, Image Base 0x400000)
- Aktueller Schritt: Ebene 3 abgeschlossen (258 Ziele: 66 exakt, 177 Skizze, siehe unten). Nächster Schritt: Skizzen ausarbeiten, Ebene 4.
- Zuletzt erledigt: 93 Funktionen (Einstieg, Ebene 1, Ebene 2) benannt, kommentiert und in `src/` rekonstruiert. Syntaxprüfung über alle Module: exit 0, keine Warnungen.
- Nächster Schritt: Skizzen im Modul frame ausarbeiten, Vtable-Slots als Typen festlegen. Die Ghidra-Namen sind sitzungsgebunden, diese Datei und src/ sind das Gedächtnis.

## Überblick
- Format: PE32, Subsystem GUI, 3 Abschnitte, Entry RVA `0x264245` (VA `0x664245`, innerhalb `.text`)
- Ghidra-Sprache: x86 (auto), Analyse-Preset `ghidra-default`
- Segmente: `.text` 0x401000–0x688000 (2 650 112 B), `.rdata` 0x688000–0x6c6000, `.data` 0x6c6000–0x785d98, `tdb` 0xffdff000 (Ghidra-intern)
- Prozeduren laut REA-Überblick: 10 398, davon 8 707 mit Namen `FUN_…` (vor der Bearbeitung), 253 Externe (Imports)
- Strings: CLI-Inventar 5 572, REA-Überblick 5 574 (Abweichung von 2, nicht geklärt)
- PDB-Verweis im Binary: `SpeedR.pdb` (RSDS, GUID ae4553a5-e4cb-403f-ac1d-e827671ab0a6, Age 0x1b). Nicht geladen.
- Statische Bibliotheken: MSVC-CRT (Strings `Microsoft Visual C++ Runtime Library`, `R6002`, `Runtime Error!`)

## Schutzprüfung (Profil pc-windows, Schritt 1)
| Abschnitt | VA | Rohgröße | Entropie |
|---|---|---|---|
| .text | 0x1000 | 2 650 112 | 6,74 |
| .rdata | 0x288000 | 253 952 | 5,66 |
| .data | 0x2c6000 | 212 992 | 4,67 |

- Keine Packer-Abschnittsnamen (`stxt`, `UPX*`, `.aspack`, `.vmp*`, `.themida`, `.securom` usw.)
- `.text` unter 7,5, Einstieg in `.text`
- Begleitdateien `SECDRV.SYS`, `DrvMgt.dll`, `00000000.016/.256` gehören zu SafeDisc. Im Binary selbst fand ich dafür keinen Beleg (kein Abschnitt, keine Strings).
- Ergebnis: Kein Hinweis auf Packer oder verschlüsselten Code. Dekompilierung zulässig.

## Umfang
- **Festlegung:** Einstieg, Ebene 1 (direkte Aufrufe von `game_main`) und Ebene 2 (direkte Aufrufe der Ebene-1-Spielfunktionen). Tiefe 3 wäre mehrere Tausend Funktionen, weit über dem Limit von 300 aus der Anleitung.
- **Bearbeitet (93 Funktionen):** Einstieg `crt_startup` (Stub, dokumentiert), Ebene 1 (7), Ebene 2 (84). Alle benannt, kommentiert und in `src/` mit Code.
- **CRT-Stubs (nicht rekonstruiert):** 15 Aufrufziele des Einstiegs. Benannt sind `crt_cinit` und `crt_exit`. 13 bleiben `FUN_…` (siehe unten).
- **Außerhalb des Umfangs:** 10 292 Prozeduren (10 398 − 93 bearbeitet − 13 CRT). Darunter 253 Imports.
- **Quote gesamt:** `FUN_…` 8 707 → 8 615 (92 davon im Umfang umbenannt, 1,1 %). Einstieg `entry` ist ebenfalls benannt.
- **Quote Umfang:** 93 von 93 Funktionen benannt und in `src/`. Im Umfang bleibt kein `FUN_…` außer den 13 CRT-Stubs.

## Ebene 1 (Umfang)
| Originaladresse | Neuer Name | Datei | Sicherheit |
|---|---|---|---|
| 0x664245 | crt_startup (vorher `entry`) | `src/startup.c` (Kommentar, Stub) | wahrscheinlich |
| 0x4448b0 | game_main | `src/startup.c` | wahrscheinlich |
| 0x4019f0 | check_process_instances | `src/startup.c` | wahrscheinlich |
| 0x4446f0 | game_init_subsystems | `src/game.c` | wahrscheinlich |
| 0x444bd0 | game_frame_loop | `src/game.c` | wahrscheinlich, teilweise |
| 0x410540 | game_shutdown | `src/game.c` | wahrscheinlich |
| 0x4ab860 | start_track_load | `src/game.c` | wahrscheinlich, Globals unklar |
| 0x664641 | crt_cinit | nicht rekonstruiert (CRT) | sicher (Muster) |
| 0x664779 | crt_exit | nicht rekonstruiert (CRT) | wahrscheinlich |

## Ebene 2 (Umfang, 84 Funktionen)
| Modul (`src/`) | Funktionen (Originaladresse) |
|---|---|
| `init.c` | init_memory_system 0x401140, init_acid_particle_pools 0x402700, init_session_index 0x444570, online_buffer_init 0x4379d0 (Methode), init_global_state_block 0x4a0c80 (Methode), init_frontend_system 0x4e5180, init_a4d8_subsystem 0x5049b0 |
| `math.c` | reset_identity_matrix 0x40a7c0 |
| `frontend.c` | frontend_disc_check 0x401550 |
| `system.c` | init_rng_seed 0x444610, init_event_system 0x553af0, process_event_queue 0x555e50 (vereinfacht), register_list_node 0x549920, format_version_string 0x585c80, sync_string_buffer 0x559770, init_asset_cache 0x5789d0 |
| `savestate.c` | read_saved_state 0x5487d0, save_session_values 0x548f90 (31 Werte, Lesen und Schreiben nicht symmetrisch) |
| `assets.c` | mount_game_archives 0x4446a0, load_global_a_bundle 0x445b20, load_global_b_and_cars 0x445b70, init_loaded_asset_pools 0x566990 |
| `render.c` | init_render_memory 0x522170 |
| `memory.c` | mem_alloc_named 0x547c20 |
| `frame.c` | update_frame_systems 0x4097d0, update_mode_from_host_state 0x42bc80, update_all_instances 0x42f8c0, is_online_busy 0x437b10, online_pump_a 0x437fc0, online_pump_b 0x437fe0, online_manager_update 0x437b90 (Debug-String "OnlineManager::Update"), update_frame_pacing 0x438000, update_timed_reset 0x442350, update_frame_timing 0x444930, sync_frame_checksum 0x444a70, update_hud_panels 0x41fda0, update_and_draw_world 0x41f260 |
| `messages.c` | pump_window_messages 0x40fda0, tick_object_timer 0x41e810, object_time_step 0x41e880, dispatch_queued_messages 0x4200e0, post_queued_message 0x420180 |
| `clock.c` | update_frame_delta 0x561d30, advance_frame_counter 0x561e80, update_render_clock 0x561f10 |
| `stream.c` | sync_event_stream 0x5535f0, stream_write_block 0x5700a0, advance_stream_state 0x552050 |
| `world.c` | process_pending_loads 0x445910, broadcast_frame_event 0x4757b0, update_car_dynamics 0x47a550, update_speedcam_and_server 0x4862d0, update_ui_systems 0x4e5210, render_scene 0x501700, update_effect_slot 0x551a10, update_effect_timer 0x552000, update_camera_path 0x554780, update_interpolators 0x5545c0, noop_hook 0x58a1c0, init_bpnode_pool 0x598240 |
| `util.c` | slot_pool_create 0x599c50 (Ghidra FUN_00599c50, Alias gesetzt), str_copy 0x59aad0, str_copy_n 0x59ab00, sleep_alertable 0x631b80, run_timed_callbacks 0x632660, init_space_node_pool 0x59c400 |
| `track.c` | reset_track_slot 0x416380, add_track_entry 0x430a20, copy_track_slot 0x430b70, request_online_state 0x4382d0, prepare_track_entries 0x43b450, select_car_model 0x4a19b0, assign_track_slots 0x4a8d30, build_dummy_participants 0x4a92b0, swap_track_slot_flag 0x4ab7d0 |
| `release.c` | release_menu_objects 0x404b90, free_list_0077f5c0 0x407510, free_list_0077d760 0x40bbc0, release_global_objects 0x40a690, release_misc_objects 0x40bdb0, free_buffers_0072b03c 0x41af20, shutdown_scene 0x501540, update_car_state_machine 0x51be40, shutdown_stream_buffers 0x56fd70 |

## Offene Punkte
- **Aufrufe in `game.c`, die noch `FUN_…` heißen:** `FUN_004200e0` und `FUN_00420180` (Ghidra-Namen von `dispatch_queued_messages`/`post_queued_message`). Ihr Objekt (`g_host`) passt nicht zum Warteschlangen-Typ, deshalb unverändert. `FUN_00501700` (= `render_scene`, Argument unklar). `FUN_0040dc90`, `FUN_004e…` und weitere Ebene-3-Aufrufe.
- **`game_init_subsystems`:** `FUN_00401550(0)` am Ende ist mit `frontend_disc_check()` benannt, das Argument ist ungeklärt. Initialwerte im Bereich `0x780470–0x78048e` sind nicht übernommen.
- **`game_frame_loop`:** Zeitscheiben (`DAT_007295dc/ec`) und die Objektschleife bei `DAT_0072ce08` sind ausgelassen. `frame_mode = 0` (Zweig in `FUN_00552000`) ist unklar.
- **`start_track_load`:** Der Kopierblock aus ca. 50 Globals ist nicht aufgelöst, der Schleifenkörper ist vereinfacht.
- **`process_event_queue` und `sync_event_stream`:** Listen-Zeigerarithmetik vereinfacht, Ghidra-Zeichenfolge nicht vollständig übernommen.
- **`update_car_dynamics`, `update_interpolators`, `render_scene`:** Felder und Zustände teilweise unbenannt, Berechnungen vereinfacht.
- **Listen mit 32-Bit-Zeigern (`free_list_*`, `release_*`, `register_list_node`):** Auf einem 64-Bit-Host nur Syntaxprüfung, kein lauffähiger Nachbau.
- **CRT-Stubs:** 13 `FUN_…` (0x6641fc … 0x6691bf, 0x6646a6, 0x66478a) unbenannt und nicht einzeln geprüft.
- **Parameter:** `game_main` und `game_init_subsystems` erhalten im Aufruf zwei Parameter, die nicht genutzt sind.
- **Terminal-Aufrufe ohne Ziel:** `0x444921` (in `game_main`) und `0x410611` (in `shutdown` über `FUN_0056fd70`). Der Grund (No-Return) ist nicht belegt.
- **Strings-Abweichung:** 5 572 (CLI) gegen 5 574 (REA-Überblick). Nicht geklärt.
- **Inventar nach Umbenennung:** Das CLI-Inventar spiegelt die Session-Umbenennungen nicht. Die Quoten oben stammen aus der laufenden REA-Sitzung.
- **`uniws.exe`:** eigenes Binary im Ordner, nicht Teil dieser Analyse.

## REA-Abdeckung (ca.)
| Werkzeug | Aufrufe | Wofür |
|---|---|---|
| rea_open_binary / rea_binary_overview / rea_procedure_info | je 1–2 | Öffnen, Überblick, Einstieg |
| rea_procedure_callees | ca. 6 | Aufrufbaum (Einstieg, Ebene 1) |
| rea_batch_decompile | ca. 25 Aufrufe, je 3 Funktionen | Ebenen 1 und 2 |
| rea_annotate_native_function | ca. 95 | Namen und Kommentare (Ausgaben ausgelagert, bis 2,6 MB pro Aufruf) |
| rea_set_address_name | 3 | Namen |
| rea_set_comment | 3 | nicht unterstützt (Provider `ghidra`) |
| rea_inspect_address_context | 3 | Readback der Namen |
| rea_search_strings | 1 | CRT-Belege |
| REA-CLI (`search`, Snapshot) | 2 | Inventar nach `inventar/` |

Hinweis: Die Ghidra-Namen und -Kommentare sind sitzungsgebunden und werden beim Schließen verworfen. Der Snapshot enthält nur die Analyse, keine Umbenennungen.


## Ebene 3 (Stand: abgeschlossen, Stufen A und B)
**Umfang:** 258 direkte Aufrufziele der 90 Funktionen aus Ebene 1 und 2. Ausgeschlossen sind Imports (`EXTERNAL:`), der CRT-Bereich ab `0x640000` (9 Ziele) und die Thunks. Aufrufbaum: `inventar/level2_callees.txt`. Zuordnung nach Modul: `inventar/ebene3_by_module.txt`. Fortschritt je Adresse: `inventar/ebene3_progress.txt` (Hilfsskript `inventar/progress.py`).

**Ergebnis je Status:**

| Status | Anzahl | Bedeutung |
|---|---|---|
| exakt | 66 | Übersetzung ohne Auslassung (einzelne Hilfswerte wie Register-Argumente sind als Parameter angegeben) |
| Skizze | 177 | Name, Zweck und Aufrufreihenfolge aus dem Dekompilat, Logik unvollständig oder leer |
| Rumpf | 1 | `stream_read_bits` (0x559600): Dekompilat verliert die Zuweisungen, liefert 0 |
| bereits aus Ebene 2 benannt | 9 | Stufe A, siehe unten |
| Thunk, nicht benannt | 3 | `0x40ae20`, `0x58a570` (thunk_FUN_0058a300), `0x59f3cf` (indirekter Sprung) |
| CRT, nicht rekonstruiert | 2 | `0x59e170` (_malloc), `0x59e180` (_free) |

**Stufe A (9 benannt, vollständig in `src/`):** `pool_init_block` (0x599aa0), `pool_release_node` (0x599a60), `pool_take_node` (0x599cb0), `mem_copy_block` (0x62f850), `timer_get_ticks` (0x546d70), `stream_write_bits` (0x559530), `event_table_push` (0x553570), `event_list_append` (0x552550), `ui_menu_shutdown` (0x4e9090).

**Modulzuordnung (Ebene 3):** frame 103, track 40, world 37, init 18, assets 16, release 11, system 7, frontend 6, messages 5, stream 5, render 4, clock 2, memory 2, savestate 1, util 1. Die Funktionen liegen in den Dateien ihres Aufrufers. Einige gemeinsame Hilfsfunktionen stehen in `track.c` (`0x40bc40`, `0x401b70`, `0x59aa50`, `0x59ac70`), weil sie dort zuerst gebraucht wurden.

**Prüfung:** `clang -fsyntax-only -Wall -Wextra -std=c11 src/*.c` ohne Ausgabe (exit 0). Objekte einzeln übersetzt: keine doppelten Symbole. Unaufgelöste Verweise: 437, davon 404 `FUN_`-Ziele der Ebene 4 und etwa 33 Daten- und Win32-Namen (`DAT_*`, `LAB_*`, `QueryPerformanceFrequency`, `__memcpy_chk`). Das Programm ist nicht gelinkt.

**Bekannte Lücken:**
- Viele Skizzen haben leere Körper oder nur Aufrufe. Sie sind benannt und dokumentiert, aber nicht verhaltensgleich. Der Kommentar nennt jeweils, was fehlt.
- Vtable-Aufrufe (Slots 0x24 bis 0x110) sind in den meisten Funktionen nicht nachgebildet.
- Register-Argumente (EAX, ECX, EDI, ESI) stehen nicht in allen Dekompilaten; die Parameter sind dort nach Aufrufern abgeleitet und mit Kommentar markiert.
- Einige Entscheidungen sind Annahmen: `frontend_disc_poll` liefert 1, damit die Schleife in `frontend_disc_check` endet. `FUN_005473c0` ignoriert ein Nullobjekt (`init.c`, `memory.c` rufen es mit 0 auf). `timer_init` liefert 0 ohne vollständige Initialisierung.
- Die Rückgabe von `stream_read_bits` (0) und `stream_slot_get` (0) ist nicht das Originalverhalten. Ein Ladevorgang über diese Pfade ist damit nicht korrekt.
- Namen in Ghidra sind sitzungsgebunden. Ghidra wurde für diese Stufe neu geöffnet, die Namen stehen nur in `src/`.

**Nächster Schritt:** Die Skizzen Modul für Modul ausarbeiten, beginnend mit `frame`, weil dort die Zustandsmaschine und die Vtable-Aufrufe konzentriert sind. Die Vtable-Slots zuerst als eigene Typen in `game.h` festlegen. Danach Ebene 4 für die Funktionen mit den meisten Aufrufen.

