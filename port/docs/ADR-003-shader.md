# ADR-003: Shader und Renderzustände

Status: vorgeschlagen (WP-17, 2026-10-27) · Beschreibung der Effekte: `plan/RENDER-ZUSTAENDE.md`

## Kontext
Das Original lädt `fx/*.efx` (HLSL-Effektquelltext, XOR 0xAA) und kompiliert ihn mit dem statisch gelinkten D3DX9-Effektcompiler (0x59ed56–0x60fa41, ≈ 1 132 Funktionen). Alle Shader sind `vs_1_1`/`ps_1_1`, 7 Effekte, rund 20 Techniken, ein Drittel davon Fixed-Function-Fallbacks.

## Entscheidung
1. **Der D3DX-Effektcompiler wird nicht portiert.** Kein Laufzeit-Compile.
2. **Shader werden von Hand in MSL neu geschrieben, 1:1 zur Vorlage** (Funktionsnamen, Formeln, Konstanten wie im `.efx`), eine `.metal`-Datei je Effekt in `port/src/render/shaders/`, mit `xcrun metal` zur Build-Zeit zu einer `.metallib` übersetzt; SDL_GPU nimmt MSL direkt. Begründung: wenige Zeilen, SM 1.x, kein Werkzeug (DXC/shadercross) nötig, Ergebnis bleibt lesbar und vergleichbar. Alle sieben Effekte liegen in `port/src/render/shaders/` vor (`world`, `world_variants` mit headlight/reflect/headlightreflect/bone, `car`, `filter`), alle kompilieren mit `xcrun metal`, keiner ist gelaufen.
3. **Fixed-Function-Techniken entfallen.** Sie existieren nur als Rückfall für alte Karten (Technikwahl per `ValidateTechnique`); der Port nimmt immer die Shadertechnik (Index 0). Die Mehrpassvarianten (`_3_pass`, `_2pass`) sind ebenfalls Rückfälle und werden nicht nachgebildet.
4. **Effektzustände** (`Base*`-Alpha/Blend, Cull, ZEnable/ZWrite/ZFunc, Sampler-Adressierung) werden als SDL_GPU-Pipelinezustand modelliert. `AlphaRef`-Test wird im Fragmentshader per `discard` ausgeführt (SDL_GPU kennt keinen Fixed-Function-Alphatest).
5. **Matrizen**: Host lädt D3D-Zeilenmatrizen unverändert, MSL rechnet `v * M`.
6. **Parameter**: eine Strukturansicht je Effekt (Uniform-Puffer) statt namensbasierter Auflösung; die 47 Namen aus `plan/RENDER-ZUSTAENDE.md` bleiben als Dokumentation.

## Folgen
- `port/CMakeLists.txt` baut auf macOS `build/<preset>/shaders/nfsu.metallib` (Ziel `nfsu_shaders`); geprüft: Build erfolgreich.
- `ps_1_1` sättigt Ausgaberegister auf [0,1]; jeder Fragmentshader gibt deshalb `saturate(...)` zurück. Zwischenwerte (Register-Genauigkeit, Clamp auf [-1,1]) werden nicht nachgebildet; Abweichungen im Hellen sind erst mit Vergleichsbildern prüfbar.
- Das Kriterium „Weltshader läuft im Viewer“ hängt an WP-18 (`apps/viewer` existiert noch nicht).

## Verworfen
- HLSL → MSL über SDL_shadercross/DXC: neue Abhängigkeit, SM1.x-Syntax (`float4 color : COLOR`, `tex2D` auf `sampler`-Objekten, Techniken) erfordert ohnehin Vorverarbeitung.
- Eigenen Effekt-Interpreter bauen: unnötig, da Techniken und Pässe hier statisch bekannt sind.
