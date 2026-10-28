# Effekte und Renderzustände (WP-17)

Quelle: `fx/*.efx` (XOR 0xAA, `nfsu-tool efx-decode` → `refs/fx/`, nicht committen), Ladecode `init_render_devices` (0x40d1d0), Setzstellen in 0x40dd10, 0x40f2a0, 0x40eff0, 0x40e700, 0x40ea30, 0x40d7f0, 0x40d8f0, 0x40db50. Stand: statisch gelesen, **nicht** im Lauf bestätigt.

## 1. Ladevorgang
- `init_render_devices` legt **7 Effektdatensätze** an (Index 0–6, je 0xE0 Byte ab 0x7123f8). Dateinamen in der Tabelle ab 0x6f3690, Stride 0x80.
- Zuordnung Index → Datei: 0 `world`, 1 `worldheadlight`, 2 `worldreflect`, 3 `worldheadlightreflect`, 4 `worldbone`, 5 `car`, 6 `filter`.
- Je Datensatz: Vertexdeklaration aus Tabelle 0x6f3270 (7 × 0x80 Byte, Paare Typ/Verwendung), Datei lesen, XOR 0xAA, NUL anhängen, D3DX-Effekt kompilieren (0x5a314f). Danach **Techniken der Reihe nach** probieren (`ValidateTechnique`, Vtable +0xF4); die erste gültige wird `SetTechnique` (+0xEC) und gemerkt.
  - Technikindex 0 (Shaderpfad) → `DAT_0072b230 = 1`; Index > 0 (Fixed Function) → `DAT_0072b22c = 1`. Das erklärt die Reihenfolge in den Dateien: Shadertechnik zuerst, Fixed-Function-Fallbacks danach. Auf bestimmten Karten (Vendor 0x1039/0x8086/0x10de/0x102b, Geräte-IDs in 0x40d1d0) werden Techniken übersprungen (Treiberumgehungen).
- 47 Parameternamen ab 0x6f3a10 (Stride 0x40) werden per `GetParameterByName` (+0x24) aufgelöst; Handle des Parameters k liegt im Datensatz bei Wortindex 7+k (`DAT_00712414 + rec*0xE0`). `OnResetDevice` (+0x114) und erneutes Auflösen: 0x40db50.

Parameter k: 0 WorldViewProj · 1 ViewProj · 2 WorldView · 3 World · 4 View · 5 Proj · 6 BlendMatrices · 7 HeadlightWVP · 8 LocalEyePos · 9 diffusemap · 10 HeadlightTex · 11 HarmonicCoeff · 12 AmbientCoeff · 13 DiffuseMin · 14 DiffuseRange · 15 SpecularMin · 16 SpecularRange · 17 SpecularPower · 18 EnvmapMin · 19 EnvmapRange · 20 EnvmapPower · 21 HeadlightColor · 22 HeadlightPos · 23 HeadlightFarClip · 24 HeadlightClipTex · 25 EnvMapTex · 26 ReflectedTex · 27 ReflectedProj · 28 ReflectedTexMin · 29 FilterWeights · 30 FilterOffsets · 31–34 FilterTexture0–3 · 35 FogOffset · 36 FogScale · 37 FogFalloffY · 38 FogColor · 39 HeadlightClipMat · 40–44 BaseAlphaTestEnable/Ref/BlendEnable/SrcBlend/DestBlend · 45 TextureOffset · 46 TextureOffsetMatrix.
Nicht jede Datei deklariert jeden Namen; fehlende Handles bleiben leer. `ViewProj`, `World`, `View`, `Proj`, `HeadlightPos`, `HeadlightFarClip`, `Fog*` und `FilterOffsets` kommen in keiner `.efx` vor (nur Namenstabelle).

## 2. ID3DXEffect-Vtable (D3DX9 Sommer 2003, aus den Aufrufen abgeleitet)
Die Setz-Slots liegen im 8-Byte-Raster, konsistent mit dem D3DX9-Header: +0x24 GetParameterByName · +0x30 GetTechnique · +0x68 SetInt · +0x78 SetFloat · +0x88 SetVector · +0x90 SetVectorArray · +0x98 SetMatrix · +0xA0 SetMatrixArray · +0xD0 SetTexture · +0xEC SetTechnique · +0xF4 ValidateTechnique · +0x100 Begin · +0x104 Pass · +0x108 End · +0x114 OnResetDevice. +0x38 = GetPass(Technik, i) (Schleife bis 16 zählt die Pässe, Ergebnis im Datensatz +0x14), +0x30 = GetTechnique(i). Nach Wahl der Technik setzt 0x40d1d0 per SetInt (+0x68) die Standardblendung BaseSrcBlend (+0xC8) = 5 (SRCALPHA) und BaseDestBlend (+0xCC) = 6 (INVSRCALPHA). Datensatz: +0x08 = 2 (Technik gültig), +0x0C Technikindex, +0x10 Technik-Handle, +0x14 Passanzahl.

## 3. Setzstellen
| Funktion | Parameter (k) | Aufruf |
|---|---|---|
| 0x40dd10 (Zeichenschleife) | Technik wechseln: `End` des alten, Callback `rec+0xE0·`, `SetVertexDeclaration`, `Begin(&Passes,0)`; je Teil: `Pass(i)` (+0x104) vor `DrawIndexedPrimitive(TRIANGLELIST)`; Streifen ohne Index über `DrawPrimitiveUP(TRIANGLESTRIP,…,stride 0x18)` mit `ZENABLE=0`/`1` (RenderState 0xE) | – |
| 0x40f2a0 (Objektmatrix) | Shaderpfad: 0 WorldViewProj, bei Layout 5 (car) zusätzlich 2 WorldView; Fixed: `SetTransform(WORLD=0x100)`, bei Wechsel `VIEW=2`/`PROJECTION=3` | SetMatrix |
| 0x40eff0 (Scheinwerfer) | 7 HeadlightWVP, 39 HeadlightClipMat (Skalierung 0,005 und +0,5), 21 HeadlightColor (3 Floats + 0) | SetMatrix/SetVector |
| 0x40e700 (Fahrzeugbeleuchtung) | 8 LocalEyePos, 11 HarmonicCoeff[10], 12 AmbientCoeff; Fixed: `SetLight` ×3 + `LightEnable` | SetVector/SetVectorArray/SetInt |
| 0x40ea30 (Fahrzeugmaterial) | 13–16 Diffuse/Specular Min/Range, 18/19 Envmap Min/Range (SetVector), 17 SpecularPower, 20 EnvmapPower (SetFloat); Fixed: `SetMaterial`, RenderState 0x3C (`D3DRS_AMBIENT`) | SetVector/SetFloat |
| 0x40d7f0, 0x40d8f0 (Spiegelung) | 26 ReflectedTex (SetTexture), 28 ReflectedTexMin (SetFloat, Wert 0,1), 27 ReflectedProj (SetMatrix; Matrix mit 0,5-Verschiebung) | – |
| 0x40f4f0 | 40 BaseAlphaTestEnable, 41 BaseAlphaRef (SetInt); ohne Effekt `SetRenderState(ALPHATESTENABLE=0xF / ALPHAREF=0x18)` | SetInt |
| 0x40f3f0 | 42 BaseAlphaBlendEnable, 43 BaseSrcBlend, 44 BaseDestBlend (SetInt); ohne Effekt RenderState 0x1B/0x13/0x14 | SetInt |
| 0x40e310 (Abschnittsgrenze, Zustandswechsel je Material) | 9 diffusemap (SetTexture), ruft 0x40f4f0/0x40f3f0; CULLMODE (0x16) 1/2, ZWRITE (0xE); 45 TextureOffset (SetVector), 46 TextureOffsetMatrix (SetMatrix) | – |
| 0x40b420 (Quad mit Textur) | 9 diffusemap, danach `DrawPrimitiveUP(TRIANGLEFAN,2,…,Stride 0x20)` | SetTexture |
| 0x40f190 | 6 BlendMatrices (SetMatrixArray, 16) | SetMatrixArray |
| 0x40d790 (Beginn-Callback Index 1) | 10 HeadlightTex (Textur zu Hash `HEADLIGHTS`), 24 HeadlightClipTex (globale Textur 0x7123f4) | SetTexture |
| 0x40dac0 (Beginn-Callback Index 5) | 25 EnvMapTex (globale Textur 0x72b284) | SetTexture |
| 0x40daf0 (Beginn-Callback Index 6) | `SetTextureStageState(0..3, TEXTURETRANSFORMFLAGS=24, DISABLE)` | – |
| 0x40c160, 0x40c440, 0x40c7b0 (Filterdurchläufe, Effekt 6 über `0x712950`) | 29/30 FilterWeights/FilterOffsets (SetVectorArray), 31–34 FilterTexture (SetTexture) | – |
| 0x40d720 (Technikwechsel) | `End` + End-Callback des alten, Beginn-Callback des neuen, `SetVertexDeclaration`, `Begin`, `Pass(0)` | – |

**Nicht gesetzt** (Namen ohne Parameter in allen `.efx`, Handle bleibt leer): ViewProj, World, View, Proj, HeadlightPos, HeadlightFarClip, Fog* (vier). Kein Aufruf im Code schreibt sie.

Datensatzoffsets (Handle von Parameter k bei Byte `(7+k)·4`): diffusemap +0x40, HeadlightTex +0x44, Harmonic +0x48, Ambient +0x4C, Diffuse/Specular/Envmap +0x50…+0x6C, HeadlightColor +0x70, HeadlightClipTex +0x7C, EnvMapTex +0x80, ReflectedTex +0x84, ReflectedProj +0x88, ReflectedTexMin +0x8C, Filter +0x90…+0xA4, Base* +0xBC…+0xCC, TextureOffset +0xD0, TextureOffsetMatrix +0xD4, **Beginn-Callback +0xD8, End-Callback +0xDC** (Tabellen 0x6f45d0 / 0x6f45ec).

Beginn-Callbacks je Effekt: 0 world: leer · 1 worldheadlight: 0x40d790 · 2 worldreflect: 0x40d7f0 · 3 worldheadlightreflect: 0x40d8f0 · 4 worldbone: leer · 5 car: 0x40dac0 · 6 filter: 0x40daf0. End-Callbacks: überall leer.

Vertexdeklarationen (Tabelle 0x6f3270, dekodiert; Typen FLOAT3/D3DCOLOR/FLOAT2, Größe aus 0x6f3670):
- Effekte 0–3 (world…headlightreflect): POSITION FLOAT3, COLOR0 D3DCOLOR, TEXCOORD0 FLOAT2 = **24 B** (entspricht WP-14).
- 4 worldbone: POSITION, NORMAL (FLOAT3), COLOR0, TEXCOORD0, BLENDWEIGHT FLOAT3, BLENDINDICES FLOAT3 = 60 B.
- 5 car: POSITION, NORMAL, COLOR0, TEXCOORD0 = **36 B** (entspricht WP-14).
- 6 filter: POSITION FLOAT3, COLOR0, TEXCOORD0–3 FLOAT2 = 48 B.

Direkte Gerätezustände außerhalb der Effekte (0x408f60 `frame_suspend` u. a.): `SetRenderState` 7 (ZENABLE), 0xE (ZWRITEENABLE), `SetSamplerState` (1/2 = ADDRESSU/V, Wert 3 CLAMP bzw. 1 WRAP). Vollständige Kartierung steht aus.

## 4. Effekte
Alle sind `vs_1_1` + `ps_1_1`. `float4`-Vertexdaten, Matrizen D3D-Zeilenkonvention (`mul(v, M)`). Gemeinsame Zustände (alle Shadertechniken außer filter-Folgepässen): Alpha-Test/Blend aus `Base*` (shared), Cull je nach Datei.

| Datei | Technik(en) | Zweck | Parameter | Zustand |
|---|---|---|---|---|
| `world` | `world` (VS/PS 1.1), `world_fixed` | Strecke/Welt. Farbe = tex · vertexcolor · 2 | WorldViewProj, TextureOffset (VS) bzw. TextureOffsetMatrix (Fixed), diffusemap | CullMode NONE, Sampler WRAP/LINEAR. Fixed: FVF XYZ\|DIFFUSE\|TEX1, `Lighting=FALSE`, Stage 0 `Modulate2x` (Farbe und Alpha) |
| `worldbone` | `world` (Rest auskommentiert) | Strecke mit Skelett (Blend ≤3 Matrizen) und Harmonic-Licht | WorldViewProj, BlendMatrices[16], LocalEyePos, HarmonicCoeff[10], DiffuseMin/Range, diffusemap | CullMode NONE; Vertex: Position, Normale, Farbe, Gewichte, Indizes |
| `worldheadlight` | `world`, `world_fixed_single_pass`, `world_3_pass`, `world_simple` | Strecke + Scheinwerferprojektion (Textur + Clip-1D-Textur) | WorldViewProj, HeadlightWVP, HeadlightClipMat, HeadlightTex, HeadlightClipTex, HeadlightColor, TextureOffset(Matrix) | TextureTransformFlags[1,2]=Projected; 3-Pass: Pass 2 `ZFunc=Equal`, SrcBlend 14 (BLENDFACTOR) / DestBlend ONE, BlendFactor 0x606060 |
| `worldreflect` | `world`, `world_fixed_single_pass`, `world_fixed_2pass`, `world_fixed` | Nasse Straße, Spiegelung aus Render-Textur | WorldViewProj, ReflectedTex, ReflectedProj | Projektive Koordinate `(x,−y)·0,5 + w·0,5`; Ausgabe `2·dc·color + color.w·dc.w·refl`, Alpha = 1; 2-Pass: Pass 2 SrcBlend DESTALPHA, DestBlend ONE |
| `worldheadlightreflect` | `world`, `world_fixed`, `world_3_pass`, `world_fixed_simple` | Kombination aus beiden | wie headlight + ReflectedTex/Proj/TexMin | Vertexfarbe als DIFFUSE |
| `car` | `car` (VS/PS 1.1), `car_fixed_envmap`, `car_fixed` | Fahrzeug: Harmonic-Licht, Spiegelglanz, Cube-Umgebungskarte | WorldViewProj, WorldView, LocalEyePos, HarmonicCoeff, AmbientCoeff, Diffuse/Specular/Envmap Min/Range/Power, EnvMapTex (Cube), diffusemap | Fixed: `Lighting=TRUE`, `SpecularEnable`, `TexCoordIndex[1]=CAMERASPACEREFLECTIONVECTOR`, Stage 1 `MultiplyAdd` |
| `filter` | `filter` (4 Pässe, ps1…ps4) | Vollbild-Filter (Blur/Bloom): gewichtete Summe von 1–4 Texturen | FilterWeights[4], FilterTexture0–3 (Offsets stehen in den Vertexdaten) | CullMode NONE, ZENABLE/ZWRITE FALSE; nur Pass 0 setzt `Base*`-Blend; Pässe 1–3 setzen keinen Blend (vermutlich vom Spiel vorab gesetzt, **unbestätigt**) |
| `blank.tag` | – | 10 Byte „blank file“, unverschlüsselt, Platzhalter | – | – |

Wichtige Detailformeln (Wortlaut siehe `refs/fx/`): car-VS: `vdotn = max(0.01, dot(normalize(eye−pos), n))`; Diffus = (Σ Harmonic) · (DiffuseMin + vdotn·DiffuseRange) · 0,5; Reflexionsvektor `2·vdotn·n − v`; Spekular über dieselben Harmonic-Koeffizienten, `pow(spec.w, SpecularPower)`; PS: `2·d + 2·d.w·(spec + 2·env)`.

## 5. Lücken
- Nicht im Lauf (Wine/D3D-Aufzeichnung) verifiziert, alles statisch.
- Die Mehrpass-Variationen und Fixed-Function-Techniken sind dokumentiert, im Port aber nicht vorgesehen (ADR-003).
