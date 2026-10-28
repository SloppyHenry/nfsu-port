// Gemeinsame Vereinbarungen der MSL-Neuschreibungen von fx/*.efx (ADR-003).
// Matrizen: D3D-Zeilenmatrizen, mul(v, M) wird zu v * M; der Host lädt sie spaltenweise (D3DXMATRIX transponiert).
// Farben: D3DCOLOR-Vertexattribute werden vom Vertexlayout nach 0..1 (RGBA) normalisiert.
// Projektive Texturen (TextureTransformFlags = Projected): Division durch w im Fragmentshader.
// 1D-Texturen (HeadlightClipTex) liegen als 2D-Textur der Höhe 1 vor, Abtastung bei y = 0.5.
#pragma once
#include <metal_stdlib>
using namespace metal;
