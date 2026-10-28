# Module und Phase-3-Pakete

Erzeugt von `tools/classify/module_md.py` (WP-02) aus `symbols/modules.csv` und `symbols/functions.csv`. Nicht von Hand bearbeiten, sondern die Bereichstabelle im Skript anpassen und neu erzeugen.

**Lesart:** Ein *Modul* ist eine GC-Quelldatei (Objekt der Linkreihenfolge) oder ein PC-eigener Block (`pc/…`, `lib/…`, `msvc/…`). Ein *Bereich* (WP-30 … WP-39) fasst Module zusammen, ein *Paket* (WP-3x.y) ist eine Session mit höchstens 150 Funktionen. Die Funktionsliste eines Pakets steht in `symbols/pakete.csv` (Modul + Adressbereich).

## Gesamtumfang

| Behandlung | Funktionen | Anteil |
|---|---:|---:|
| rekonstruieren | 5620 | 54.2 % |
| ersetzen | 2740 | 26.4 % |
| entfällt | 2018 | 19.4 % |
| **Summe** | 10378 | |

## Phase-3-Bereiche (Reihenfolge = Vorschlag)

| WP | Bereich | Module | Funktionen | Bytes | davon mit GC-Namen | Pakete | hängt ab von (Aufrufe) |
|---|---|---:|---:|---:|---:|---:|---|
| WP-30 | Basis (bWare), Hilfsfunktionen, Dateien und Spielablauf | 19 | 436 | 66892 | 141 (32 %) | 3 | WP-37 (90), WP-33 (88), WP-31 (40), WP-35 (17) |
| WP-31 | Ecstasy-Engine und Render-Spielseite | 9 | 257 | 98642 | 78 (30 %) | 3 | WP-30 (360), WP-32 (30), WP-33 (21), WP-38 (12) |
| WP-32 | Welt-Infrastruktur: Streaming, Topologie, Weltobjekte | 19 | 409 | 72396 | 147 (36 %) | 4 | WP-30 (467), WP-33 (66), WP-34 (64), WP-31 (43), WP-36 (18), WP-35 (17) |
| WP-33 | Spielwelt und Rennen (World, Player, Car, Race), Fahrzeugdaten, Karriere, Replay | 8 | 768 | 181854 | 266 (35 %) | 7 | WP-30 (924), WP-37 (161), WP-32 (122), WP-31 (117), WP-34 (99), WP-38 (87) |
| WP-34 | Physik und Fahrzeugdynamik | 9 | 339 | 110474 | 158 (47 %) | 3 | WP-30 (439), WP-33 (243), WP-32 (21), WP-31 (18) |
| WP-35 | KI und Verkehr | 10 | 466 | 110798 | 260 (56 %) | 4 | WP-33 (448), WP-30 (335), WP-32 (227), WP-34 (34), WP-38 (17) |
| WP-36 | Kamera, Animation (EAGLAnim), NIS | 5 | 417 | 134976 | 134 (32 %) | 4 | WP-30 (758), WP-32 (77), WP-33 (67), WP-31 (59), WP-34 (14) |
| WP-37 | Frontend-Engine (FEng) und Frontend-Grundlagen (Fe) | 2 | 815 | 170257 | 300 (37 %) | 6 | WP-30 (726), WP-38 (198), WP-33 (151), WP-31 (54), WP-34 (17) |
| WP-38 | Frontend-Bildschirme (FePkg, PC-Frontend) | 5 | 1242 | 321850 | 485 (39 %) | 10 | WP-37 (5367), WP-30 (1373), WP-33 (453), WP-31 (51), WP-36 (37), WP-39 (28) |
| WP-39 | Audio-Spiellogik (EAXSound, CSIS) | 2 | 471 | 105410 | 151 (32 %) | 4 | WP-30 (319), WP-34 (31), WP-33 (17), WP-32 (17) |

Aufrufe aus Phase-3-Bereichen in Module, die ersetzt werden oder entfallen (Schnittstellen, die Phase 4 bzw. Stubs bereitstellen müssen):

| WP | Ziele außerhalb Phase 3 (Aufrufe) |
|---|---|
| WP-30 | `msvc/crt` (69), `lib/realcore` (33), `lib/filesys` (23), `pc/main` (6), `lib/d3dx9` (4), `pc/render` (3), `src/ecstasy/EcstasyEx.cpp` (2), `src/ecstasy/EcstasyE.cpp` (2) |
| WP-31 | `lib/d3dx9` (109), `msvc/crt` (98), `pc/main` (41), `src/ecstasy/EcstasyE.cpp` (34), `lib/filesys` (5), `pc/render` (2), `src/ecstasy/EcstasyEx.cpp` (2), `lib/eagl` (2) |
| WP-32 | `msvc/crt` (42), `lib/d3dx9` (12), `pc/render` (4), `src/ecstasy/EcstasyE.cpp` (4), `pc/main` (1) |
| WP-33 | `msvc/crt` (122), `pc/main` (68), `pc/render` (36), `pc/online` (22), `LGWheels.cpp` (16), `lib/rcmp` (9), `lib/realcore` (6), `lib/filesys` (5) |
| WP-34 | `msvc/crt` (140), `lib/d3dx9` (4), `pc/main` (2), `src/ecstasy/EcstasyE.cpp` (2) |
| WP-35 | `pc/main` (99), `msvc/crt` (30), `lib/d3dx9` (1) |
| WP-36 | `msvc/crt` (66), `lib/d3dx9` (39), `lib/eagl` (38), `pc/main` (18), `src/ecstasy/EcstasyE.cpp` (2), `lib/filesys` (1) |
| WP-37 | `msvc/crt` (40), `src/ecstasy/EcstasyE.cpp` (5) |
| WP-38 | `msvc/crt` (121), `pc/online` (41), `pc/main` (30), `pc/savegame` (8), `src/ecstasy/EcstasyE.cpp` (3), `lib/d3dx9` (3), `lib/rcmp` (2), `lib/realcore` (2) |
| WP-39 | `lib/easound` (182), `msvc/crt` (129), `lib/filesys` (16), `lib/realcore` (8), `pc/main` (3) |

**Begründung der Reihenfolge:** WP-30 (bWare: Speicher, Listen, Mathe, Zeichenketten, Dateien) wird von allen Bereichen aufgerufen und ruft selbst fast nur CRT/RealCore. Danach die Engine (WP-31 Ecstasy) und die Welt-Infrastruktur (WP-32), auf denen Spielwelt (WP-33), Physik (WP-34), KI (WP-35) und Kamera (WP-36) aufsetzen. Frontend (WP-37/38) und Audio (WP-39) hängen von der Spielwelt ab, werden aber von ihr kaum aufgerufen und können parallel zu WP-34 … WP-36 laufen. Zyklen (z. B. World ↔ Physics ↔ AI) lassen sich nicht vermeiden; dort gilt: Blätter des Aufrufgraphs zuerst (siehe `WP-3x-VORLAGE.md`).

## Pakete

| Paket | Module (Teil) | Funktionen | Bytes | Adressbereich |
|---|---|---:|---:|---|
| WP-30.1 | BB_ObjsMisc.cpp, Joystick.cpp, Rumble.cpp, Joylog.cpp, misc/Spline.cpp, misc/Timer.cpp, misc/QueuedFile.cpp, EventManager.cpp, misc/point.cpp | 144 | 24942 | 0x444550–0x585c80 |
| WP-30.2 | misc/bFile.cpp, misc/QuickSpline.cpp, EasterEggs.cpp, Slider.cpp, Localize.cpp, BB_ObjsBWare.cpp (87) | 149 | 23754 | 0x4011c0–0x597f20 |
| WP-30.3 | BB_ObjsBWare.cpp (86), misc/bMemoryOverloads.cpp, misc/Table.cpp, misc/LZCompress.cpp, pc/utillib | 143 | 18196 | 0x597f30–0x59ebd0 |
| WP-31.1 | src/render/bba_com.cpp, src/ecstasy/eMathE.cpp, TrackMarkers.cpp | 60 | 18014 | 0x401000–0x40cdd0 |
| WP-31.2 | BB_ObjsEcstasy.cpp | 115 | 30163 | 0x522170–0x52a530 |
| WP-31.3 | world/CarRender.cpp, world/SkyRender.cpp, TrackMap.cpp, lib/font, lib/shape | 82 | 50465 | 0x548770–0x62c590 |
| WP-32.1 | BB_ObjsWorld2.cpp | 118 | 20780 | 0x431450–0x4a0dd0 |
| WP-32.2 | BB_ObjsWorld3.cpp, TrafficMover.cpp, MobileTopology.cpp, world/WorldEntity.cpp, WorldObject.cpp, AnimationMover.cpp, Avoidable.cpp | 121 | 25695 | 0x40e310–0x5587e0 |
| WP-32.3 | SomeWorldObjects.cpp, world/Clans.cpp, world/WorldModel.cpp, WorldObjectCollisionBody.cpp, Subworld.cpp, world/FernFlattener.cpp, Smokeable.cpp, world/TerrainEffects.cpp, world/SpaceNode.cpp | 149 | 24501 | 0x41de40–0x59c650 |
| WP-32.4 | world/SpaceCadet.cpp, CarWorldObject.cpp | 21 | 1420 | 0x59c6b0–0x59cc90 |
| WP-33.1 | BB_ObjsWorld.cpp (128) | 128 | 28398 | 0x41e130–0x425c90 |
| WP-33.2 | BB_ObjsWorld.cpp (128) | 128 | 39746 | 0x4260b0–0x4304b0 |
| WP-33.3 | BB_ObjsWorld.cpp (128) | 128 | 23375 | 0x430520–0x436530 |
| WP-33.4 | BB_ObjsWorld.cpp (126) | 126 | 27038 | 0x436540–0x43d1d0 |
| WP-33.5 | Replay.cpp, DemoModeReplay.cpp, CarInfo.cpp | 98 | 29033 | 0x426440–0x59aad0 |
| WP-33.6 | CarLoader.cpp, CareerMode.cpp, RCMP_Player.cpp | 134 | 24499 | 0x5666c0–0x57f300 |
| WP-33.7 | CarSkin.cpp | 26 | 9765 | 0x582120–0x5848d0 |
| WP-34.1 | BB_ObjsPhysics.cpp | 135 | 35435 | 0x41e370–0x586290 |
| WP-34.2 | BB_ObjsPhysics2.cpp, RigidBody.cpp | 138 | 57362 | 0x468e10–0x589f30 |
| WP-34.3 | Wheel.cpp, Suspension.cpp, Skids.cpp, BodyDamage.cpp, CarDamage.cpp, physics/PhysicsUpgrades.cpp | 66 | 17677 | 0x562890–0x59b6c0 |
| WP-35.1 | BB_ObjsAI.cpp (134) | 134 | 30429 | 0x4236b0–0x451120 |
| WP-35.2 | BB_ObjsAI.cpp (134) | 134 | 35032 | 0x4513a0–0x45afb0 |
| WP-35.3 | BB_ObjsAI2.cpp, TrafficController.cpp, TrafficTeleporter.cpp, DrivingLocation.cpp, TrackedController.cpp, RealController.cpp | 134 | 33408 | 0x45b030–0x577390 |
| WP-35.4 | FuturePoints.cpp, TrafficIntersection.cpp, CrossTrafficEmitter.cpp | 64 | 11929 | 0x57f520–0x585270 |
| WP-36.1 | BB_ObjsAnim.cpp | 43 | 7336 | 0x472ec0–0x58a570 |
| WP-36.2 | BB_ObjsCamera.cpp (119) | 119 | 33151 | 0x474a60–0x47e3b0 |
| WP-36.3 | BB_ObjsCamera.cpp (118), NIS.cpp, animation/NFSSkeleton.cpp | 127 | 54630 | 0x47e3d0–0x57f4f0 |
| WP-36.4 | lib/eaglanim | 128 | 39859 | 0x61a960–0x6284b0 |
| WP-37.1 | BB_ObjsFeng.cpp (126) | 126 | 15616 | 0x4237e0–0x4e5900 |
| WP-37.2 | BB_ObjsFeng.cpp (126) | 126 | 24098 | 0x4e5960–0x592bf0 |
| WP-37.3 | BB_ObjsFe.cpp (141) | 141 | 36452 | 0x4e41f0–0x4f24b0 |
| WP-37.4 | BB_ObjsFe.cpp (141) | 141 | 55785 | 0x4f2590–0x58d670 |
| WP-37.5 | BB_ObjsFe.cpp (141) | 141 | 20370 | 0x58d7f0–0x592d90 |
| WP-37.6 | BB_ObjsFe.cpp (140) | 140 | 17936 | 0x592df0–0x5979a0 |
| WP-38.1 | pc/frontend | 107 | 20131 | 0x401460–0x41cd80 |
| WP-38.2 | BB_ObjsFePkg.cpp (134) | 134 | 44198 | 0x416380–0x497e20 |
| WP-38.3 | BB_ObjsFePkg.cpp (134) | 134 | 42664 | 0x497fe0–0x4a2e60 |
| WP-38.4 | BB_ObjsFePkg.cpp (134) | 134 | 40405 | 0x4a2ef0–0x4ae280 |
| WP-38.5 | BB_ObjsFePkg.cpp (134) | 134 | 32494 | 0x4ae2d0–0x4b8390 |
| WP-38.6 | BB_ObjsFePkg.cpp (133) | 133 | 29765 | 0x4b8520–0x5981e0 |
| WP-38.7 | BB_ObjsFePkg2.cpp (122) | 122 | 33051 | 0x413480–0x4cb780 |
| WP-38.8 | BB_ObjsFePkg2.cpp (122) | 122 | 27256 | 0x4cb7a0–0x4d2ae0 |
| WP-38.9 | BB_ObjsFePkg2.cpp (122) | 122 | 29577 | 0x4d2b00–0x53b3e0 |
| WP-38.10 | BB_ObjsFePkg3.cpp, FEPkg_MapScreen.cpp | 100 | 22309 | 0x4c0890–0x5486b0 |
| WP-39.1 | BB_ObjsEAXSound.cpp (149) | 149 | 39922 | 0x5005b0–0x50bf50 |
| WP-39.2 | BB_ObjsEAXSound.cpp (149) | 149 | 33167 | 0x50c0c0–0x516cd0 |
| WP-39.3 | BB_ObjsEAXSound.cpp (148) | 148 | 30852 | 0x516d10–0x521e90 |
| WP-39.4 | lib/csis | 25 | 1469 | 0x63e9b0–0x63f1c0 |

## Module außerhalb von Phase 3

| Modul | Funktionen | Bytes | Behandlung | Ersatz / Grund |
|---|---:|---:|---|---|
| lib/d3dx9 | 1132 | 421726 | ersetzen | Effekte offline nach Metal übersetzen (WP-17); Funktionen selbst nicht portieren |
| msvc/unwind | 1091 | 15201 | entfällt | vom Compiler erzeugte Unwind-Funclets (C++-Ausnahmen/Destruktoren) |
| msvc/crt | 461 | 68910 | ersetzen | Host-libc/libc++ |
| pc/online | 437 | 89482 | entfällt | PC-Online-Lobby und MU_Online*-Bildschirme (Server abgeschaltet) |
| lib/easound | 365 | 71874 | ersetzen | EA Audio (SNDSYS/AEMS, Mixer, Decoder) → Audio-Ausgabe WP-42 |
| lib/net | 256 | 50847 | entfällt | EA-Netzwerk (DirtySock-artig: TCP/UDP, TAPI-Modem, seriell) |
| msvc/staticinit | 198 | 2024 | entfällt | dynamische Initialisierer globaler Objekte; im Port Teil der Globals des jeweiligen Moduls |
| lib/eagl | 193 | 41552 | ersetzen | EA Grafikbibliothek (D3D9/D3D8-Wrapper) → Render-Schnittstelle WP-40 |
| lib/rcmp | 192 | 26096 | ersetzen | EA RCMP/AV-Player/MAD → FFmpeg (WP-16) |
| pc/main | 107 | 20831 | ersetzen | WinMain, Fenster, Registry, Einzelinstanz, Hauptschleife → SDL3 (WP-43/50) |
| lib/filesys | 85 | 10970 | ersetzen | EA FILESYS/ASYNCFILE → Dateischicht WP-43 (Schnittstelle nachbilden) |
| lib/realcore | 63 | 9594 | ersetzen | EA RealCore (Timer, Threads, CPU, Fenster, Abbruch) → WP-43 |
| win32/thunks | 36 | 216 | entfällt | Sprungstellen auf Importe |
| Periodic.cpp, LGWheels.cpp, Force.cpp, Wheels.cpp, Condition.cpp | 34 | 9131 | ersetzen | Eingabe/Lenkrad → WP-41 |
| src/ecstasy/EcstasyE.cpp, src/ecstasy/EcstasyEx.cpp | 31 | 12203 | ersetzen | Ecstasy-Plattformrenderer → WP-40 |
| Stream.cpp | 27 | 5559 | ersetzen | Datenstrom-Ein/Ausgabe → WP-43 |
| pc/savegame | 22 | 6632 | ersetzen | Speicherstände unter Eigene Dateien → WP-43 |
| pc/input | 16 | 4195 | ersetzen | DirectInput, Logitech-Lenkrad, Force Feedback → WP-41 |
| pc/render | 12 | 2922 | ersetzen | PC-Ecstasy-Renderer (D3D9) → WP-40 |
