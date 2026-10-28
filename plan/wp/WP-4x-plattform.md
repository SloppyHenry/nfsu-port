# WP-40 … WP-44 Plattformschicht (SDL3, alle Zielplattformen)

**Phase 4 · Voraussetzungen: WP-04 (COM-Vtables), WP-10; je Teilpaket eine Session oder mehr**

Grundsatz: Der Spielcode spricht **eigene, schmale Schnittstellen** an (`port/src/platform/*.h`, `port/src/render/*.h`). Diese werden auf SDL3 und Metal umgesetzt. Grundlage ist die Liste aller tatsächlich benutzten DirectX- und Win32-Aufrufe aus WP-02 (`kind=win32glue`) und WP-04 (`com_vtables.csv`).

## WP-40 Renderer (Modell: Opus, 3–6 Sessions)
- Schnittstelle nach den tatsächlich genutzten D3D9-Funktionen: Texturen, Vertex- und Indexbuffer, Effekte bzw. Shader, Renderzustände, Rendertargets (Reflexionen, Motion Blur, Filter-Effekt), Present, Auflösung und Fenster
- Umsetzung mit SDL_GPU (Metal, Vulkan, D3D12): Pipeline-Cache je Zustandskombination, Shader aus WP-17
- Breitbild und hohe Auflösung als Option. Die vorhandenen `uniws`-Patches zeigen, wo das Original das Seitenverhältnis festlegt.
- Fertig, wenn: Viewer (WP-18) und Frontend-Darstellung laufen über diese Schicht.

## WP-41 Eingabe (Sonnet, 1 Session)
- DirectInput8 (Tastatur, Gamepad, Lenkrad, Force Feedback) → SDL3 Keyboard und Gamepad
- Tastenbelegung so übernehmen, wie das Original sie speichert
- Fertig, wenn: Menüsteuerung und Fahren per Tastatur und Controller gehen (Test mit WP-50/51).

## WP-42 Audio-Ausgabe (Sonnet, 1–2 Sessions)
- DirectSound-Puffer und 3D-Sound bzw. EAX-Ersatz → SDL3-Audio-Stream mit eigenem Mischer (Lautstärke, Pan, Pitch, Loop, Streaming)
- Fertig, wenn: Musikstream, Menügeräusche und mehrere Motorstimmen gleichzeitig klingen.

## WP-43 System, Dateien, Zeit, Speicherstände (Sonnet, 1–2 Sessions)
- Win32-Nachrichtenschleife (`pump_window_messages`) → SDL-Events
- `QueryPerformanceCounter` → SDL-Zeit; Threads und Events → std::thread/SDL
- Registry-Zugriffe (ADVAPI32) → Konfigurationsdatei
- Speicherstände (SHFOLDER: Eigene Dateien) → `SDL_GetPrefPath("NFSU", "NFSU")` (je Plattform der übliche Ort), im Originalformat (siehe `savestate` in `decompiled/speed`)
- Doppelstart-Sperre und CD-Prüfung (`frontend_disc_check`) entfallen bzw. liefern Erfolg.
- Fertig, wenn: Booten ohne einen Win32-Aufruf möglich ist (Linkprüfung: keine Win32-Symbole).

## WP-44 Netzwerk (Sonnet, < 1 Session)
- Online, LAN und Modem (WS2_32, TAPI) → als „nicht verfügbar“ stillgelegt, Menüpunkte ausgegraut
- Ein späterer LAN-Nachbau ist möglich, aber nicht Ziel.
