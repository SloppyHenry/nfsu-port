# Geprüfte Fakten (Stand 2026-10-20)

Jede Zeile ist nachgeprüft. Vermutungen stehen nur in `ROADMAP.md` und sind dort so markiert.
Neue Fakten hier mit Datum und Quelle (Befehl oder Datei) ergänzen.

## Binary
| Punkt | Wert | Quelle |
|---|---|---|
| Datei | `Speed.exe`, PE32 i386, GUI, Image Base 0x400000, Entry 0x664245 | `decompiled/speed/REPORT.md` |
| SHA-256 | `1f72abe1e5c527c0a161e382d489605f483dd36c4ce7a2f4d561a0964c5db235` | ebd. |
| Abschnitte | `.text` 0x401000–0x688000, `.rdata` 0x688000–0x6c6000, `.data` 0x6c6000–0x785d98 | ebd. |
| Compiler | MSVC, Visual Studio 2003 (Ghidra-FID: 200× „Visual Studio 2003 Release“) | `out2/all_functions.c` |
| PDB | `SpeedR.pdb` referenziert, nicht vorhanden | REPORT.md |
| RTTI | keine `.?AV`-Strings, also C++ ohne RTTI; Klassen nur über Vtables/Konstruktoren erkennbar | `strings.json` |
| Schutz | kein Packer, keine Verschlüsselung (Entropie .text 6,74) | REPORT.md |
| Funktionen | 10 378 im Export 2 (`decompiled/full/out2`), 1 Fehlschlag (0x666f5d) | `HANDOVER.md` |
| Konventionen | stdcall 5 001, fastcall 1 947, thiscall 1 779, cdecl 1 617, unknown 34 | `out2/functions.tsv` |
| Registerartefakte | 11 485 `in_E?X`, 17 723 `unaff_*` im Rohdekompilat | `HANDOVER.md` |

## Verteilung im Code (Funktionen je 64-KB-Block, aus `out2/functions.tsv`)
- 0x401000–0x5a0000: ca. 6 200 Funktionen, fast keine Bibliothekstreffer → **überwiegend Spielcode**.
- 0x5a0000–0x610000: D3DX-Effektcompiler-Strings (45 Funktionen mit Bezug), RCMP/AV-Player-Anteile → **Bibliotheken gemischt mit Engine**.
- 0x630000–0x688000: RCMP (20 Funktionen bei 0x63xxxx), MSVC-CRT (0x660000–0x680000: 1 387 von 1 666 Funktionen per FID erkannt).
- Statisch gelinkt nachgewiesen: MSVC-CRT, D3DX9-Effekt-/Shadercompiler, zlib „inflate 1.1.4“, EA RCMP/AV-Player (`src/av/cmn/avplayer.cpp` u. a.), EA-Sound (`SNDSYS_inited()`, `EAXS_*Stream`, `Csis SlotPools`).
- Genaue Modulgrenzen: **offen** → WP-02.

## Importe (DLLs)
D3D9, DDRAW, DINPUT8, DINPUT, DSOUND, WINMM, USER32, GDI32, KERNEL32, ADVAPI32, SHELL32, SHFOLDER, WS2_32, TAPI32, NETAPI32 (`decompiled/full/run.log`).

## Spieldaten PC (`game/pc/`, nur lesen; Pfade unten relativ dazu)
| Format | Befund | Quelle |
|---|---|---|
| `*.lzc` | Magic `JDLZ` (EA-JDLZ-Kompression) | `xxd Tracks/TRACKB1001.lzc` |
| `*.BUN`, `*.BIN` | EA-Chunk-Container (Kopf: 4-Byte-ID, 4-Byte-Länge, z. B. `0x80134000`-artige IDs) | `xxd Global/GLOBALA.BUN` |
| `Movies/*.mad` | EA MAD Video 512×256, 30,3 fps + EA-XA-ADPCM (adpcm_ea_r2) – **FFmpeg dekodiert** | `ffprobe` |
| `SNDSTREAMS/*.ast`, `SpeechStreams/*.ast` | EA SCHl-Stream, adpcm_ea_r2 22 050 Hz – **FFmpeg dekodiert** | `ffprobe` |
| `fx/*.efx` | D3DX-Effekt-**Quelltext** (HLSL), jedes Byte XOR 0xAA | Python-Test |
| `Sound/*` | Soundbänke je Kategorie (CAR_0…, NOS, SKIDS, TURBO, FE …) – Format offen | `ls` |
| `Languages/*.bin` | Sprachtabellen | `ls` |
| `Cars/<NAME>/` | `GEOMETRY.BIN`, `TEXTURES.BIN`, `VINYLS.BIN` | `ls` |

## Werkzeuge auf dem Rechner (Apple M1, 16 GB, macOS arm64)
Vorhanden: Apple clang, cmake, ninja, python3, git, Homebrew, SDL3, FFmpeg, llvm (brew), Ghidra 12.1.4 (`~/rea-stack/ghidra_12.1.4_PUBLIC`), OpenJDK 21 (`JAVA_HOME` muss gesetzt werden, siehe `decompiled/full/HANDOVER.md` §2), REA-MCP.
**Nicht vorhanden:** Wine/CrossOver, mingw-w64, Unicorn.

## Externe Referenzen
- NFS Most Wanted 2005 Decomp (GameCube, mit DWARF-Symbolen der Release-ELF): `dbalatoni13/nfsmw` auf GitHub. Gleiche EA-Black-Box-Engine-Linie („Speed“), Nachfolger von NFSU. Quelle für Klassen-, Funktions- und Strukturnamen.
- NFSU GameCube-Prototyp (10.09.2003): bei Hidden Palace dokumentiert, Symbolgehalt **unbekannt**.
- ~~Ein öffentliches Dekompilierungsprojekt für NFSU (2003) wurde nicht gefunden (Websuche 2026-10-20).~~ Korrigiert 2026-10-27, siehe Tabelle unten.

### Recherche externe Projekte (2026-10-27)
| Projekt | Befund | Quelle |
|---|---|---|
| `dbalatoni13/nfsmw` (lokal `refs/nfsmw`, Commit `80993f3`, 10.10.2026) | Lizenz CC0 1.0. Ziel ist eine byte-identische Decomp von MW 2005 (GC `GOWE69` mit ProDG GC 3.9.3, außerdem X360, PS2, PC 1.3). `src/Speed/Indep`: 966 `.cpp`, rund 191 000 Zeilen | `LICENSE`, README, `find … \| wc -l` |
| nfsmw ↔ NFSU, Signaturen | 1 237 von 6 177 NFSU-`indep`-Funktionen haben denselben mangled Namen in `config/GOWE69/symbols.txt`, 789 davon mit Größe ±10 %. Über alle GC-Funktionen sind es 3 368 gleiche Namen (inkl. SDK) | `symbols/gc_functions.csv` (`mangled`) gegen `symbols.txt` |
| nfsmw ↔ NFSU, Quellcode | 1 427 NFSU-`indep`-Funktionen haben eine Definition mit gleichem `Klasse::Methode` in der nfsmw-Quelle. Dicht bei FEngine 39/42, FEPackageReader 23/23, FEPackage 21/21, TrackStreamer 24/32, eStreamPackLoader 23/24, bWare 160/173, EAXSound 24/55. Gezählt sind Namen, ob der Inhalt gleich ist, ist offen (WP-03) | Regex über `src/Speed/Indep/**/*.cpp` |
| nfsmw `symbols/bchunks.txt` | 250 Zeilen ID → Name. Benennt 68 der 92 namenlosen IDs in `chunks.csv`. Die 24 von WP-13/14 selbst benannten IDs stimmen inhaltlich alle überein | Skriptvergleich gegen `symbols/chunks.csv` |
| `dbalatoni13/nfsug` | Decomp von NFSU **GC**, Ziel `GNDP8P` (Prototyp 10.09.2003, nicht unser `GNDD69`). CC0, angelegt 05/2025, letzter Commit 04.02.2026, `src/` ≈ 290 KB, fast nur Dolphin-SDK-Header und Ecstasy. Derzeit kein Nutzen | GitHub-API (`gh api repos/dbalatoni13/nfsug`) |
| `whoismept/OpenUG2` | Clean-Room-Nachbau der **UG2**-Engine in C mit SDL2/OpenGL. macOS arm64, MIT. Liest GEOMETRY.BIN, STREAM*.BUN, TPK, Paths*.bin und GLOBALB.BUN. Nur zur Gegenprobe für Formate geeignet | README |
| `bdrtr/PryHUB`, `NFSTools/Binary`, `SpeedReflect/Binary` | Chunk-Browser bzw. -Editoren für UG2 (BIN/BUN/LZC) | GitHub |
| `xan1242/NFSU-XtendedInput`, ThirteenAG Widescreen Fix | Hooks in die PC-Exe (Eingabe, Seitenverhältnis), MIT bzw. offen. Höchstens für WP-40/41 interessant | GitHub, nfsmods.xyz |
| NFSU-Prototypen (Hidden Palace) | PS2 und GC von Aug. bis Okt. 2003 mit Debug-Menü im Pausenmenü. Hinweise auf zusätzliche Symbole wurden nicht gefunden | hiddenpalace.org |

## GameCube-Fassung (`game/gc/Need for Speed - Underground (Germany).iso`)
| Punkt | Wert | Quelle |
|---|---|---|
| Disc | Spiel-ID `GNDD69`, „NFS Underground“, 1 459 978 240 Byte, 577 FST-Einträge | Python, Disc-Header/FST |
| Programme | `/Speed.dol` (3 036 160 B, Offset 933 499 452) und **`/Speed.elf`** (8 252 556 B, Offset 936 535 612) | FST |
| ELF | PPC32 Big-Endian, `.text` 0x800034a0 (2 501 508 B), `.rodata`, `.data`, `.bss`, `.sdata*` | Section-Header |
| **Symbole** | `.symtab` 17 295 Symbole, **10 650 Funktionen**, GNU-v2-Mangling von GCC 2.95.3 (SN Systems), **nicht** CodeWarrior (korrigiert in WP-07) (z. B. `PlaceNewCar__17TrafficTeleporterP10DriverInfo`, `GetVectorOnTrack__FP8bVector3RC8bVector2`) | `.symtab` |
| **Debuginfo** | `.debug` 4 210 984 B (DWARF 1), `.line` 192 886 B, `.debug_srcinfo`, `.debug_sfnames`, `.debug_aranges`, `.debug_pubnames` | Section-Header |
| Quellpfade | `../indep/bWare/Inc/bWare.hpp`, `../indep/src/{Camera,FEng,Physics,World,misc}/…`, `src/ecstasy/EcstasyE.cpp` (Renderer „Ecstasy“), `src/file/gc/filedev.cpp`, `src/render/AcidFXE.cpp`, Dolphin SDK 1.0 | Strings im DOL |
| Datentypen auf der Disc | bin 166, abk 103 (EA-Soundbänke), bun 73, hot 51, lzc 34, mad 25, loc 15, csi 7, ast 7 … | FST |

Hinweis: Ältere Dokumente (`decompiled/speed/*`, `run*.log`) nennen noch den alten Pfad `NFSU/Speed.exe`. Seit 2026-10-20 liegt die Datei in `game/pc/` (siehe `plan/log/2026-10-20-umzug.md`).

## Emulator und PE-Layout (WP-05, geprüft 2026-10-20)
| Fakt | Wert | Quelle |
|---|---|---|
| Speed.exe Abschnitte | `.text` 0x401000 (0x286a57), `.rdata` 0x688000 (0x3d639), `.data` 0x6c6000 (0xbfd98, schreibbar, BSS inbegriffen) | `tools/emu/nfsu_emu.py` Loader |
| Importe | 254 Thunks, alle über IAT; im Harness Stubs ab 0x7FFD0000 | Loader |
| `FUN_0059dd70` = JDLZ-Dekompressor | Kopf: `JDLZ`, Version 2, +8 entpackte Größe, +12 Gesamtgröße, +0x10/+0x11 Flag-Bytes A/B, Daten ab +0x12. A: 0 Literal/1 Kopie; B (nur bei Kopie): 0 → Länge (b0&0x1f)+3, Abstand ((b0&0xe0)<<3\|b1)+0x11; 1 → Länge ((b0&0xf0)<<4\|b1)+3, Abstand (b0&0xf)+1. Literale werden an die Zielgröße begrenzt, Kopien nicht | Emulatorvergleich, 10 000 Ströme bitgleich |
| `FUN_0059dfe0` | Dispatcher: `COMP` (0x504d4f43) → `FUN_0059deb0`, `JDLZ` → `FUN_0059dd70` | Dekompilat |
| `FUN_0059aa50` name_hash | h = -1; je Byte h = h*33 + b | Emulatorvergleich, 10 000 Strings |
| Unicorn 2.1.4 | `UC_X86_REG_ST0` liefert nur die Mantisse, `FS_BASE` veraltet. Harness nutzt GDT für FS und `fst` für Float-Rückgaben | Probe |


## Symbol-DB und Export 3 (WP-01, 2026-10-20)
| Punkt | Wert | Quelle |
|---|---|---|
| FID-Namen | 1 349 Nicht-Thunk-Funktionen mit Bibliotheksnamen, 89 Thunks | `out2/functions.tsv`, `tools/build_symbols.py` |
| Altnamen | 343 Adressen (67 exakt, 273 Skizze, 3 nur NOTES), keine Konflikte außer `entry`→`crt_startup` | `log/2026-10-20-WP-01.md` |
| Export 3 | 42 283 Aufrufkanten, 3 998 String-Refs, 28 247 Daten-Refs, 30 876 Konstanten; 1 Dekompilierfehler | `decompiled/full/out3/` |
| Reexport | `tools/reexport.sh`, ca. 7 min, dauerhaftes Projekt `decompiled/full/project_persist/nfsu` | `reexport.log` |


## GC-ELF (WP-07, geprüft 2026-10-20)
| Fakt | Wert | Quelle |
|---|---|---|
| Entpackt | `refs/gc/Speed.elf` (SHA-256 `05245132…1291`), `refs/gc/Speed.dol` (`5d9be0b5…d53d`). Alle 7 DOL-Abschnitte sind byteweise gleich den ELF-Abschnitten (`.init .text .ctors .rodata .data .sdata .sdata2`) | FST + Vergleich |
| Compiler | **GCC 2.95.3 „SN BUILD v1.76“** (54 Einheiten) und v1.72 (8), Arbeitsverzeichnis `D:\Speed\GameCube`. Mangling GNU v2 (`Name__<Klasse><Args>`, Ctor `__<Klasse>`, Dtor `_$_`/`_._`, Templates `t…`, Template-Funktionen `H…X01`) | DWARF-Producer |
| **bool = 4 Byte** | In Strukturen belegt `bool` 4 Byte (DWARF meldet 1, Offsets zeigen 4; `Force.Playing[4][8]` belegt 0x80). Der PC-Build (MSVC) hat 1 Byte → Layouts unterscheiden sich | `gc_types.py --check` |
| DWARF-Umfang | nur **62 Übersetzungseinheiten** mit `-g` (788 Funktionen mit Zeilen, 834 mit Signatur). Der Großteil des Spiels hat nur Namen, keine Typen | `dtk dwarf dump` |
| Typen | 751 Strukturen/Klassen (217 mit Basis oder virtuell), 10 Unions, 208 Enums, 491 Typedefs; 660 Größen geprüft, 658 gleich (Ausnahmen: `ReadStatus` 32-Byte-Ausrichtung, `__vtbl_ptr_type` interne Union). GCC 2.95: Vptr ist ein echtes Feld `_vptr.<Klasse>` | `symbols/gc_types.h` |
| Unity-Build | Spielcode in 21 Sammeldateien `../indep/src/generated/BB_Objs*.cpp` (5 710 Funktionen, 1,37 MB) + 83 Einzeldateien `indep` | `plan/QUELLBAUM.md` |
| Link-Reihenfolge | Die `STT_FILE`-Symbole (654) stehen in Link-Reihenfolge. `gcc2_compiled.` markiert den Objektanfang (412 Objekte), `__static_initialization_and_destruction_0`/`_GLOBAL_.I/.D` stehen am Objektende | Adressvergleich, DWARF-Bereiche |
| linkonce | 0x8025D5B0–0x80266024: 301 Inline-/Template-Funktionen ohne Objekt (`.gnu.linkonce.t`) | `gc_functions.csv` |
| Small Data | r13 = `_SDA_BASE_` 0x803C8640, r2 = `_SDA2_BASE_` 0x803D8640 | `.symtab` |
| `main` | 0x800477E0 (in `BB_ObjsMisc.cpp`) | `.symtab` |
| Ghidra | Keine Gekko-Sprache in 12.1.4 → `PowerPC:BE:32:default`; Paired-Single-Befehle (`psq_l` usw.) werden nicht dekodiert („Unable to resolve constructor“) | `decompiled/gc/run.log` |
| dtk | v1.8.4 (`dtk-macos-arm64`, SHA-256 `5ba1fd76…0353`) in `refs/tools/`. 33 Tags in `NFSSkeleton.cpp` nicht ausgegeben (`member_of` ohne Namen) | Lauf |

## Laufzeitbeobachtungen (WP-06, 2026-10-21)
| Thema | Befund |
|---|---|
| Wine-Bezug | Offizielle Homebrew-Wine-Casks sind seit 2026-09-01 deaktiviert (Gatekeeper). Installiert: Tap `gcenx/wine`, Cask `game-porting-toolkit` (meldet `wine-7.7`, GPTK 1.1), Binärdatei `wine64` (Rosetta, 32-Bit-Prozesse laufen). Prefix: `refs/wine/prefix`, Spielkopie: `refs/wine/nfsu-copy` |
| Start | `Speed.exe` lädt (wined3d → d3d9 über Apples OpenGL, dinput8, dsound, ddraw, winmm; Vulkan fehlt, unkritisch), **stürzt aber ab**: `NtUserChangeDisplaySettings` 800x600x32 → „No matching mode found“, danach Null-Zugriff bei 0x004076D7 (in `FUN_00407630`, Aufruf über Vtable +0x158 auf `g_frontend_obj`, das 0 ist) |
| Wirkungslos | virtueller Desktop (`explorer /desktop=…`), Registry `Mac Driver\RetinaMode=n`, `EA Games\…\Install Dir` unter Wow6432Node |
| **Ursache Absturz 1** | Mit `+d3d9`: `CreateDevice` (Vollbild) scheitert, `hr 0x8876086a` (INVALIDCALL), weil macdrv nur 960x600, 1024x640, 1280x800, 1440x900, 1680x1050, 1920x1200, 2048x1280, 2560x1600 anbietet (kein 800x600/1024x768). `0x407630` ruft dann Vtable +0x158 auf das Null-**D3D-Device** (`DAT_0072b180` ist IDirect3D9, das Device liegt in `g_frontend_obj`-Nachbarn; Name `g_frontend_obj` ist falsch). Das Spiel prüft den CreateDevice-Rückgabewert nicht |
| Ursache Absturz 2 | `FUN_0040f9c0` (aus `render_create_window` 0x407c00) markiert in `0x712ff0[5]` die vom Adapter angebotenen Auflösungen (Tabelle Breite `0x6f31bc` = 640, 800, 1024, 1280, 1280; Höhe `0x6f31d0` = 480, 600, 768, 960, 1024). `FUN_0040fa50` läuft ab `DAT_006f3020` aufwärts bis zum ersten gesetzten Eintrag; ist keiner gesetzt, liest sie über die Tabelle hinaus (Seitenfehler 0x786000) |
| Fenstermodus | `FUN_00407a80` füllt D3DPRESENT_PARAMETERS bei 0x7121ac; Feld Windowed (+0x20 → 0x7121cc) = `[0x72b194]`, wird im Code nirgends geschrieben (nur Lesezugriff) |
| Ergebnis mit Patches | Mit `tools/wine/patch_copy.py` (Windowed=1, Auflösungstabelle auf macOS-Modi) und `Drivers\Audio=""` läuft `Speed.exe` weiter als bisher und bleibt am Leben (Stand nach 70 s). DirectSound öffnen mit CoreAudio-Treiber → Wine-Assertion `alloc_pages_vprot` (virtual.c:1032) und Stack-Zerstörung. Bildschirmfotos: `screencapture` aus der Sitzung nicht erlaubt („could not create image from display“) |

## Abgleich PC ↔ GC (WP-08, 2026-10-21)
| Fakt | Wert | Quelle |
|---|---|---|
| Zuordnung | 3 889 PC↔GC-Paare (`symbols/pc_gc_map.csv`): sicher 516, wahrscheinlich 1 706, möglich 1 667; Spielbereich (0x401000–0x5a0000 ohne CRT) 59,2 % zugeordnet, 35,0 % mindestens wahrscheinlich | `tools/match/match.py` |
| Genauigkeit | Kreuzvalidierung ohne Strings: sicher 98 %, wahrscheinlich 93 %, möglich 84 %; Handstichprobe 20: 12 eindeutig richtig, 0 falsch, 8 nicht entscheidbar | `plan/log/2026-10-21-WP-08.md` |
| Linkreihenfolge | PC-Adressordnung = GC-Linkreihenfolge, auch für die 21 Unity-Dateien `BB_Objs*` (Reihenfolge im Log) | Anker-Auswertung |
| Größen | PC-Funktionsgröße ≈ GC-Symbolgröße × e^-0,15 (Median, σ 0,26 im log) | `Matcher.fit_size` |
| Ghidra-GC-Größen | bei `halt_baddata`-Funktionen zu klein; Symboltabelle (`gc_functions.csv`) ist maßgeblich | `Car::FinishMovement` 8 statt 2 788 |
| GC-Vtable | `_vt.<len><Klasse>` in `.data`: 8-Byte-Einträge, Eintrag 0 = Kopf, Funktionszeiger im zweiten Wort jedes Eintrags (Wortindex 3, 5, 7 …) | ELF-Dump |
| MSVC-Anteil | PC enthält sehr viele Kleinstfunktionen (≤ 32 Byte, ca. 1 000 im Spielbereich), die GCC einbettet; nur 20 % davon haben ein GC-Gegenstück | Log WP-08 |

## Klassifikation (WP-02, 2026-10-21)
| Fakt | Wert | Quelle |
|---|---|---|
| Umfang | 10 378 Funktionen: 5 620 rekonstruieren (54 %), 2 740 ersetzen (26 %), 2 018 entfällt (19 %); `kind=unbekannt`: 0 | `symbols/modules.csv`, `plan/MODULE.md` |
| Adressaufbau | 0x401000–0x41de40 PC-Plattform (Gegenstück GC-Objekte 0–35, plus WinMain/Fenster/Eingabe/Speicherstände) · –0x52a5a0 Spielcode in GC-Linkreihenfolge · –0x546cc0 PC-only Online-Lobby (`MU_Online*.fng`, `ps2nfs04.ea.com`) · –0x59ed50 bWare, indep-Einzeldateien, JDLZ (0x59dd70) · –0x611550 D3DX9-Shader/Effektcompiler (1 132 Fkt., 422 KB, 18,8 % der Bytes, nicht 16-Byte-ausgerichtet) · –0x6641fc EA-Bibliotheken (EAGL, EAGLAnim, Font, SHAPE, FILESYS, RealCore, RCMP/MAD, CSIS, EA-Audio, Netz) · –0x678000 MSVC-CRT (+ ASYNCFILE 0x6767b0–0x677120) · –0x67e660 `Unwind@`-Funclets · bis 0x688000 Initialisierer globaler Objekte | `tools/classify/classify.py` ZONES |
| zlib | nur Kennung `inflate 1.1.4` bei 0x6a7679, Tabellen im `.rdata` ohne Codeverweise gefunden; zlib-Code ist nicht als eigener Block erkennbar (nicht verwendet für Spieldaten, dort JDLZ) | Bytesuche, `datarefs.tsv` |
| Importe | Ghidra `EXTERNAL:%08x` = laufende Nummer ab 1 in Reihenfolge der Importtabelle (254 Stück, geprüft an `RegOpenKeyExA`, `DirectInputCreateA`) | `tools/classify/pe_imports.py` |
| GC-only | Anker aus `pc_gc_map.csv` in Dolphin-SDK/newlib-Objekte (6 Stück) sind Fehlzuordnungen und werden verworfen | `classify.py` |

## Typen, Vtables, COM (WP-04, 2026-10-21)
| Fakt | Wert | Quelle |
|---|---|---|
| PC-Vtables | 462 Zeigerfolgen auf Code mit Code-Referenz (441 mit Konstruktor-Beleg), 4 218 Slots; viele Slotziele sind in Ghidra keine Funktionen | `symbols/vtables.csv` |
| Vptr | MSVC schreibt den Vptr bei Offset 0 des Objekts bzw. an den Anfang der einführenden Klasse (GCC 2.95: hinter die Felder). Gegenprobe an 9 Klassen | `tools/types` |
| Löschdestruktor | Slot 0 vieler Klassen: `call X::~X; test byte [esp+8],1; je; … call bFree` | Dekompilat |
| Direct3D | `g_d3d9` 0x72b180, `g_d3d_device` 0x72b184 (nicht `g_frontend_obj`), Back-Buffer 0x72b18c; DSound 0x70b8a8; DInput8 0x72b168 und 0x72b150 | `com_globals.csv` |
| COM-Offsets | IDirect3DDevice9: Present +0x44, SetRenderState +0xE4, SetTexture +0x104, DrawIndexedPrimitive +0x148 (119 Slots) | `com_defs.py`, gegen Dekompilat |
| types.h | 755 Strukturen/Unions, PC-Layout berechnet (bool 1 Byte), 753 Größen mit clang `-m32` geprüft | `tools/types/gen_types.py` |
| Wirkung | Offsetausdrücke im Export 15 087 → 13 568 | `refs/types_cache/baseline_offsets.txt` |

## 64-Bit-Strategie (WP-11, 2026-10-21)
| Fakt | Wert | Quelle |
|---|---|---|
| Zeiger in Chunkdaten | nur leere Slots (0) für Lader; 1 820 von 1 820 TextureInfo in `Cars/*/TEXTURES.BIN` haben Slots +0..+8, +0x68..+0x74 = 0 | Python-Scan |
| TextureInfo | 0x7c Byte, Chunk 0x33310004; Pack-Kopf 0x33310001 (0x7c), Tabelle 0x33310005 je 0x20 Byte | `Cars/240SX/TEXTURES.BIN`, 0x529410 |
| Solid-Header | Chunk 0x134002 (eSolidListHeader 0x80), 0x134003/4 Tabellen; Lader 0x523f30 | Dekompilat |
| Speicher < 4 GiB | auf macOS arm64 nicht möglich (mmap schlägt fehl, `-pagezero_size` klein → Exit 137) | Test 2026-10-21 |
| `NameHash` TextureInfo | ≠ `name_hash(DebugName)` für 106 von 119 Einträgen | Python |

## Video (WP-16, 2026-10-21)
| Fakt | Wert | Quelle |
|---|---|---|
| Filme | 45 `.mad` in `Movies/`, 39 × 512×256, 6 × 512×512, 30,303 fps, Ton 44,1 kHz Stereo `adpcm_ea_r2` | `nfsu-tool movie-md5` |
| Container | EA-Multimedia: `MADk` (3 864 Byte Kopf), dann `SCHl`; libavformat `ea` + libavcodec `eamad` lesen alle 45 | ffprobe |
| Untertitel | keine Dateien in `Movies/` | `ls` |
| Original-Player | `MovieStart` 0x57f080, `MovieUpdate` 0x57f130 (RCMP_Player.cpp) | `functions.csv` |

## Audioformate (WP-15, 2026-10-21)
| Fakt | Wert | Quelle |
|---|---|---|
| `.ast` | Folge unabhängiger Ströme: `SCHl` (Kopf), `SCCl` (Blockzahl), `SCDl`×n, `SCEl`, danach Nullen bis zur Ausrichtung. Tags und Größen LE, Größe inkl. 8 Byte Kopf. Zahl der Ströme: Music 27, AMB_Mono 102, AMB_Stereo 320, Special 33, Speech 277 (je Sprache) | `nfsu-tool snd` |
| SCHl-Kopf | `PT\0\0`, dann Elemente (Tag, Längenbyte, Wert BE) bis `0xFF`; `0xFD` leitet die Audioelemente ein: `0x80`=2 (Codec), `0x82` Kanäle (fehlt = 1), `0x84` Rate (fehlt = 22 050), `0x85` Samples je Kanal | Python/C++ |
| Codec | EA-XA R2 in allen Dateien. SCDl: u32 Samplezahl (&0x0FFFFFFF), je Kanal u32 Offset ab Ende der Offsettabelle, Frames: `0xEE` = Rohframe (cur, prev, 28 BE16, 61 Byte), sonst Koeffizientenbyte + 14 Byte (15 Byte). Zustand läuft über Blöcke, Frame 1 jedes Stroms ist `0xEE` | bitgleich zu FFmpeg |
| Endframe | Blöcke enthalten nur ganze 28er-Frames, Kopf-Samplezahl ist bis zu 27 größer als dekodierbar (FFmpeg verwirft den Rest ebenso) | Vergleich |
| Bitgleich | alle 6 `.ast` bitgleich zu `ffmpeg -f s16le` (ganze Datei); SpecialStream hat Mono- und Stereoströme (24 kHz), FFmpeg dekodiert dort ab Strom 15 falsch (nimmt Kopf von Strom 0), je Strom 33/33 gleich | `tools/snd_verify.py` |
| Spieldauer | Music 5 837 s in 27 Strömen (Titel je 30–330 s), AMB_Stereo 4 344 s (320 × 15,4 s), AMB_Mono 729 s, Special 303 s, Speech 1 516 s (EN) | `nfsu-tool snd` |
| Stromindex | `Sound/STRMBANK/<name>.abk` enthält ab 0x1d4 (Music 0x1e4) Tripel `(1, Offset im .ast, 0)` je Strom, davor das Zählwort. 5 von 6 Dateien decken sich mit den SCHl-Offsets; AMB_Mono nennt 92 von 102 Strömen | Python |
| `.abk` | `ABKC`, Version 0x101. Kopf (u32 LE): +0x14 Dateigröße, +0x18 Größe des Objektabbilds, +0x20/0x24 Offset/Größe Sampleflächen A, +0x28/0x2c Fläche B (nur NOS/Skids/Turbo/…: leer in allen 106), +0x30/0x34/0x38 Offsets dreier Tabellen (je Zähler + Einträge) | `AEMS_BankLoad` 0x6416f0 |
| Objektabbild | ab 0x5c: C++-Objekte (Sounddeskriptoren, Hüllkurven …) plus x86-Stub (Folge `push esi; call rel32; mov [esi+X],eax; add esi,N`). Tabelle 1 = Offsets der rel32-Felder (Typ-ID → Distanz zu `PTR_LAB_006dc368[id]`), Tabelle 2 = Offsets von Zeigerfeldern (+ Basis), Tabelle 3 = Tripel (Ziel, Quelle, Typ) für entpackte Blöcke | `AEMS_BankRelocate` 0x6414c0 |
| Sampleflächen | beginnen mit `BNKl`: u16 Version (5), u16 Soundzahl, u32 Größe, Tabelle ab +0x14 (u32 rel, 0 = leer; Ziel = Eintragsposition + rel). Je Sound `PT\0\0` + Elemente: `0x85` Samples, `0x84` Rate (fehlt = 22 050), `0x86/0x87` Schleife, `0x88` Datenoffset ab Flächenbeginn, `0xA0`=8 → PCM16 LE, fehlt → EA-XA R2 (fortlaufende Frames ohne Block­kopf, Länge ⌈n/28⌉) | `nfsu-tool abk` |
| Soundbänke | alle 106 `.abk` parsen, 563 Sounds (456 PCM16, 107 EA-XA), alle mono; Raten 44,1 k (396), 22,05 k (110), 32 k (47), 24 k (6), 28 k (4). Gelesene Bytes liegen bei EA-XA höchstens 17 Byte unter dem Abstand zum nächsten Sound (Ausrichtung) – das Rahmenformat stimmt | `nfsu-tool abk` |
| Ladefluss | `EAXAemsManager::LoadAemsBank` 0x506500 → `AddQueuedFile` → Rückruf → `FUN_00506820` → `AEMS_BankLoad` 0x6416f0; Pfade in `SetCurrentBankPath` 0x5061b0: `sound\<Ordner>\<Name>.abk`, Streams `sound\strmBank\<Name>.abk` + `SNDStreams\` bzw. `SPEECHSTREAMS\<Name>.ast` | Dekompilat |
| Nicht entschlüsselt | Bedeutung der Objekte im Abbild (Zuordnung Sound-ID → Sample, Hüllkurven, Namenstabelle am Dateiende `FX_NITROUS_01` mit Hash), `Sound/EVT_SYS/*.a1c/*.csi` | – |

## Geometrie (WP-14, 2026-10-27)
| Fakt | Wert | Quelle |
|---|---|---|
| Kette | `InternalLoaderSolidHeaderChunks` 0x523f30 (0x134002 eSolidListHeader, 0x134003/4 Tabellen) → `FUN_00524010` 0x524010 (0x134011 eSolid, 0x134012 Texturtabelle, 0x134013 Lichtmaterialtabelle) → `FUN_0040b760` 0x40b760 (0x134900 eModel, 0x134b01 Vertexdaten, 0x134b02 StripEntry, 0x134b03 Indexdaten) | Dekompilat, Namen aus `gc_functions.csv`/`gc_types.h` |
| Vertexformat | 36 B: Position(3f) Normale(3f) D3DCOLOR UV(2f) — Fahrzeugteile; 24 B: Position(3f) D3DCOLOR UV(2f), keine Normale — einfache Streckenobjekte; Stride nicht aus einem Format-Flag gelesen, sondern pro Solid erkannt (Indexgrenze + Positionsplausibilität) | Bruteforce an 395 Solids aus `Cars/350Z/GEOMETRY.BIN` (Einheitsnormalen exakt 1.000) |
| Ausrichtung | Vertexdaten beginnen an der nächsten 128-Byte-Dateigrenze, Indexdaten an der nächsten 16-Byte-Grenze (Füllbytes 0x11 davor im Original) | Bytevergleich, 395 Solids |
| Indizes | uint16, Dreiecksliste (kein Strip trotz `eStripEntry`-Tabelle; die Tabelle unterteilt die Liste nur in Draw-Batches) | Indexmuster (0,1,2, 2,3,0 …) |
| Umfang | Alle 39 `Cars/*/GEOMETRY.BIN`: 9389 Solids, 0 Dateifehler, 1 übersprungen; alle 5 Top-Level-Streckenpakete `Tracks/STREAML1R{A,B,C,E,X}.BUN`: 8064 Solids, 1002 übersprungen (~drittes, unbekanntes Vertexformat); übrige 93 Streckendateien ohne 0x134xxx-Inhalt (Texturpakete bzw. Streckentopologie in anderem Format) | `nfsu-tool mesh` über alle Dateien |
| Streckentopologie | Die Fahrbahn/Kollisionsgeometrie liegt nicht in 0x134xxx-Chunks; vermutlich eine der in WP-12 noch „nicht gefundenen“ 0x34xxx-Chunk-Familien | Dateiscan, offen |

| Vertexformat 60 B | Flag 0x4081: Pos, Normale, D3DCOLOR, UV, BLENDW(3f), BLENDI(3f) (`worldbone.efx`); Formattabelle 0x6f3270, gebaut in 0x40d1d0 | Dekompilat + Dateien |
| StripEntry | 0x3c B: Anzahl +0xc, Texturindex +0x1c (in 0x134012, 8 B/Eintrag), Startindex +0x34; Dreiecksliste | Zeichenschleife 0x40dd10 |
| Übersprungene Strecken-Solids | leer (keine Vertex-/Indexdaten), kein unbekanntes Format | RX-Prüfung |

## Texturpakete (WP-13, 2026-10-21)
| Fakt | Wert | Quelle |
|---|---|---|
| Aufbau | `B3300000` ⊃ `B3310000` (Kopfteil: `33310001` Kopf 0x7c, `33310002` Index je 8, `33310003` Streaming-Einträge je 0x14 (optional), `33310004` TextureInfo je 0x7c, `33310005` PlatInfo je 0x20) und `B3320000` (`33320001` 24 B, `33320002` Daten) | `InternalLoadTexturePackHeaderChunks` 0x529410 |
| Umfang | 347 Pakete in 142 Dateien, **27 458 Texturen**, alle dekodiert und als PNG exportiert (0 Fehler); `Support/EReg.bin` ist keine Chunkdatei | `nfsu-tool tex` |
| Formate (PlatInfo +0x14 = D3DFORMAT) | P8 (41) 17 572, DXT1 7 356, DXT3 2 062, A8R8G8B8 (21) 468; **kein DXT5**, kein 565/4444. `ImageCompressionType` 0x22 = DXT1, 0x24 = DXT3, 0x20 = ARGB, P8 = 0x08 (8 808), 0x80 (8 076), 0x81 (688) | Lauf über alle Pakete |
| Mipmaps | `ImageSize` = Summe aller `NumMipMapLevels` Stufen (max(1,w>>l) …, DXT in 4×4-Blöcken), 27 458/27 458 exakt | Python-Survey |
| P8 | Palette = `PaletteSize/4` D3DCOLOR (Speicher B,G,R,A); 0x4071c0 kopiert den DWORD 1:1 nach A8R8G8B8 und setzt den Typ auf 0x20. Fast alle P8-Texturen sind Vinyls (`Cars/*/VINYLS.BIN`) | Dekompilat 0x4071c0 |
| PC-PlatInfo | +0x00/+0x04 Liste (Kopf 0x77f5c0), +0x08 ∈ {0,1}, +0x0C = 5, +0x10 ∈ {6,2}, +0x14 D3DFORMAT, +0x18 IDirect3DTexture9* (Laufzeit), +0x1C = 0 | 0x4071c0, alle Pakete |
| PC-Kopf ≠ GC | `TexturePackHeader` am PC: Name 28 B (+0x04), Dateiname 64 B (+0x20), **+0x60 = name_hash(Dateiname)** (347/347), ab +0x64 wie GC (+0x6C EndianSwapped = 1, +0x70 Pack, +0x74 Index, +0x78 Streamtabelle). `33320001` +0x0C trägt denselben Hash | Python, Lader |
| Rohdaten | Ohne `33310003`: Placement zählt ab der ersten 0x80-Grenze (Dateioffset) nach Beginn der `33320002`-Nutzdaten (Füllbytes 0x11); 263/263 Pakete passen, in `INGAMEB.BUN` beginnen die Daten selbst mit 0x11 | Python |
| Einzelkompression | Mit `33310003` (84 Pakete: Autos, Vinyls, Frontend): je Textur ein **JDLZ-Block** bei absolutem Dateioffset (`file_offset`, `size` = gepackte Größe, Flags +0x0D Bit 0 immer gesetzt). Entpackt = Bereich vom kleinsten Placement bis Palettenende (Bild, 0x80-Ausrichtung, Palette). 20 205 Blöcke, alle JDLZ | Survey, `InternalLoadStreamingEntry` 0x522890 |
| Leere Texturen | 27 Licht-Texturen pro Auto (`*_HEADLIGHT`, `*_BRAKELIGHT`) sind ganz transparent (29-Byte-JDLZ-Block, DXT1 mit c0 ≤ c1, Index 3) – echte Daten, kein Dekodierfehler | 240SX |
| Zeitbedarf | Export aller 27 458 PNG: 140 s (Release, M1), 561 MB | `time` |
| Effekte laden | `init_render_devices` 0x40d1d0: 7 Effektdatensätze (0x7123f8, Stride 0xE0): world, worldheadlight, worldreflect, worldheadlightreflect, worldbone, car, filter; Techniken der Reihe nach per ValidateTechnique (+0xF4), erste gültige gewinnt; Index 0 = Shaderpfad (`0x72b230`), >0 = Fixed Function (`0x72b22c`) | Dekompilat, nicht im Lauf geprüft |
| Effektparameter | 47 Namen ab 0x6f3a10 (Stride 0x40), Handle k im Datensatz bei Wortindex 7+k; ID3DXEffect-Vtable: SetInt +0x68, SetFloat +0x78, SetVector +0x88, SetVectorArray +0x90, SetMatrix +0x98, SetTexture +0xD0, SetTechnique +0xEC, Begin/Pass/End +0x100/104/108 | `plan/RENDER-ZUSTAENDE.md` |

## nfsmw als Code-Spender (WP-03, 2026-10-27)
| Fakt | Befund | Beleg |
|---|---|---|
| Zuordnung | `symbols/nfsmw_map.csv`: 1 506 Zeilen. 1 192 `signatur` (737 `wahrscheinlich` mit Größe ±10 %, 455 `moeglich`), 314 `name`. 45 von 1 237 Signaturtreffern sind compilererzeugte Initialisierer (nicht zugeordnet, Absicht). 88 Signaturtreffer haben keine Quelldatei (Definition inline im Header). 179 Treffer zeigen auf einzeilige, meist leere Körper | `tools/nfsmw_index.py`, nfsmw `80993f3` |
| Trefferquote Stichprobe (30 Paare, Seed 3, Größe 80–300 B) | `signatur/wahrscheinlich`: 8 von 8 beurteilbaren brauchbar (5 gleich, 3 gleich bis auf Felder/Konstanten). `signatur/moeglich`: 3 von 8 (1 gleich, 2 gleich bis auf, 5 anders). `name`: 3 von 10 (nur die mit Größe ±10 %: 3 von 3; die übrigen 0 von 7) | `plan/log/2026-10-27-WP-03.md` |
| Größe als Prüfwert | Weicht MW um mehr als ≈ 20 % ab, ist die Funktion in MW 2005 anders (mehr Logik, anderer Parametersatz). Bis ≈ 20 % meist gleich bis auf Felder | gleiche Stichprobe |
| GC-Dekompilat unvollständig | 873 von 10 748 Funktionen in `decompiled/gc/out/all_functions.c` bestehen aus `halt_baddata()` (Ghidra kann die Befehle nicht lesen, vermutlich Gekko Paired Singles). In der Stichprobe 5 von 30. Dort ist nfsmw die einzige lesbare Vorlage | `grep -c halt_baddata` |
| Strukturvergleich GC ↔ MW | gleich: `bNode`, `bList`, `bChunk` (8 B), `bMatrix4` (0x40), `eModel` (0x18), `TextureInfo` (0x7C, Namen stimmen). Anders: `eSolid` GC 0x98 ↔ MW 0xE0, `Camera` 0x27C ↔ 0x290, `eView` 0x54 ↔ 0x68, `FEObject` 0x58 ↔ 0x5C. In der GC-DWARF ohne Typ: `FEPackage` (leer, 1 B), `TrackStreamer`, `ResourceFile`. MW-Größen 0xC4, 0x888, 0x50 sind dort nur Hinweis | `/scratchpad`-Skript, `gc_types.h` ↔ `refs/nfsmw/src/Speed/Indep` |
| Chunk-Namen | `chunks.csv` hat Spalte `name_mw`: 92 von 116 IDs benannt (alle 24 bereits benannten stimmen überein, 68 neu). Ohne Namen bleiben 24 (u. a. 0x34035/34104/3412x/3413x/34148/34149/34154, 0x39200–39202, 0x3a000, 0x3b300, 0x80034020, 0x34036/34492/34a03) | `symbols/chunks.csv` |
| Anteil Vorlage je Paket (PC-Seite, Basis 5 620 Funktionen) | `wahrscheinlich` / `moeglich`: WP-30 70/94, WP-31 36/54, WP-32 21/35, WP-33 0/20, WP-34 2/1, WP-35 0/5, WP-36 22/25, WP-37 108/62, WP-38 14/73, WP-39 8/47. Insgesamt 281/416 | `symbols/pakete.csv` (Spalten `vorlage_*`) |

## WP-18 Strecken-Viewer
| Fakt | Wert | Quelle |
|---|---|---|
| Streckendateien | Je Strecke `TRACKS\STREAML1R?.BUN` (Region, Geometrie + Szenerie + Texturen), `TEX<id>TRACK.BIN`, `GLOBAL\GLOBALB.BUN` (TrackInfos); `STREAML1R?_<id>.BUN` sind in L1RA 0 Byte. Strecken derselben Region teilen dieselbe Geometrie (Streckenführung kommt aus `TRACKB<id>.lzc`, noch nicht gelesen) | `nfsu-viewer --stats` |
| Schlüssel Szenerie → Solid | `SceneryInfo::NameHash[lod]` = `name_hash(Solidname)`, der Name steht als String am Ende von 0x134011. Das Feld +0x10 des Solidkopfs ist nur bei 1043 von 5066 Solids gleich diesem Hash. Mit dem Namenshash fehlen in L1RA 2 331 statt 14 117 von 21 461 Instanzen | Python + Viewer |
| Rest ohne Solid | 2 331 (L1RA), 328 (RB), 131 (RC), 173 (RE), 5 (RX); Ursache offen (57 Solids ohne Format reichen nicht, vermutlich andere Chunkfamilie oder LOD-Hashes) | Viewer |
| TRACKB: Streckenabschnitte | Chunk 0x34153: +0x08 Streckennummer, +0x0C Anzahl, ab +0x10 int16-Abschnittsnummern (1001: 19, 1004: 13). Chunk 0x34152: Einträge à 148 B, +0x08 Abschnittsnummer, +0x10 Anzahl, ab +0x14 int16-Liste sichtbarer Abschnitte (max. 64). Chunk 0x34151: 101 Grenzpolygone à 140 B (Nummer +0x08 als int16, Punktanzahl +0x0A) | Dateien, MW `VisibleSection.hpp` |
| LOD-Abschnitte | Abschnittsnummer + 10 (101 → 111) | Viewer-Abschnittsboxen |
| Platzhalter-Solids | 57 (RA) Solids haben nur Füllbytes als Vertexpuffer und keine Indizes | Viewer-Diagnose |
| Alpha je Textur | `AlphaUsageType` +0x55 (1 Alphatest), `AlphaBlendType` +0x56 (0 keiner, 1 SrcAlpha/InvSrcAlpha, 2 additiv, 3–8 wie 1); in RA: Blend 0 deckend 1 229+150 Stück, Blend 2 ≈ 100 | nfsmw `Texture.hpp`, `EcstasyEx.cpp`, TextureInfo-Statistik |
| Panorama-Abschnitte | Abschnitte 2600 (Z0, Himmelskuppel/Ozean, bbox ±Tausende) und 2601–2617 (Z1–Z17, flache Fernflächen/Wasser) stehen in keiner TRACKB-Sichtbarkeitsliste und werden immer gezeichnet | Viewer, Vergleich gefiltert/ungefiltert |
| INGAMEB.BUN | `GLOBAL\INGAMEB.BUN` (1,7 MB, 15 Solids) enthält Texturen, die Streckenobjekte per Hash nutzen: `SFX_LIGHT_BEAMA/B` (0x2e95ce7c/7d, additiv, Lichtkegel an Lampen), `TRACK_BARRIER_FRAME` (0x2413c4c4), `GRAY_MID_A` (0x8c58916b); `INGAMEA.BUN` ist 0 Byte | Texturpaket-Scan aller Dateien |
| tCubic1D::Update ruft ClampSecondDerivative mit fDClamp | Bedingung `fDDClamp > 0`, Argument aber `fDClamp` (Fehler im Original, per Emulator bestätigt, 20 000 Eingaben) | WP-30.1, `point.cpp` |
| Ghidra-Namen Mathehelfer | 0x4361d0 = bTruncF, 0x4361f0 = bCeil, 0x546e60 = bFMod, 0x599e10 = bNormalize, 0x599fc0 = bScaleAdd, 0x666390 = `__ftol` | WP-30.1 |
| Timer | `PackedTime` in 1/4000 s, 0 = ungesetzt, 0x7fffffff = unendlich; Einheitenschalter für `PrintToString` in Global 0x6e0f60 | WP-30.1 |
| FPU-Genauigkeit | D3D9-Standard 24 Bit, Emulator-FPCW 0x27F = 53 Bit; Abweichungen in x87-Code sind erwartbar | WP-30.1 |
| tRumbler | Feld +0x18 je Platz: Wert = Kurve(time) * scale und Zeitfortschritt dt / scale; StartRumble 0x5544f0 ist stdcall (Kurve, scale, Priorität) | WP-30.1, Emulator |
| Joylog-Format | Eintrag = Kopfbyte (Bytes<<4 | Kanal) + Wert LE; Kanal 6 = QueuedFile-Status; Block 0x4000 | WP-30.1 |
| QueuedFile-Bündel | höchstens 16 Dateien, Spanne <= 0x40000, Lücke < 0x8000, Start auf 0x800 abgerundet, Priorität aufsteigend, 3 gleichzeitige Lesevorgänge | WP-30.1 |
| Joystick-Scanner | 6 Arten (Typ, Taste einmal, Taste Änderung, Zwei-Tasten-Achse, Analog mit Wiederholung 25/6 Bilder, Analog über Kennlinie mit Hysterese); Bereich 0x553c20–0x5541a0 ist in Ghidra keine Funktion | WP-30.1, Emulator |
| Joystick-Typwechsel | neuer Typ gilt nach 32 Bildern in Folge; dann Totzone, Scannerwahl, Fresh-Meldung | WP-30.1 |
| Zeitgeber | Schritt = clamp(dt*1.0167 + Rest, 1..3 Bilder), quantisiert 1/3600 s (Joylog Kanal 3), Ticks 1/4000 s, Bilder 1/60 s | WP-30.1, Emulator |
| cPoint::SplineSeek | arbeitet auf tCubic1D (Ghidra-Typ falsch), = Update ohne duration-Schutz | WP-30.1, Emulator |
| bATan(x, y) | Winkel von (x, y) = atan2(y, x) in 1/65536 Umdrehung; erste Parameter ist x. Nur AX gültig (obere 16 Bit Müll). Viertelkreis über bASin-Tabelle, Quadrant per Vorzeichen | WP-30.2, Emulator |
| bASin-Tabelle | 0x6e46b8, Eintrag 8 Byte {int16 Winkel, int16 pad, float Steigung}; Stufe 0..11 mit je 16 Einträgen, auf Stufe 11 bis Index 11*16+31 (208 Einträge nötig) | WP-30.2, Emulator |
| bFixATan | 0x547290 (Verhältnis 16.16 → Winkel, Tabellen 0x6e5148 fein / 0x6e5250 grob, je 132 uint16); 0x547330 (x in EAX, y in ECX) | WP-30.2, Emulator |
| bRandom | Zustand 0x6e46b4 (Start 0x12345678), Ergebnis = Zustand % n, dann xor-shift; float-Fassung `Zustand % 0x7fffffff * 4.656613e-10 * scale` | WP-30.2, Emulator |
| bStrNCmp | liest bei zwei leeren Zeichenketten und n > 0 a[-1] (Undefiniertes im Original) | WP-30.2 |
| bNormalize(bVector3*, const bVector4*) 0x599f10 | Länge = sqrt(x²+y²+z²+w+w) (Tippfehler im Original: w statt w²), schreibt nur xyz | WP-30.2, Emulator |
| 0x598550 | ist die Matrixtransponierte, nicht bNormalize (GC-Zuordnung falsch); 0x598340 ist die affine Inverse (nicht DVDCancelAsync); 0x599300/0x599400 Quaternion→Matrix, 0x5994e0 Matrix→Quaternion | WP-30.2 |
| eMulVector 0x40a8a0 | D3DXVec4Transform(out, v, M): Zeilenvektor mal Matrix; 0x40a880 (bMulMatrix) ist D3DXMatrixMultiply. Beide laufen über einen D3DX-Zeiger (0x6c7334) und sind im Emulator nicht lauffähig (compare.py hakt 0x40a8a0) | WP-30.2 |
| QuickSpline | Objekt 0x2c: Hash, EndPointType (0 Loop/1 Line/2 Extrapoliert), BasisType, MaxParam, MinParam, Length, Dirty, Anzahl; Puffer = Punkt −1 … n+1 (je bVector4, w = Bogenlänge); Punkt = (1,t,t²,t³)·Basis[Ableitung]·4 Stützpunkte; Basismatrizen Overhauser transponiert (0x72c4b0, 3 Stück) | WP-30.2, Emulator |
| QuickSpline::FindClosestParam | Newton/Sekanten bis 998 Schritte, Schrittbegrenzung halbiert sich bei Vorzeichenwechsel; vierter Stapelparameter (use_euans_hack) wird in der PC-Fassung nicht gelesen | WP-30.2, Emulator |
| bList::AddSorted 0x598180 | Rückruf liefert ≠ 0 = „neu gehört hinter vorhandenen“; eingefügt wird vor dem ersten Knoten mit 0 | WP-30.2 |
| Table::GetValue | Index = ftol((x−Min)·IndexMultiplier); i < 0 → Eintrag 0, i ≥ n−1 → letzter; für x knapp unter Min (Index −1<f<0) rechnet es mit Bruch f+1 zwischen Eintrag 0 und 1 (Fehler im Original) | WP-30.3, Emulator |
| Linear::GetInverse | bei \|m\| ≤ 0.001 liefert es b (nicht x) | WP-30.3, Emulator |
| Average (PC-Layout) | vptr 0, nSize 4, nSlots 5, nSamples 6, nCurrentSlot 7, fTotal 8, fAverage 0xc, pData 0x10, SmallBuffer[5] 0x14 (Größe 0x28); AverageWindow: Fenster 0x28, ältester 0x2c, Zeiten 0x30; Plätze = ftol(Fenster·Frequenz) | WP-30.3, Emulator |
| LZ-Formate | 'COMP' Version 1 (16-Byte-Kopf, +6 == 1 unkomprimiert, +0xc Gesamtgröße; 16 Flaggenbits, Kopie 12-Bit-Abstand + 3..18 Länge) und 'JDLZ' Version 2 | WP-30.3, Emulator |
| 0x59d8e0–0x59ed42 („utillib“) | kein Bibliothekscode: Partikel-/Effektpool (0x50-Byte-Einträge, 0x95 Stück, Zähler bei +0x3078), kein GC-Gegenstück | WP-30.3 |
| Ghidra-Zuordnung 0x59cd00–0x59ce80 | gehört zu CarWorldObject/SpaceNode-Umfeld, nicht zu Table.cpp | WP-30.3 |

- Speed.exe (PC) SHA-256 `1f72abe1e5c527c0a161e382d489605f483dd36c4ce7a2f4d561a0964c5db235`; Datentabellen aus der Exe werden beim Bauen erzeugt (`tools/pe_image.py`, `NFSU_GAME_DIR`), nie eingecheckt (2026-10-28).
