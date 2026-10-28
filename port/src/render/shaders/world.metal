// Neuschreibung von fx/world.efx, Technik "world" (vs_1_1/ps_1_1), 1:1 zur Vorlage. Nicht gegen das Original getestet.
// Matrizen kommen zeilenvektor-orientiert wie in D3D, mul(v, M) wird zu v * M. Der Host legt sie spaltenweise ab
// (wie D3DX SetMatrix bei column_major), d. h. eine D3DXMATRIX wird transponiert hochgeladen (WP-18 geprüft).
#include <metal_stdlib>
using namespace metal;

struct WorldUniforms {
    float4x4 WorldViewProj;   // WORLDVIEWPROJECTION
    float4   TextureOffset;   // TEXTUREOFFSET
};

struct VSIn {
    float3 position [[attribute(0)]];
    float4 color    [[attribute(1)]];   // D3DCOLOR, vom Vertexlayout nach 0..1 normalisiert
    float2 texcoord [[attribute(2)]];
};

struct VtoP {
    float4 position [[position]];
    float4 color;
    float4 tex;
};

// orig: fx/world.efx vertex_shader
vertex VtoP world_vs(VSIn in [[stage_in]], constant WorldUniforms& u [[buffer(0)]]) {
    VtoP out;
    out.position = float4(in.position, 1.0) * u.WorldViewProj;
    out.tex = float4(in.texcoord, 0.0, 0.0) + u.TextureOffset;
    out.color = in.color;
    return out;
}

// orig: fx/world.efx pixel_shader. Alpha-Test/-Blend laufen als Pipeline-Zustand (BASEALPHA*), nicht im Shader.
fragment float4 world_fs(VtoP in [[stage_in]], texture2d<float> diffusemap [[texture(0)]], sampler samp [[sampler(0)]]) {
    float4 d = diffusemap.sample(samp, in.tex.xy) * in.color * 2.0;
    return saturate(float4(d.xyz, d.w));
}

// Alphatest (BaseAlphaTestEnable/BaseAlphaRef) als Fragmentvariante, da SDL_GPU keinen Alphatest-Zustand kennt (ADR-003).
// Vergleich >= (D3DCMP_GREATEREQUAL angenommen, Vergleichsfunktion im Original nicht gelesen); AlphaRef.x in 0..1.
fragment float4 world_fs_atest(VtoP in [[stage_in]], constant float4& AlphaRef [[buffer(0)]],
                               texture2d<float> diffusemap [[texture(0)]], sampler samp [[sampler(0)]]) {
    float4 d = saturate(diffusemap.sample(samp, in.tex.xy) * in.color * 2.0);
    if (d.w < AlphaRef.x) discard_fragment();
    return d;
}
