# Quellbaum der GameCube-`Speed.elf`

Automatisch erzeugt von `tools/gc_quellbaum.py` (WP-07). Nicht von Hand bearbeiten, sondern das Skript anpassen.

**Lesart**

- Ein *Objekt* ist eine Übersetzungseinheit im Link (Symbol `STT_FILE`). Die Reihenfolge ist die Link-Reihenfolge.
- Der Spielcode liegt größtenteils in **Sammel-Übersetzungseinheiten** `BB_Objs*.cpp` (Unity-Build). Welche `.cpp` darin stecken, verrät nur DWARF, und das gibt es nur für 62 Einheiten.
  Für die übrigen nennt die Tabelle die häufigsten Klassen. Sie sind die Grundlage für die Phase-3-Module.
- „sicher“ = Zuordnung über lokale Symbole, `gcc2_compiled.`, Objektende (statische Initialisierung) oder DWARF-Bereich; „Name“ = über den Dateinamen geraten.
- 301 Inline- und Template-Funktionen am Ende von `.text` (linkonce) gehören zu keinem Objekt. Sie stehen am Schluss nach Klassen.

## Übersicht

| Gruppe | Objekte | Funktionen | Bytes | Port |
|---|---:|---:|---:|---|
| GC-Plattformcode des Spiels | 36 | 497 | 110.348 | ersetzen (Plattformschicht: Renderer, Eingabe, Speicherkarte, Film) |
| indep: Sammel-Übersetzungseinheiten (Unity-Build) | 21 | 5710 | 1.369.348 | **portieren** (Spielcode) |
| indep: Einzeldateien | 83 | 1754 | 362.000 | **portieren** (Spielcode) |
| Startcode (Assembler, crt) | 6 | 0 | 0 | entfällt |
| EA DVD-/Streaming-Hilfen (FSasync, sndvd) | 2 | 25 | 3.904 | ersetzen |
| C-Laufzeit (newlib, libgcc, libm) Teil 1 | 53 | 88 | 37.920 | entfällt (Host-libc) |
| Nintendo Dolphin SDK (OS, DVD, VI, PAD, CARD, GX, EXI, SI, IP) | 99 | 827 | 196.640 | entfällt |
| EA Videospieler (avplayer, RCMP, MAD-Codec) | 10 | 56 | 9.060 | nachbauen (WP-16) |
| EA Grafikbibliothek (rendercontext, device, model, pcode …) | 28 | 193 | 43.220 | ersetzen (WP-40) |
| EAGLAnim (Animationsbibliothek) | 32 | 221 | 99.140 | portieren (indep-nah) |
| EA Dateisystem (filesys, BIG-Archive) | 7 | 77 | 23.916 | teilweise portieren (WP-43) |
| EA RealMath | 6 | 1 | 64 | prüfen |
| Realmc (Speicherkarten-Bibliothek) | 8 | 119 | 37.472 | ersetzen (WP-43) |
| EA RealCore (Threads, Timer, Ausgabe) | 19 | 55 | 6.580 | ersetzen |
| EA Schrift und Lokalisierung (font*, loccore) | 11 | 18 | 1.976 | portieren |
| EA SHAPE (Texturformat) | 12 | 19 | 2.724 | portieren (WP-13) |
| CSIS (Sound-Schnittstelle) | 1 | 38 | 3.976 | portieren (WP-15/42) |
| EA Audio (SNDI, AEMS, Mixer, Decoder) | 137 | 474 | 91.536 | nachbauen (WP-15/42) |
| C-Laufzeit (newlib stdio, libm) Teil 2 | 54 | 44 | 10.368 | entfällt |
| Nintendo Dolphin SDK (OSFatal, AR, AX, DSP, CARD) | 24 | 109 | 29.772 | entfällt |
| Netzwerk-Hilfen (eth, md5, base64) | 4 | 22 | 13.224 | Stub (WP-44) |
| EA Codec (refsize) | 1 | 1 | 784 | portieren (WP-12) |
| Inline/Template am Textende (linkonce) | – | 301 | 35444 | mit der jeweiligen Klasse |
| **Summe** | 654 | 10650 | | |

## GC-Plattformcode des Spiels

Link-Index 0–35 · Pfad: `GameCube/src/…` · Port: ersetzen (Plattformschicht: Renderer, Eingabe, Speicherkarte, Film)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 0 | `src/ecstasy/EcstasyE.cpp` | 97 | 26412 | 97 | eViewPlatInterface (3), ePoly (1), eRenderTarget (1), frei: 92 |
| 1 | `src/ecstasy/EcstasyEx.cpp` | 46 | 11312 | 46 | cSphereMap (8), cCaptureBuffer (5), cReflectMap (3), cQuarterSizeMap (3), frei: 27 |
| 2 | `eLightE.cpp` | 9 | 2436 | 9 | eLightMaterialPlatInterface (2), frei: 7 |
| 3 | `src/ecstasy/eEnvMapE.cpp` | 7 | 1500 | 7 | `src/ecstasy/eEnvMapE.cpp` (7) |
| 4 | `src/ecstasy/eEnvEffectsE.cpp` | 1 | 4 | 1 | `src/ecstasy/eEnvEffectsE.cpp` (1) |
| 5 | `src/ecstasy/eMathE.cpp` | 25 | 3564 | 25 | `src/ecstasy/eMathE.cpp` (25) |
| 6 | `src/ecstasy/eStateE.cpp` | 0 | 0 | 0 |  |
| 7 | `src/ecstasy/eMatrixE.cpp` | 2 | 204 | 2 | `src/ecstasy/eMatrixE.cpp` (2) |
| 8 | `ePShader.cpp` | 12 | 8440 | 12 | frei: 12 |
| 9 | `src/ecstasy/eVShader.cpp` | 6 | 1532 | 6 | `src/ecstasy/eVShader.cpp` (6) |
| 10 | `eStrip.cpp` | 13 | 812 | 13 | frei: 13 |
| 11 | `src/ecstasy/eSolidPlat.cpp` | 11 | 972 | 11 | `src/ecstasy/eSolidPlat.cpp` (11) |
| 12 | `eViewPlat.cpp` | 11 | 1264 | 11 | eViewPlatInterface (6), eViewPlatInfo (1), frei: 4 |
| 13 | `Platform.cpp` | 11 | 1476 | 11 | frei: 11 |
| 14 | `src/ecstasy/TextureInfoPlat.cpp` | 17 | 1380 | 17 | `src/ecstasy/TextureInfoPlat.cpp` (17) |
| 15 | `src/JoyE.cpp` | 17 | 2484 | 17 | `src/JoyE.cpp` (17) |
| 16 | `src/render/SunE.cpp` | 0 | 0 | 0 |  |
| 17 | `screenshot.cpp` | 0 | 0 | 0 |  |
| 18 | `src/render/bba_com.cpp` | 32 | 14776 | 2 | AcidEffectSystem (10), AcidEmitter (2), frei: 20 |
| 19 | `src/render/AcidFXE.cpp` | 3 | 272 | 3 | frei: 3 |
| 20 | `src/MemoryCard.cpp` | 0 | 0 | 0 |  |
| 21 | `src/LogitechWheel.cpp` | 0 | 0 | 0 |  |
| 22 | `src/dbFiles.cpp` | 25 | 332 | 25 | `../indep/src/Frontend/dbFilesBase.hpp` (24), `src/dbFiles.cpp` (1) |
| 23 | `src/DemoDisc.cpp` | 4 | 132 | 4 | `src/DemoDisc.cpp` (4) |
| 24 | `src/MovieE.cpp` | 11 | 1812 | 11 | `src/MovieE.cpp` (11) |
| 25 | `Wheels.cpp` | 4 | 512 | 4 | Wheels (4) |
| 26 | `Ramp.cpp` | 1 | 48 | 1 | Ramp (1) |
| 27 | `Periodic.cpp` | 3 | 576 | 3 | Periodic (3) |
| 28 | `LGWheels.cpp` | 33 | 7388 | 33 | LGWheels (33) |
| 29 | `Force.cpp` | 5 | 564 | 5 | Force (5) |
| 30 | `Constant.cpp` | 3 | 524 | 3 | Constant (3) |
| 31 | `Condition.cpp` | 3 | 560 | 3 | Condition (3) |
| 32 | `Stream.cpp` | 32 | 8144 | 32 | frei: 32 |
| 33 | `src/GC_MemoryCard.cpp` | 40 | 9888 | 40 | LoadSaveGame (35), frei: 5 |
| 34 | `JoyloggableLoadSavegame.cpp` | 13 | 1028 | 13 | JoyloggableLoadSaveGame (13) |
| 35 | `GlobalA.cpp` | 0 | 0 | 0 |  |

## indep: Sammel-Übersetzungseinheiten (Unity-Build)

Link-Index 36–56 · Pfad: `../indep/src/generated/BB_Objs*.cpp` · Port: **portieren** (Spielcode)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 36 | `BB_ObjsWorld.cpp` | 429 | 119684 | 429 | Player (90), Car (82), RaceCoordinator (34), CarDriver (32), World (30), Race (29), DriftManager (16), RacingCar (15), RaceParameters (13), MiscStyleManager (13), StyleMoment (12), cPlayerSettings (9), frei: 28 |
| 37 | `BB_ObjsWorld2.cpp` | 174 | 36772 | 174 | TrackStreamer (32), TrackPathManager (22), TrackRoutePoint (17), TopologyTree (13), TopologyCoordinate (9), TrackPathCoordinate (9), TrackRouteManager (8), TrackPolygon (6), Signpost (6), TrackRoute (6), TrackInfo (5), HoleFiller (3), frei: 36 |
| 38 | `BB_ObjsWorld3.cpp` | 41 | 11860 | 41 | VisibleSectionManager (7), DrivableScenerySection (6), GrandSceneryCullInfo (5), ScenerySectionHeader (3), VisibleSectionBoundary (1), frei: 19 |
| 39 | `BB_ObjsMisc.cpp` | 94 | 11516 | 94 | GameFlowManager (13), ResourceFile (10), OnlineUtility (1), frei: 70 |
| 40 | `BB_ObjsAI.cpp` | 473 | 102164 | 473 | AICarController (45), DriveTarget (32), DrivingChoice (26), AIAction (18), AIActionObeyTrafficIntersections (16), AIActionGetUnstuck (15), CarController (14), AIActionPass (13), AIActionPullOver (13), AIActionAccidentRecovery (13), AIActionHitWallDragRace (13), AIActionAvoidAvoidableAsTraffic (13), frei: 39 |
| 41 | `BB_ObjsAI2.cpp` | 112 | 26872 | 112 | PsychoDriver (34), RealDriver (24), AIDriver (23), DragDriver (16), PlayerSteering (7), AnimationToAvoidInfo (3), frei: 5 |
| 42 | `BB_ObjsPhysics.cpp` | 221 | 49372 | 221 | StaticCollisionBody (26), DynamicCollisionBody (25), CarCollisionBody (23), CarState (19), BeltMover (18), Collision (18), CollisionBody (18), CollisionBodyWitness (16), Contact (14), TrackPolygonCollisionBody (9), PhysicsSystem (7), PhysicsComponent (4), frei: 15 |
| 43 | `BB_ObjsPhysics2.cpp` | 149 | 63224 | 149 | DriveTrain (29), PhysicsMover (24), Mellovator (18), Engine (15), MellowMover (13), GJKPolytope (10), GJKWitness (8), GJKWafer (5), GJKBoxPoly (4), GJKPointPrimitive (3), SCEA_GJKsimplex (1), SCEA_GJKsimplexPair (1), frei: 16 |
| 44 | `BB_ObjsAnim.cpp` | 68 | 13024 | 68 | NisSceneManager (18), CAnimCtrl (12), CAnimSkeleton (5), CAnimBank (4), CAnimPart (4), CNFSAnimBank (2), CAnimEngineManager (1), NisSceneMapper (1), frei: 21 |
| 45 | `BB_ObjsCamera.cpp` | 333 | 119244 | 333 | DragCameraManager (45), SelectCarCameraMover (30), CameraMover (29), DriveCameraMover (16), CubicCameraMover (14), ZonePreviewCameraMover (9), OrbitCarCameraMover (8), PreviewRoute (8), Camera (7), AnimationControllerCameraMover (7), PreviewComet (7), DemoCameraCoordinator (6), frei: 36 |
| 46 | `BB_ObjsFePkg.cpp` | 946 | 230852 | 946 | UserProfileScreen (81), PauseOptionsMenu (71), PostRaceStatsScreen (27), QROptionsScreen (26), GarageMainScreen (25), cFrontendDatabase (23), FEPlayerCarDB (23), RaceStarter (18), PostRaceMenuScreen (17), QRTrackSelectScreen (17), FEng_PauseDragOptions (16), UserProfile (16), frei: 140 |
| 47 | `BB_ObjsFePkg2.cpp` | 475 | 98116 | 475 | OptionsMenu (73), ColorPicker (26), PauseMenu (20), ControllerMenu (16), HUDMenu (15), EATraxScreen (14), LoadingScreen (14), MPCarSelectScreen (13), GaragePresetsScreen (11), LangSelectScreen (9), SideScrollScreen (8), GarageNewVinylScreen (8), frei: 95 |
| 48 | `BB_ObjsFePkg3.cpp` | 124 | 23412 | 124 | FEKeyboard (25), MemcardInterface (20), UndergroundStatsScreen (15), MemcardPrompter (7), OnlineDialogBox (7), UndergroundNewCareerFMV (6), UndergroundMagazineScreen (6), UndergroundRankScreen (6), UndergroundMenuScreen (6), OnlineOKDialog (6), UndergroundNewCareerExtroFMV (4), frei: 16 |
| 49 | `BB_ObjsFeng.cpp` | 336 | 52212 | 336 | FEngine (42), cFEng (40), FEPackageManager (20), cFEngRender (16), ThumbnailScroller (14), FEngFont (10), HorizontalBrowserBackScreen (10), FEPackageData (9), MenuScreen (8), ListPicker (6), Scrollerina (5), ScrollerSlot (4), frei: 143 |
| 50 | `BB_ObjsFe.cpp` | 571 | 159996 | 571 | FECodeListBox (24), GarageManager (24), FEPackageReader (23), FEPackage (21), FEListBox (19), FEObject (18), GaragePartsBrowser (14), GarageRimsStyleBrowser (14), GaragePerfTuner (13), GarageRimsBrowser (13), GarageCategoryVisual (12), FERenderObject (12), frei: 128 |
| 51 | `../indep/src/generated/BB_ObjsFe2.cpp` | 11 | 6600 | 11 | `../indep/src/Frontend/MagazineDatabase.cpp` (10), `../indep/bWare/Inc/bList.hpp` (1) |
| 52 | `BB_ObjsEAXSound.cpp` | 801 | 171316 | 801 | EAXTunerCar (74), EAXSound (55), EAXS_StreamControl (52), stSoundCollision (38), EAXS_StreamManager (34), EAXS_AmbStereoStream (28), EAXS_MusicStream (27), EAXS_TCMStream (26), EAXAITunerCar (23), EAXS_AmbMonoStream (23), EAXAemsManager (22), EAXS_Streams (22), frei: 41 |
| 53 | `BB_ObjsEcstasy.cpp` | 173 | 49536 | 173 | eStreamPackLoader (24), eModel (13), eView (10), TexturePack (8), eSolid (7), Irradiance (6), TextureAnimPack (3), eLightMaterial (1), TextureInfo (1), frei: 100 |
| 54 | `../indep/src/generated/BB_ObjsBWare.cpp` | 175 | 23312 | 175 | `../indep/bWare/Src/bMemory.cpp` (33), `../indep/bWare/Src/bVector.cpp` (30), `../indep/bWare/Src/bSlotPool.cpp` (25), `../indep/bWare/Src/bMath.cpp` (19), `../indep/bWare/Src/bPrintf.cpp` (11), `../indep/bWare/Src/Strings.cpp` (10), `../indep/bWare/Src/bDebug.cpp` (9), `../indep/bWare/Src/bList.cpp` (9), `../indep/bWare/Src/bFunkServer.cpp` (8), `../indep/bWare/Src/bMatrix.cpp` (6), `../indep/bWare/Src/bFunkPlat.cpp` (4), `../indep/bWare/Src/bMemCpy.cpp` (3), `../indep/bWare/Src/bQuaternion.cpp` (3), `../indep/bWare/Src/bInventory.cpp` (2), `../indep/bWare/Src/bCrc32.cpp` (1), `../indep/src/generated/BB_ObjsBWare.cpp` (1), `../indep/bWare/Inc/bFunk.hpp` (1) |
| 55 | `../indep/src/eaxsound/Global_.cpp` | 2 | 128 | 2 | `../indep/src/eaxsound/Global_.cpp` (2) |
| 56 | `../indep/src/eaxsound/NFSU_Streams.cpp` | 2 | 136 | 2 | `../indep/src/eaxsound/NFSU_Streams.cpp` (2) |

## indep: Einzeldateien

Link-Index 57–139 · Pfad: `../indep/src/<Bereich>/…` · Port: **portieren** (Spielcode)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 57 | `BuildRegionEurope.cpp` | 1 | 8 | 0 | frei: 1 |
| 58 | `BuildVersion.cpp` | 0 | 0 | 0 |  |
| 59 | `FEPkg_MapScreen.cpp` | 21 | 3472 | 21 | MapScreenV2 (10), UnlockStatusScreen (6), frei: 5 |
| 60 | `Config.cpp` | 3 | 1152 | 3 | frei: 3 |
| 61 | `../indep/src/world/CarRender.cpp` | 46 | 38316 | 46 | CarRenderInfo (13), Car (4), FrontEndRenderingCar (3), CarPartCuller (2), frei: 24 |
| 62 | `../indep/src/world/SkyRender.cpp` | 7 | 972 | 7 | `../indep/src/world/SkyRender.cpp` (7) |
| 63 | `Replay.cpp` | 45 | 8164 | 45 | ReplayManager (30), ReplaySnapshot (12), bRepeat (1), frei: 2 |
| 64 | `DemoModeReplay.cpp` | 21 | 2784 | 21 | DemoModeReplay (16), frei: 5 |
| 65 | `Joystick.cpp` | 43 | 6768 | 43 | Joystick (3), QueuedJoystickEvents (2), frei: 38 |
| 66 | `Rumble.cpp` | 23 | 4412 | 23 | tEnvelope (4), tShaker (4), tRumbler (3), frei: 12 |
| 67 | `TrafficMover.cpp` | 7 | 2060 | 7 | TrafficMover (5), frei: 2 |
| 68 | `../indep/src/physics/PhysicsUpgrades.cpp` | 9 | 2228 | 9 | `../indep/src/physics/PhysicsUpgrades.cpp` (9) |
| 69 | `MobileTopology.cpp` | 16 | 2900 | 16 | MobileTopology (6), MobileTrackPolygon (2), MobileTopologyCoordinate (1), FlatMobileTopology (1), frei: 6 |
| 70 | `../indep/src/world/WorldEntity.cpp` | 34 | 2996 | 34 | `../indep/src/world/WorldEntity.cpp` (32), `../indep/src/world/WorldEntity.hpp` (2) |
| 71 | `WorldObject.cpp` | 33 | 4176 | 33 | WorldObject (26), frei: 7 |
| 72 | `../indep/src/world/Mover.cpp` | 7 | 828 | 7 | `../indep/src/world/Mover.cpp` (6), `../indep/src/world/Mover.hpp` (1) |
| 73 | `../indep/src/world/SpaceNode.cpp` | 20 | 2236 | 20 | `../indep/src/world/SpaceNode.cpp` (20) |
| 74 | `../indep/src/world/SpaceCase.cpp` | 2 | 136 | 2 | `../indep/src/world/SpaceCase.cpp` (2) |
| 75 | `../indep/src/world/SpaceCadet.cpp` | 6 | 792 | 6 | `../indep/src/world/SpaceCadet.cpp` (6) |
| 76 | `AnimationMover.cpp` | 13 | 3452 | 13 | AnimationMover (11), frei: 2 |
| 77 | `CarWorldObject.cpp` | 14 | 1148 | 14 | CarWorldObject (7), CarWorldObjectMover (4), frei: 3 |
| 78 | `Avoidable.cpp` | 39 | 5192 | 39 | Avoidable (20), NearbyAvoidableList (14), NearbyAvoidable (1), frei: 4 |
| 79 | `SomeWorldObjects.cpp` | 28 | 3208 | 28 | TrainEngine (5), Barrel (3), NISProp (3), NISCrowd (3), NISFlagGirl (3), NISdj (3), frei: 3 |
| 80 | `../indep/src/world/SomeMovers.cpp` | 5 | 708 | 5 | `../indep/src/world/SomeMovers.cpp` (5) |
| 81 | `Joylog.cpp` | 19 | 1852 | 19 | Joylog (10), JoylogBuffer (7), frei: 2 |
| 82 | `CarInfo.cpp` | 76 | 35848 | 76 | RideInfo (33), CarPartDatabase (8), CarPart (7), CarTypeInfo (4), DriverInfo (4), frei: 20 |
| 83 | `CarPartNames.cpp` | 1 | 8 | 1 | frei: 1 |
| 84 | `../indep/src/misc/Spline.cpp` | 14 | 3380 | 14 | `../indep/src/misc/Spline.cpp` (14) |
| 85 | `TrafficController.cpp` | 38 | 5168 | 38 | TrafficController (36), frei: 2 |
| 86 | `../indep/src/misc/Table.cpp` | 26 | 3384 | 26 | `../indep/src/misc/Table.cpp` (26) |
| 87 | `RigidBody.cpp` | 27 | 7552 | 27 | RigidBody (22), RigidBodyInfo (1), frei: 4 |
| 88 | `../indep/src/misc/Timer.cpp` | 22 | 3840 | 22 | `../indep/src/misc/Timer.cpp` (22) |
| 89 | `Wheel.cpp` | 27 | 7856 | 27 | Wheel (23), frei: 4 |
| 90 | `Suspension.cpp` | 20 | 3100 | 20 | Suspension (13), TwoWheelAckermanSteering (5), Steering (1), frei: 1 |
| 91 | `../indep/src/world/WeatherMan.cpp` | 9 | 1752 | 9 | `../indep/src/world/WeatherMan.cpp` (9) |
| 92 | `../indep/src/world/fogvolume.cpp` | 10 | 1976 | 10 | `../indep/src/world/fogvolume.cpp` (10) |
| 93 | `Skids.cpp` | 19 | 4380 | 19 | SkidSet (5), SkidSegment (3), SkidMaker (2), frei: 9 |
| 94 | `../indep/src/world/Clans.cpp` | 10 | 1280 | 10 | `../indep/src/world/Clans.cpp` (10) |
| 95 | `../indep/src/misc/QueuedFile.cpp` | 20 | 2900 | 20 | `../indep/src/misc/QueuedFile.cpp` (20) |
| 96 | `CarLoader.cpp` | 80 | 10728 | 80 | CarLoader (70), LoadedTexturePack (1), LoadedSolidPack (1), LoadedSkinLayer (1), LoadedSkin (1), LoadedWheel (1), frei: 4 |
| 97 | `NIS.cpp` | 16 | 680 | 16 | NisManager (7), frei: 9 |
| 98 | `../indep/src/misc/LZCompress.cpp` | 5 | 940 | 5 | `../indep/src/misc/LZCompress.cpp` (5) |
| 99 | `BodyDamage.cpp` | 21 | 2940 | 21 | BodyDamage (12), BodyDamageModel (2), frei: 7 |
| 100 | `CarDamage.cpp` | 12 | 1704 | 12 | CarDamage (12) |
| 101 | `../indep/src/misc/VolumeTree.cpp` | 4 | 1016 | 4 | `../indep/src/misc/VolumeTree.cpp` (4) |
| 102 | `EventManager.cpp` | 12 | 3080 | 12 | frei: 12 |
| 103 | `TrackMap.cpp` | 29 | 12364 | 29 | TrackMap (25), frei: 4 |
| 104 | `TrackMarkers.cpp` | 8 | 1524 | 8 | TrackMarkerFile (4), frei: 4 |
| 105 | `../indep/src/misc/Profiler.cpp` | 1 | 4 | 1 | `../indep/src/misc/Profiler.cpp` (1) |
| 106 | `../indep/src/world/WorldModel.cpp` | 14 | 3468 | 14 | `../indep/src/world/WorldModel.cpp` (14) |
| 107 | `../indep/src/world/TerrainType.cpp` | 3 | 200 | 3 | `../indep/src/world/TerrainType.cpp` (3) |
| 108 | `WorldObjectCollisionBody.cpp` | 24 | 2144 | 24 | WorldObjectCollisionBody (24) |
| 109 | `../indep/src/world/CharacterInfo.cpp` | 2 | 996 | 2 | `../indep/src/world/CharacterInfo.cpp` (2) |
| 110 | `Subworld.cpp` | 22 | 4148 | 22 | SubWorld (19), frei: 3 |
| 111 | `../indep/src/world/FernFlattener.cpp` | 15 | 1856 | 15 | `../indep/src/world/FernFlattener.cpp` (15) |
| 112 | `../indep/src/misc/point.cpp` | 29 | 2448 | 29 | `../indep/src/misc/point.cpp` (29) |
| 113 | `../indep/src/misc/bRepeat.cpp` | 5 | 1140 | 5 | `../indep/src/misc/bRepeat.cpp` (5) |
| 114 | `../indep/src/misc/bFile.cpp` | 35 | 4572 | 35 | `../indep/src/misc/bFile.cpp` (35) |
| 115 | `../indep/src/misc/bMemoryOverloads.cpp` | 5 | 168 | 5 | `../indep/src/misc/bMemoryOverloads.cpp` (5) |
| 116 | `../indep/src/frontend/Print3d.cpp` | 2 | 128 | 2 | `../indep/src/frontend/Print3d.cpp` (2) |
| 117 | `../indep/src/world/DebugTopology.cpp` | 10 | 1076 | 10 | `../indep/src/world/DebugTopology.cpp` (10) |
| 118 | `Smokeable.cpp` | 68 | 16880 | 68 | Smokeable (36), VirginSmokeable (7), SmokeableInfo (1), frei: 24 |
| 119 | `../indep/src/world/TerrainEffects.cpp` | 5 | 972 | 5 | `../indep/src/world/TerrainEffects.cpp` (5) |
| 120 | `TrafficTeleporter.cpp` | 53 | 12964 | 53 | TrafficTeleporter (50), frei: 3 |
| 121 | `DrivingLocation.cpp` | 7 | 724 | 7 | DrivingLocation (7) |
| 122 | `TrackedController.cpp` | 12 | 1228 | 12 | TrackedController (12) |
| 123 | `RealController.cpp` | 17 | 2764 | 17 | RealController (17) |
| 124 | `../indep/src/misc/QuickSpline.cpp` | 27 | 6580 | 27 | `../indep/src/misc/QuickSpline.cpp` (27) |
| 125 | `../indep/src/misc/Stomper.cpp` | 11 | 1084 | 11 | `../indep/src/misc/Stomper.cpp` (11) |
| 126 | `EasterEggs.cpp` | 19 | 4588 | 19 | EasterEggs (16), frei: 3 |
| 127 | `../indep/src/ai/Elipse.cpp` | 9 | 628 | 9 | `../indep/src/ai/Elipse.cpp` (9) |
| 128 | `Slider.cpp` | 18 | 3632 | 18 | cSlider (7), cIllusionSlider (7), cVolSlider (4) |
| 129 | `Localize.cpp` | 22 | 2172 | 22 | frei: 22 |
| 130 | `CareerMode.cpp` | 147 | 28424 | 147 | FECareerManager (41), FERaceEvent (29), UnlockTeller (16), FECareerPurchasedParts (11), EventDesc (7), UpgradeInfo (2), frei: 37 |
| 131 | `RCMP_Player.cpp` | 22 | 2912 | 22 | RCMP_Manager (7), MoviePlayerOptions (1), frei: 14 |
| 132 | `../indep/src/animation/NFSSkeleton.cpp` | 6 | 616 | 6 | `../indep/src/animation/NFSSkeleton.cpp` (6) |
| 133 | `FuturePoints.cpp` | 114 | 19200 | 114 | FuturePoints (39), FuturePoint (20), FuturePointsForwards (14), FuturePointsBackwards (14), FuturePointForwards (13), FuturePointBackwards (13) |
| 134 | `NeuQuant.cpp` | 9 | 2756 | 9 | frei: 9 |
| 135 | `CarSkin.cpp` | 23 | 9496 | 1 | IncomingTrafficCar (1), frei: 22 |
| 136 | `TrafficIntersection.cpp` | 19 | 2432 | 19 | TrafficIntersection (19) |
| 137 | `CrossTrafficEmitter.cpp` | 6 | 1056 | 6 | CrossTrafficEmitter (6) |
| 138 | `../indep/src/misc/Base64.cpp` | 0 | 0 | 0 |  |
| 139 | `../indep/src/misc/SubTitle.cpp` | 5 | 1184 | 5 | `../indep/src/misc/SubTitle.cpp` (5) |

## Startcode (Assembler, crt)

Link-Index 140–145 · Pfad: `—` · Port: entfällt

D:/DOCUME~1/tom/LOCALS~1/Temp/crtE1.tmp (0), D:/DOCUME~1/tom/LOCALS~1/Temp/ppc42.tmp (0), D:/DOCUME~1/tom/LOCALS~1/Temp/fil14A.tmp (0), D:/DOCUME~1/tom/LOCALS~1/Temp/pro14B.tmp (0), inituser.c (0), D:/DOCUME~1/tom/LOCALS~1/Temp/tea151.tmp (0)

## EA DVD-/Streaming-Hilfen (FSasync, sndvd)

Link-Index 146–147 · Pfad: `?` · Port: ersetzen

FSasync.c (13), sndvd.c (12)

## C-Laufzeit (newlib, libgcc, libm) Teil 1

Link-Index 148–200 · Pfad: `—` · Port: entfällt (Host-libc)

printf.c (1), sprintf.c (0), vprintf.c (1), vsprintf.c (1), atexit.c (1), qsort.c (1), sn_malloc.c (1), memcpy.c (1), memmove.c (1), memset.c (1), strcasecmp.c (1), strcmp.c (1), strcpy.c (1), strlen.c (1), strncmp.c (1), strncpy.c (1), vfprintf.c (8), vfprintf.c (2), _tolower.c (1), ctype_.c (0), locale.c (2), math_support.c (4), impure.c (0), mbtowc_r.c (1), memchr.c (3), libgcc2.c (0), libgcc2.c (1), libgcc2.c (1), libgcc2.c (1), libgcc2.c (0), libgcc2.c (1), libgcc2.c (1), libgcc2.c (1), libgcc2.c (1), libgcc2.c (1), libgcc2.c (4), e_pow.c (1), e_sqrt.c (1), s_fabs.c (1), ef_pow.c (1), ef_sqrt.c (1), sf_cos.c (1), sf_fabs.c (1), sf_sin.c (1), s_scalbn.c (1), sf_scalbn.c (1), kf_cos.c (1), kf_sin.c (1), ef_rem_pio2.c (1), s_copysign.c (1), sf_copysign.c (1), kf_rem_pio2.c (1), sf_floor.c (24)

## Nintendo Dolphin SDK (OS, DVD, VI, PAD, CARD, GX, EXI, SI, IP)

Link-Index 201–299 · Pfad: `DolphinSDK1.0` · Port: entfällt

PPCArch.c (0), allsrc.c (38), OS.c (15), OSAlarm.c (27), OSAlloc.c (1), OSArena.c (0), OSAudioSystem.c (0), OSCache.c (1), OSContext.c (17), OSError.c (0), OSFont.c (13), OSInterrupt.c (8), OSLink.c (0), OSMemory.c (14), OSMutex.c (0), OSReset.c (7), OSResetSW.c (0), OSRtc.c (17), OSSync.c (3), OSThread.c (28), OSTime.c (1), __ppc_eabi_init.cpp (4), OSReboot.c (18), db.c (0), mtx.c (13), mtxvec.c (0), mtx44.c (0), vec.c (2), dvdfs.c (2), dvd.c (51), dvdqueue.c (0), dvderror.c (4), dvdidutils.c (0), dvdFatal.c (0), fstload.c (4), dvdlow.c (18), vi.c (22), Padclamp.c (2), Pad.c (26), ai.c (11), CARDMount.c (19), CARDNet.c (0), CARDBios.c (16), CARDUnlock.c (6), CARDRdwr.c (5), CARDBlock.c (6), CARDDir.c (3), CARDCheck.c (17), CARDStatEx.c (0), CARDOpen.c (1), GXInit.c (5), GXFifo.c (54), GXAttr.c (0), GXMisc.c (38), GXGeometry.c (0), GXFrameBuf.c (0), GXLight.c (0), GXTexture.c (77), GXBump.c (0), GXTev.c (0), GXPixel.c (0), GXDisplayList.c (0), GXTransform.c (0), GXPerf.c (0), EXIBios.c (23), EXIUart.c (6), SIBios.c (24), SISamplingRate.c (0), SISteering.c (4), SISteeringXfer.c (6), SISteeringAuto.c (3), IPSocket.c (15), IPPPPoE.c (0), IPLcp.c (0), IPPap.c (0), IPChap.c (0), IP.c (0), IPIcmp.c (6), IPRoute.c (4), IPUdp.c (13), IPFrag.c (0), IPEther.c (10), IFFifo.c (0), IFRing.c (4), IPTcp.c (0), IPTcpOutput.c (4), IPTcpTimer.c (3), IPTcpUser.c (20), IPTcpTimeWait.c (0), IPDns.c (4), IPDhcp.c (3), IPZero.c (0), IPOpt.c (0), IPPPP.c (21), IPIpcp.c (0), IPArp.c (0), DebuggerDriver.c (7), AmcExi2Comm.c (8), AmcExi.c (25)

## EA Videospieler (avplayer, RCMP, MAD-Codec)

Link-Index 300–309 · Pfad: `?` · Port: nachbauen (WP-16)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 300 | `avplayer.cpp` | 0 | 0 | 0 |  |
| 301 | `audioplayer.cpp` | 6 | 736 | 0 | RCMP::AUDIO_PLAYER (6) |
| 302 | `avsubtitle.cpp` | 0 | 0 | 0 |  |
| 303 | `rcmpbase.cpp` | 17 | 1132 | 17 | RCMP::DECODER (10), RCMP::CODEC_IDATA (2), RCMP::RCMP_SYSTEM (1), RCMP::CHUNK (1), frei: 3 |
| 304 | `rcmp2real.cpp` | 1 | 36 | 1 | RCMP::RCMP_SYSTEM (1) |
| 305 | `rcmp_mad_codec.cpp` | 16 | 2720 | 16 | MAD_CODEC_INTERNAL (12), MAD_FRAME (2), RCMP (1), frei: 1 |
| 306 | `rcmp_mad_codec_chunk_types.cpp` | 1 | 56 | 1 | RCMP (1) |
| 307 | `maddec.cpp` | 10 | 2844 | 10 | frei: 10 |
| 308 | `maddeca.cpp` | 2 | 456 | 2 | frei: 2 |
| 309 | `madidct.cpp` | 3 | 1080 | 3 | frei: 3 |

## EA Grafikbibliothek (rendercontext, device, model, pcode …)

Link-Index 310–337 · Pfad: `?` · Port: ersetzen (WP-40)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 310 | `dlopen.cpp` | 17 | 6864 | 17 | EAGL::DynamicLoader (13), frei: 4 |
| 311 | `conspool.cpp` | 7 | 500 | 0 | EAGL::ConstructorPool (4), EAGL::RuntimeAllocConstructorPool (3) |
| 312 | `device.cpp` | 2 | 100 | 2 | frei: 2 |
| 313 | `device_cmn.cpp` | 8 | 216 | 8 | EAGL::Device (4), EAGLInternal::DevicePrivate (2), frei: 2 |
| 314 | `rendercontext.cpp` | 31 | 3828 | 31 | EAGL::RenderContextExtension (10), EAGL::RenderContextExtensionBase (6), EAGLInternal::RenderContextPrivate (5), EAGL::RenderContext (4), EAGLInternal::RenderContextPrivate::GPBreak (2), EAGL::RenderContextBase (1), frei: 2 |
| 315 | `rendercontext_cmn.cpp` | 6 | 420 | 6 | EAGL::RenderContextBase (3), EAGLInternal::RenderContextPrivateBase (1), frei: 2 |
| 316 | `state.cpp` | 12 | 2172 | 12 | EAGL::GeoPrimStateExtension (6), EAGL::GeoPrimState (4), frei: 2 |
| 317 | `tar.cpp` | 10 | 1288 | 10 | EAGL::TARExtension (5), EAGL::TAR (3), frei: 2 |
| 318 | `tevstage.cpp` | 10 | 1568 | 10 | EAGL::TevStage (8), frei: 2 |
| 319 | `texturerc_cmn.cpp` | 3 | 224 | 3 | EAGL::TextureRenderContext (1), frei: 2 |
| 320 | `viewport.cpp` | 12 | 4044 | 12 | EAGL::ViewPort (6), EAGLInternal::ViewPortPrivate (2), EAGL::ViewPortExtension (2), frei: 2 |
| 321 | `viewport_cmn.cpp` | 5 | 616 | 5 | EAGL::ViewPort (3), frei: 2 |
| 322 | `profiler_cmn.cpp` | 13 | 4156 | 13 | EAGL::ProfilerRegion (5), EAGL::Profiler (5), frei: 3 |
| 323 | `singledraw.cpp` | 11 | 3912 | 11 | EAGL::DrawGouraud (7), EAGL::DrawArray (4) |
| 324 | `base.cpp` | 2 | 80 | 2 | EAGLInternal (2) |
| 325 | `verbositycontrol.cpp` | 0 | 0 | 0 |  |
| 326 | `SingleDrawGouraud.cpp` | 4 | 2400 | 4 | frei: 4 |
| 327 | `SingleDrawTexture.cpp` | 4 | 2428 | 4 | frei: 4 |
| 328 | `printmessage.cpp` | 1 | 212 | 1 | EAGL (1) |
| 329 | `sympool.cpp` | 6 | 1324 | 0 | EAGL::SymbolPool (6) |
| 330 | `model.cpp` | 6 | 1308 | 6 | EAGL::DynamicModel (4), frei: 2 |
| 331 | `platform_model.cpp` | 0 | 0 | 0 |  |
| 332 | `symbolinit.cpp` | 0 | 0 | 0 |  |
| 333 | `gcbuffer.cpp` | 0 | 0 | 0 |  |
| 334 | `rendermethod.cpp` | 5 | 860 | 5 | EAGL::RenderMethod (3), frei: 2 |
| 335 | `transform.cpp` | 9 | 2620 | 9 | EAGL::Transform (8), frei: 1 |
| 336 | `texturerc.cpp` | 8 | 888 | 8 | EAGL::TextureRenderContext (3), EAGL::TextureRenderContextExtension (2), EAGLInternal::TextureRenderContextPrivate (1), frei: 2 |
| 337 | `pcode.cpp` | 1 | 1192 | 1 | EAGLInternal (1) |

## EAGLAnim (Animationsbibliothek)

Link-Index 338–369 · Pfad: `?` · Port: portieren (indep-nah)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 338 | `Skeleton.cpp` | 3 | 2276 | 3 | EAGLAnim::Skeleton (3) |
| 339 | `FnAnimFactory.cpp` | 0 | 0 | 0 |  |
| 340 | `System.cpp` | 1 | 708 | 1 | frei: 1 |
| 341 | `SystemCmn.cpp` | 2 | 236 | 2 | EAGLAnim::Initializer (1), EAGLAnim (1) |
| 342 | `ScratchBuffer.cpp` | 4 | 232 | 4 | EAGLAnim::ScratchBuffer (2), frei: 2 |
| 343 | `MemoryPoolManager.cpp` | 14 | 2008 | 14 | EAGLAnim::MemoryPoolManager (14) |
| 344 | `RawStateChan.cpp` | 6 | 1280 | 6 | EAGLAnim::FnRawStateChan (6) |
| 345 | `BoneMask.cpp` | 0 | 0 | 0 |  |
| 346 | `AnimBank.cpp` | 5 | 520 | 3 | EAGLAnim::AnimBank (5) |
| 347 | `FnAnim.cpp` | 15 | 156 | 15 | EAGLAnim::FnAnim (15) |
| 348 | `FnAnimMemoryMap.cpp` | 6 | 120 | 6 | EAGLAnim::FnAnimMemoryMap (6) |
| 349 | `CompoundChannel.cpp` | 16 | 3784 | 16 | EAGLAnim::FnCompoundChannel (15), EAGLAnim::CompoundChannel (1) |
| 350 | `FnDeltaQFast.cpp` | 11 | 8608 | 11 | EAGLAnim::FnDeltaQFast (11) |
| 351 | `FnDeltaF3.cpp` | 10 | 8396 | 10 | EAGLAnim::FnDeltaF3 (10) |
| 352 | `FnDeltaQ.cpp` | 7 | 10944 | 7 | EAGLAnim::FnDeltaQ (7) |
| 353 | `FnDeltaF1.cpp` | 10 | 6160 | 10 | EAGLAnim::FnDeltaF1 (10) |
| 354 | `FnDeltaSingleQ.cpp` | 7 | 13264 | 7 | EAGLAnim::FnDeltaSingleQ (7) |
| 355 | `DeltaChan.cpp` | 21 | 7884 | 21 | EAGLAnim::FnDeltaLerpChan (5), EAGLAnim::FnDeltaChan (4), EAGLAnim::FnDeltaQuatChan (3), EAGLAnim::FnKeyDeltaChan (3), EAGLAnim::FnKeyLerpChan (3), EAGLAnim::FnKeyQuatChan (3) |
| 356 | `DeltaCompressedData.cpp` | 4 | 3592 | 4 | EAGLAnim::DeltaCompressedData (4) |
| 357 | `FnEventBlender.cpp` | 3 | 388 | 3 | EAGLAnim::FnEventBlender (3) |
| 358 | `FnRawPoseChannel.cpp` | 5 | 288 | 5 | EAGLAnim::FnRawPoseChannel (5) |
| 359 | `RawPoseChannel.cpp` | 3 | 1448 | 3 | EAGLAnim::RawPoseChannel (3) |
| 360 | `StatelessQ.cpp` | 1 | 144 | 1 | EAGLAnim::StatelessQ (1) |
| 361 | `StatelessF3.cpp` | 1 | 68 | 1 | EAGLAnim::StatelessF3 (1) |
| 362 | `FnPoseBlender.cpp` | 5 | 5820 | 5 | EAGLAnim::FnPoseBlender (5) |
| 363 | `RawEventChannel.cpp` | 1 | 564 | 1 | EAGLAnim::RawEventChannel (1) |
| 364 | `PhaseChan.cpp` | 4 | 836 | 4 | EAGLAnim::FnPhaseChan (4) |
| 365 | `FnRunBlender.cpp` | 19 | 7692 | 19 | EAGLAnim::FnRunBlender (19) |
| 366 | `FnTurnBlender.cpp` | 15 | 4796 | 15 | EAGLAnim::FnTurnBlender (15) |
| 367 | `Attribute.cpp` | 1 | 136 | 1 | EAGLAnim::AttributeBlock (1) |
| 368 | `FnStatelessQ.cpp` | 10 | 2888 | 10 | EAGLAnim::FnStatelessQ (10) |
| 369 | `FnStatelessF3.cpp` | 11 | 3904 | 11 | EAGLAnim::FnStatelessF3 (11) |

## EA Dateisystem (filesys, BIG-Archive)

Link-Index 370–376 · Pfad: `src/file/{gc,cmn}/` · Port: teilweise portieren (WP-43)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 370 | `src/file/gc/filesys.cpp` | 28 | 9560 | 28 | `src/file/gc/filesys.cpp` (28) |
| 371 | `src/file/cmn/filesysopts.cpp` | 4 | 848 | 4 | `src/file/cmn/filesysopts.cpp` (4) |
| 372 | `src/file/cmn/syncfile.cpp` | 7 | 1592 | 7 | `src/file/cmn/syncfile.cpp` (7) |
| 373 | `src/file/cmn/hlafile.cpp` | 13 | 3584 | 13 | `src/file/cmn/hlafile.cpp` (13) |
| 374 | `src/file/cmn/hlsfile.cpp` | 3 | 532 | 3 | `src/file/cmn/hlsfile.cpp` (3) |
| 375 | `src/file/cmn/locatbig.cpp` | 7 | 2580 | 7 | `src/file/cmn/locatbig.cpp` (6), `include/realsystem.h` (1) |
| 376 | `src/file/gc/filedev.cpp` | 15 | 5220 | 15 | `src/file/gc/filedev.cpp` (15) |

## EA RealMath

Link-Index 377–382 · Pfad: `?` · Port: prüfen

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 377 | `rmv3math.cpp` | 0 | 0 | 0 |  |
| 378 | `rmv3arit.cpp` | 0 | 0 | 0 |  |
| 379 | `mathvars.cpp` | 0 | 0 | 0 |  |
| 380 | `rmtrigfl.cpp` | 0 | 0 | 0 |  |
| 381 | `rmv3flot.cpp` | 0 | 0 | 0 |  |
| 382 | `fixdmult.cpp` | 1 | 64 | 1 | frei: 1 |

## Realmc (Speicherkarten-Bibliothek)

Link-Index 383–390 · Pfad: `?` · Port: ersetzen (WP-43)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 383 | `gc_interface.cpp` | 17 | 4836 | 17 | Realmc::GCInterface (12), Realmc::Interface (1), frei: 4 |
| 384 | `public.cpp` | 22 | 3944 | 22 | Realmc::GCInterface (22) |
| 385 | `trctasks.cpp` | 10 | 15756 | 10 | Realmc::GCInterface (10) |
| 386 | `tasks.cpp` | 15 | 4376 | 15 | Realmc::GCInterface (14), frei: 1 |
| 387 | `gc_driver.cpp` | 37 | 7556 | 37 | Realmc::GCDriver (37) |
| 388 | `blockcalculator.cpp` | 6 | 312 | 6 | Realmc::BlockCalculatorImp (6) |
| 389 | `memvectors.cpp` | 8 | 264 | 8 | Realmc (8) |
| 390 | `interfaceimp.cpp` | 4 | 428 | 4 | Realmc::InterfaceImp (4) |

## EA RealCore (Threads, Timer, Ausgabe)

Link-Index 391–409 · Pfad: `e:/core/realcore/5/` · Port: ersetzen

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 391 | `locale.cpp` | 5 | 1180 | 5 | Realmc::Locale (5) |
| 392 | `systemvars.cpp` | 0 | 0 | 0 |  |
| 393 | `inittmr.cpp` | 5 | 532 | 5 | frei: 5 |
| 394 | `../indep/src/misc/Timer.cpp` | 3 | 72 | 3 | frei: 3 |
| 395 | `threads.cpp` | 10 | 832 | 10 | frei: 10 |
| 396 | `mutex2.cpp` | 6 | 540 | 0 | frei: 6 |
| 397 | `systask.cpp` | 0 | 0 | 0 |  |
| 398 | `signals.cpp` | 3 | 144 | 3 | frei: 3 |
| 399 | `printstr.cpp` | 8 | 1156 | 8 | frei: 8 |
| 400 | `abortmsg.cpp` | 2 | 476 | 2 | frei: 2 |
| 401 | `memclear.cpp` | 2 | 104 | 0 | frei: 2 |
| 402 | `debugger.cpp` | 0 | 0 | 0 |  |
| 403 | `timerthread.cpp` | 4 | 352 | 1 | frei: 4 |
| 404 | `printn3.cpp` | 1 | 44 | 1 | frei: 1 |
| 405 | `memfill.cpp` | 1 | 324 | 1 | frei: 1 |
| 406 | `memcopy.cpp` | 1 | 504 | 1 | frei: 1 |
| 407 | `exit.cpp` | 4 | 320 | 2 | frei: 4 |
| 408 | `initvblt.cpp` | 0 | 0 | 0 |  |
| 409 | `vbltimr.cpp` | 0 | 0 | 0 |  |

## EA Schrift und Lokalisierung (font*, loccore)

Link-Index 410–420 · Pfad: `?` · Port: portieren

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 410 | `fontchar.cpp` | 1 | 64 | 1 | frei: 1 |
| 411 | `fontcreate.cpp` | 2 | 120 | 2 | frei: 2 |
| 412 | `fontdraw.cpp` | 3 | 172 | 3 | frei: 3 |
| 413 | `fontdrawf.cpp` | 1 | 204 | 0 | frei: 1 |
| 414 | `fontdriver.cpp` | 0 | 0 | 0 |  |
| 415 | `fontinit.cpp` | 0 | 0 | 0 |  |
| 416 | `fontkern.cpp` | 3 | 272 | 3 | frei: 3 |
| 417 | `fontnull.cpp` | 2 | 32 | 1 | frei: 2 |
| 418 | `fontstdf.cpp` | 0 | 0 | 0 |  |
| 419 | `loccore.cpp` | 4 | 752 | 4 | frei: 4 |
| 420 | `ustrcpy.cpp` | 2 | 360 | 2 | frei: 2 |

## EA SHAPE (Texturformat)

Link-Index 421–432 · Pfad: `?` · Port: portieren (WP-13)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 421 | `creates.cpp` | 6 | 1388 | 6 | frei: 6 |
| 422 | `shpclut.cpp` | 1 | 104 | 1 | frei: 1 |
| 423 | `shpdepth.cpp` | 1 | 24 | 1 | frei: 1 |
| 424 | `shpdestroy.cpp` | 1 | 48 | 1 | frei: 1 |
| 425 | `shpisclut.cpp` | 1 | 108 | 0 | frei: 1 |
| 426 | `shplongname.cpp` | 0 | 0 | 0 |  |
| 427 | `shpmemalloc.cpp` | 1 | 16 | 1 | frei: 1 |
| 428 | `shpsize.cpp` | 2 | 684 | 0 | frei: 2 |
| 429 | `shpsubs.cpp` | 0 | 0 | 0 |  |
| 430 | `shptype.cpp` | 1 | 156 | 1 | frei: 1 |
| 431 | `swizzlesize.cpp` | 4 | 112 | 4 | frei: 4 |
| 432 | `cluttype.cpp` | 1 | 84 | 1 | frei: 1 |

## CSIS (Sound-Schnittstelle)

Link-Index 433–433 · Pfad: `?` · Port: portieren (WP-15/42)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 433 | `csis.cpp` | 38 | 3976 | 38 | Csis::Class (13), Csis::System (8), Csis::ClassHandle (4), Csis::Function (3), Csis::GlobalVariable (3), Csis::FunctionHandle (2), frei: 2 |

## EA Audio (SNDI, AEMS, Mixer, Decoder)

Link-Index 434–570 · Pfad: `?` · Port: nachbauen (WP-15/42)

saems.c (74), saemsamb.c (3), saemsstr.c (18), saemstimupdt.c (1), salloc.c (4), sbadd.c (2), sballoc.c (2), sbhdrcpy.c (1), sbhdrsze.c (1), sbplay.c (1), sbremove.c (1), sbvalid.c (1), scalcfx.c (1), sdata.c (0), sdfx.c (3), sdspmix.c (8), seffect.c (4), sevent.cpp (0), sfxlevel.c (1), sfxrevc.c (7), sgetdata.c (1), sinitdts.c (1), smemcpy.c (1), smemman.c (5), smixer.c (16), snddrv.c (47), spatkey.c (1), spitch.c (1), spktplay.c (14), splysdef.c (1), spoutlat.c (1), srandom.c (2), srender.c (1), sresopat.c (1), sserver.c (8), ssine.c (1), sst.c (20), sst3dpos.c (1), sstcrtap.c (1), sstfxlev.c (1), sstgetrp.c (1), ssthighp.c (1), ssthold.c (1), sstlowp.c (1), sstop.c (1), sstovrhd.c (2), sstpmult.c (1), sstqreqi.c (1), sstrmdry.cpp (1), sstrstat.c (1), sstsetgl.c (1), sststat.c (1), ssttmul.c (1), sstvol.c (2), ssys.cpp (9), ssysinit.c (5), ssysserv.c (3), ssysveccsismutex.cpp (3), stagpat.c (3), stimemul.c (1), stpparse.c (1), stretch.c (9), svecreal.cpp (1), svol.c (1), sx87d16.c (1), author.cpp (0), coda.cpp (2), s3dlow.c (1), saramman.c (6), satospkr.c (4), sclcptch.c (2), sctlfilt.c (1), sctrldry.cpp (1), sdownmix.cpp (1), sfamplf.c (3), sfbpffir8.c (3), sfecho.c (4), sfft24.c (2), sfhpffir8.c (3), sfilter.c (3), sfir.c (1), sfir8.c (2), sflpf.c (3), sflpffir8.c (3), sfmixer.c (3), sformat.c (0), sfreson.c (4), sfsplit.c (3), sfsrc.c (3), sgettag.c (1), shipass.c (1), sinit16.c (1), sinitut.c (1), sinitxa.c (1), slib.c (1), slinklst.c (5), slinkmix.c (1), slowpass.c (1), smemdis.c (0), smemhigh.cpp (1), smixc.c (1), smixfilt.c (1), smixfram.c (1), smixhip.c (1), smixlowp.c (1), smixptch.c (1), smixtmul.c (1), smixvec.c (0), SNDI_cos.c (1), SNDI_mult16.c (1), SNDI_root1x.c (1), SNDI_sin.c (1), sover.c (1), spantoaz.c (1), spat2hdr.c (1), spktctoh.c (1), srrange.c (1), sstopall.c (1), ssysreal.c (3), supf.c (3), suplf.c (3), supmutf.c (4), supmutlf.c (4), supmutpf.c (3), suppf.c (2), supxaf.cpp (4), supxalf.cpp (4), supxapf.cpp (3), eaxadecf.cpp (9), mtdecf.cpp (16), sattrdef.c (1), sexithndl.c (1), sfrsf.c (3), sgparse.cpp (1), SNDI_findprime.c (2), scrsfl.c (1), dummy.c (7)

## C-Laufzeit (newlib stdio, libm) Teil 2

Link-Index 571–624 · Pfad: `—` · Port: entfällt

tors.c (0), isprint.c (1), isspace.c (0), tolower.c (1), puts.c (2), sscanf.c (0), vfscanf.c (0), atof.c (0), atoi.c (1), bsearch.c (1), strtod2.c (0), strtoul.c (0), memcmp.c (1), strchr.c (1), strstr.c (1), strtol.c (2), isdigit.c (0), errno.c (0), fread.c (0), fvwrite.c (1), refill.c (0), ungetc.c (0), wsetup.c (1), fflush.c (1), fopen.c (3), fseek.c (0), fwalk.c (1), makebuf.c (1), sn_buf.cpp (0), stdio.c (4), closer.c (1), fstatr.c (1), lseekr.c (1), openr.c (0), readr.c (1), sbrkr.c (0), writer.c (1), flags.c (0), e_acos.c (0), e_atan2.c (0), s_atan.c (0), s_cos.c (0), s_sin.c (1), s_tan.c (0), ef_atan2.c (0), sf_atan.c (1), sf_tan.c (1), k_cos.c (1), k_sin.c (1), k_tan.c (0), e_rem_pio2.c (1), kf_tan.c (1), k_rem_pio2.c (1), s_floor.c (8)

## Nintendo Dolphin SDK (OSFatal, AR, AX, DSP, CARD)

Link-Index 625–648 · Pfad: `DolphinSDK1.0` · Port: entfällt

OSFatal.c (0), OSMessage.c (0), ar.c (30), arq.c (6), AX.c (0), AXAlloc.c (2), AXAux.c (2), AXCL.c (2), AXOut.c (24), AXSPB.c (2), AXVPB.c (2), AXProf.c (0), AXComp.c (0), DSPCode.c (0), dsp.c (14), dsp_debug.c (0), dsp_task.c (0), CARDFormat.c (3), CARDCreate.c (4), CARDRead.c (3), CARDWrite.c (4), CARDDelete.c (5), CARDStat.c (4), CARDRename.c (2)

## Netzwerk-Hilfen (eth, md5, base64)

Link-Index 649–652 · Pfad: `?` · Port: Stub (WP-44)

eth.c (10), ethsec.c (4), md5.c (6), base64.c (2)

## EA Codec (refsize)

Link-Index 653–653 · Pfad: `src/codec/cmn/` · Port: portieren (WP-12)

| # | Objekt | Funktionen | Bytes | sicher | Inhalt (Unterdateien laut DWARF oder häufigste Klassen) |
|---:|---|---:|---:|---:|---|
| 653 | `src/codec/cmn/refsize.cpp` | 1 | 784 | 1 | `src/codec/cmn/refsize.cpp` (1) |

## Inline- und Template-Funktionen am Textende (linkonce)

(frei) (17), NullController (16), EAGLAnim::FnDefaultAnimBank (8), TrackRoutePoint (7), FEGameInterface (7), NullDriver (6), EAGLAnim::FnCycle (6), EAGLAnim (6), EAGLAnim::FnRawEventChannel (5), Realmc::TaskManager (5), DrivingLocation (4), DrivingChoice (4), EAGLAnim::FnRawLinearChannel (4), EAGLAnim::FnPoseMirror (4), Realmc::GCMessage (4), Player (3), FECareerManager (3), FERefList (3), FEPoolNode (3), EA::Allocator::DefaultIAllocatorImpl (3), EAGLAnim::FnGraft (3), TableBase (2), Car (2), bVector3 (2), SCEA_GJKsimplexPair (2), tAverage (2), AverageBase (2), FEMinList (2), FEngSetDirtyFlagsCallback (2), FEngMovieStopper (2), RenderObjectDisconnect (2), ObjectDirtySetter (2), DefaultMenuScreen (2), FEButtonCounter (2), FEButtonEnumerator (2), FEString (2), FEGroup (2), FEImage (2), FEColoredImage (2), FEMovie (2), FEFindByHash (2), FEFindByGUID (2), FEGetNumSpawnResponses (2), FESetSpawnResponses (2), FEAnimImage (2), FESimpleImage (2), FEModel (2), eLightContext (2), SemiWheel (2), Trolley (2), FuturePoint (2), EAGLAnim::DeltaQFast (2), EAGLAnim::DeltaQ (2), Realmc::TaskTrc (2), Realmc::CmnFileDescriptor (2), Realmc::GcFileDescriptor (2), eRenderStageQueue (1), GrandSceneryCullInfo (1), eTextureBucket (1), eDataRender (1), AcidEmitter (1), AcidEffectSystem (1), Timer (1), RideInfo (1), cPlayerSettings (1), RaceCoordinator (1), RacingCar (1), CarDamage (1), CarPhysicsInfo (1), RigidBodyInfo (1), HoleFillerNode (1), VisibleSectionManager (1), DriveTarget (1), Avoidable (1), NearbyAvoidableList (1), SpaceNode (1), DifficultyInfo (1), PhysicsVehicle (1), DriveTrain (1), MiniWorld (1), FEVector3 (1), ListPicker (1), FERaceEventCar (1), RewardNotifier (1), StyleAnimator (1), CarTradeManager (1), MiniWorldSegment (1), CarOrbiter (1), SelectableTrack (1), MultiTrackSelector (1), FECareerPurchasedParts (1), FECareerMagazine (1), ColorElement (1), BootFlowScreen (1), BootFlowManager (1), MPCarSelectData (1), PresetNode (1), FEObjectSorter (1), FEList (1), FEObjectCallback (1), FEPackageCommand (1), FEMessageNode (1), cFEngRender (1), ListPicker::ListPickerItem (1), FEngMovieStarter (1), FEngHidePCObjects (1), ObjectVisibilitySetter (1), FEMinNode (1), FEPackageManager (1), FEColoredImageData (1), FEVector2 (1), FEQuaternion (1), FEHashNodePtr (1), FEKeyNode (1), PackageInitStateCB (1), FEObjectComment (1), FEBuildMsgs (1), FEFieldNode (1), FETypeNode (1), FESlotNode (1), FESlotPool (1), ResourceConnector (1), MagazineTextureHandler (1), DecalColorPair (1), stEAXSndGameControl (1), stEAXS_VerticalCylinderParams (1), stEAX_WindParams (1), EAXWheels (1), fVectorX (1), fVectorY (1), fVectorZ (1), tShaker (1), FlatMobileTopology (1), WorldEntityType (1), DrawBridge (1), CarPartDatabase (1), CarTypeInfoIterator (1), RigidBody (1), RigidBody::RigidBodyState (1), Wheel (1), Suspension (1), CarLoader (1), SmokeableInfo (1), FinishedRaceStatsEntry (1), FERaceEventTrack (1), RCMP::AV_MS_TIMER (1), RCMP::RCMP_SYSTEM (1), RCMP::CODEC (1), EAGL::GCBuffer (1), EAGLAnim::FnAnimSuper (1), EAGLAnim::FnDeltaLerpChan (1), EAGLAnim::FnDeltaQuatChan (1), EAGLAnim::FnKeyLerpChan (1), EAGLAnim::FnKeyQuatChan (1), EAGLAnim::FnPhaseChan (1), Realmc::FindInfoStruct (1), Realmc::GcTask (1), Realmc::TaskTrcStartGame (1), Realmc::TaskTrcGetCardInfo (1), Realmc::TaskTrcSaveFile (1), Realmc::TaskTrcListFiles (1), Realmc::TaskTrcLoadFile (1), Realmc::TaskTrcDeleteFile (1), Realmc::BlockCalculator (1), Realmc::DeviceDriver (1), Realmc::BaseInterface (1)

