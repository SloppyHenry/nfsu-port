# WP-19 Plattformunabhängige Grafik und Build

**Phase 2 · Voraussetzungen: WP-10, WP-17, WP-18 · Modell: Opus, 1–2 Sessions**

Anlass: Nutzerentscheidung D10 (2026-10-28) – der Port soll auf macOS, Linux und Windows laufen. Bisher ist nur die Grafik an Metal gebunden.

## Stand vorher (geprüft 2026-10-28)
- `port/src/render/shaders/*.metal` (7 Effekte, MSL), gebaut mit `xcrun metal` → `nfsu.metallib` (`port/CMakeLists.txt`, nur unter APPLE)
- `port/src/platform/window.cpp:20` erzwingt `SDL_GPU_SHADERFORMAT_MSL`, `apps/viewer/main.cpp` `SDL_GPU_SHADERFORMAT_METALLIB`
- Rest (Formate, Spielcode, Pfade, Audio, Video über libavcodec) ist bereits plattformneutral; zlib über `find_package(ZLIB)`

## Aufgaben
1. Shader einmal in HLSL (SM 5/6, Inhalt 1:1 zu den MSL-Fassungen bzw. den `.efx`-Vorlagen) unter `port/src/render/shaders/`.
2. Übersetzung zur Bauzeit mit SDL_shadercross (oder DXC + SPIRV-Cross) nach SPIR-V, DXIL und MSL; Abhängigkeit braucht Rückfrage (Download).
3. Device mit allen verfügbaren Formaten anlegen, Shaderformat zur Laufzeit per `SDL_GetGPUShaderFormats` wählen.
4. ADR-003 anpassen (MSL-only verwerfen, Begründung).
5. CMake: kein Apple-Sonderweg mehr außer Bundle (WP-53); Presets für Linux/Windows.
6. Wenn möglich Probelauf unter Linux (VM/Container) mit dem Viewer.

## Fertig, wenn
- Viewer rendert auf macOS über die neuen Shader wie vorher (Sichtvergleich)
- Build unter Linux und Windows kompiliert (mindestens CI); Lauf dort ehrlich als getestet/ungetestet vermerkt
