# WP-17 Shader und Renderzustände

**Phase 2 · Modell: Opus · Umfang: 2 Sessions · Voraussetzungen: WP-10, WP-04 (D3D9-Vtable-Namen)**

## Befund
`fx/*.efx` (car, world, worldbone, worldheadlight, worldreflect, worldheadlightreflect, filter, blank.tag) sind **D3DX-Effekt-Quelltexte (HLSL)**, jedes Byte XOR 0xAA. Das Original kompiliert sie zur Laufzeit mit dem statisch gelinkten D3DX-Effektcompiler. Dieser Compiler wird **nicht** dekompiliert.

## Schritte
1. `nfsu-tool efx-decode` schreibt die Klartexte nach `refs/fx/`. Die Klartexte nicht committen, sie sind Spielinhalt.
2. Je Effekt aufschreiben: Techniques und Passes, Shader-Modelle (vs/ps 1.x–2.0), Renderzustände, `shared`-Parameter und deren Semantiken (`WORLDVIEWPROJECTION`, `DIFFUSEMAP`, `BASEALPHAREF` …).
3. Im Export alle Stellen finden, an denen das Spiel Effektparameter setzt (`ID3DXEffect::Set*` über Vtable-Offsets) und Fixed-Function-Zustände nutzt (`SetRenderState` und `SetTextureStageState` am Device). Ergebnis: `plan/RENDER-ZUSTAENDE.md`.
4. Portierungsweg festlegen (`port/docs/ADR-003-shader.md`):
   - HLSL über SDL_shadercross bzw. DXC nach MSL
   - oder die Shader in MSL bzw. als SDL_GPU-Shader von Hand neu schreiben. Sie sind klein und stammen aus der SM1/2-Zeit. Empfehlung: neu schreiben, aber 1:1 zur Vorlage.
   - Fixed-Function-Pfade nachbilden
5. `world.efx` als erster Shader im Viewer (WP-18).

## Fertig, wenn
- Alle Effekte sind dokumentiert, das ADR steht und der Weltshader läuft im Viewer.
