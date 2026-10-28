// Neuschreibung von worldheadlight.efx, worldreflect.efx, worldheadlightreflect.efx, worldbone.efx
// (jeweils Technik "world", vs_1_1/ps_1_1). Nicht gegen das Original getestet.
#include "common.h"

struct VSIn {
    float3 position [[attribute(0)]];
    float4 color    [[attribute(1)]];
    float2 texcoord [[attribute(2)]];
};

// ---------- worldheadlight ----------
struct HeadlightUniforms {
    float4x4 WorldViewProj;
    float4x4 HeadlightWVP;
    float4   TextureOffset;
    float4   HeadlightColor;
};
struct HeadlightV {
    float4 position [[position]];
    float4 color;
    float4 t0, t1, t2;
};
// orig: fx/worldheadlight.efx vertex_shader
vertex HeadlightV worldheadlight_vs(VSIn in [[stage_in]], constant HeadlightUniforms& u [[buffer(0)]]) {
    HeadlightV o;
    float4 pos = float4(in.position, 1.0);
    o.position = pos * u.WorldViewProj;
    float4 tp = pos * u.HeadlightWVP;
    o.t0 = float4(in.texcoord, 0.0, 0.0) + u.TextureOffset;
    o.t1 = tp;
    o.t2 = float4((tp.w * 0.005) + 0.5, 0.0, 1.0, 1.0);
    o.color = in.color;
    return o;
}
// orig: fx/worldheadlight.efx pixel_shader
fragment float4 worldheadlight_fs(HeadlightV in [[stage_in]], constant HeadlightUniforms& u [[buffer(0)]],
                                  texture2d<float> diffusemap [[texture(0)]], texture2d<float> headlight [[texture(1)]],
                                  texture2d<float> headlightclip [[texture(2)]],
                                  sampler samp [[sampler(0)]], sampler hl_samp [[sampler(1)]], sampler clip_samp [[sampler(2)]]) {
    float4 tex = diffusemap.sample(samp, in.t0.xy);
    float4 hl = headlight.sample(hl_samp, in.t1.xy / in.t1.w);
    float4 clip = headlightclip.sample(clip_samp, float2(in.t2.x, 0.5));
    float4 o = (hl * clip * u.HeadlightColor + in.color) * tex * 2.0;
    return saturate(float4(o.xyz, tex.w * in.color.w * 2.0));
}

// ---------- worldreflect ----------
struct ReflectUniforms {
    float4x4 WorldViewProj;
};
struct ReflectV {
    float4 position [[position]];
    float4 color;
    float4 t0, t1;
};
// orig: fx/worldreflect.efx vertex_shader
vertex ReflectV worldreflect_vs(VSIn in [[stage_in]], constant ReflectUniforms& u [[buffer(0)]]) {
    ReflectV o;
    float4 p = float4(in.position, 1.0) * u.WorldViewProj;
    o.position = p;
    o.t0 = float4(in.texcoord, 0.0, 0.0);
    o.color = in.color;
    p.y = -p.y;
    p.xy += p.w;      // "one" - texture bias
    p.xy *= 0.5;
    o.t1 = p;
    return o;
}
// orig: fx/worldreflect.efx pixel_shader
fragment float4 worldreflect_fs(ReflectV in [[stage_in]], texture2d<float> diffusemap [[texture(0)]],
                                texture2d<float> reflected [[texture(1)]], sampler samp [[sampler(0)]], sampler rsamp [[sampler(1)]]) {
    float4 dc = diffusemap.sample(samp, in.t0.xy);
    float4 rs = reflected.sample(rsamp, in.t1.xy / in.t1.w);
    float3 diffuse = (dc * in.color).xyz;
    float3 refl = in.color.w * dc.w * rs.xyz;
    return saturate(float4(diffuse * 2.0 + refl, 1.0));
}

// ---------- worldheadlightreflect ----------
struct HLRUniforms {
    float4x4 WorldViewProj;
    float4x4 HeadlightWVP;
    float4   HeadlightColor;
};
struct HLRV {
    float4 position [[position]];
    float4 color;
    float4 t0, t1, t2, t3;
};
// orig: fx/worldheadlightreflect.efx vertex_shader
vertex HLRV worldheadlightreflect_vs(VSIn in [[stage_in]], constant HLRUniforms& u [[buffer(0)]]) {
    HLRV o;
    float4 pos = float4(in.position, 1.0);
    float4 p = pos * u.WorldViewProj;
    o.position = p;
    float4 tp = pos * u.HeadlightWVP;
    o.t0 = float4(in.texcoord, 0.0, 0.0);
    o.t1 = tp;
    o.t2 = float4((tp.w * 0.005) + 0.5, 0.0, 1.0, 1.0);
    p.y = -p.y;
    p.xy += p.w;
    p.xy *= 0.5;
    o.t3 = p;
    o.color = in.color;
    return o;
}
// orig: fx/worldheadlightreflect.efx pixel_shader
fragment float4 worldheadlightreflect_fs(HLRV in [[stage_in]], constant HLRUniforms& u [[buffer(0)]],
                                         texture2d<float> diffusemap [[texture(0)]], texture2d<float> headlight [[texture(1)]],
                                         texture2d<float> headlightclip [[texture(2)]], texture2d<float> reflected [[texture(3)]],
                                         sampler samp [[sampler(0)]], sampler hl_samp [[sampler(1)]],
                                         sampler clip_samp [[sampler(2)]], sampler rsamp [[sampler(3)]]) {
    float4 tex = diffusemap.sample(samp, in.t0.xy);
    float4 hl = headlight.sample(hl_samp, in.t1.xy / in.t1.w);
    float4 clip = headlightclip.sample(clip_samp, float2(in.t2.x, 0.5));
    float4 rs = reflected.sample(rsamp, in.t3.xy / in.t3.w);
    float3 diffuse = ((hl * clip * u.HeadlightColor + in.color) * tex).xyz;
    float3 refl = tex.w * in.color.w * rs.xyz;
    return saturate(float4(diffuse * 2.0 + refl, 1.0));
}

// ---------- worldbone ----------
struct BoneIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float4 color    [[attribute(2)]];
    float2 texcoord [[attribute(3)]];
    float3 weight   [[attribute(4)]];
    float3 index    [[attribute(5)]];
};
struct BoneUniforms {
    float4x4 WorldViewProj;
    float4   LocalEyePos;
    float4   HarmonicCoeff[10];
    float4x4 BlendMatrices[16];
    float4   DiffuseMin;
    float4   DiffuseRange;
};
struct BoneV {
    float4 position [[position]];
    float4 color;
    float4 t0;
};
// orig: fx/worldbone.efx vertex_shader
vertex BoneV worldbone_vs(BoneIn in [[stage_in]], constant BoneUniforms& u [[buffer(0)]]) {
    BoneV o;
    float4 pos = float4(in.position, 1.0);
    int ix = int(in.index.x), iy = int(in.index.y), iz = int(in.index.z);
    float4 p = pos * u.BlendMatrices[ix] * in.weight.x;
    p += pos * u.BlendMatrices[iy] * in.weight.y;
    p += pos * u.BlendMatrices[iz] * in.weight.z;

    // transponierte Multiplikation für die Normale, nur mit der ersten Matrix
    float3 n = in.normal.xxx * u.BlendMatrices[ix][0].xyz;
    n += in.normal.yyy * u.BlendMatrices[ix][1].xyz;
    n += in.normal.zzz * u.BlendMatrices[ix][2].xyz;

    float3 view = u.LocalEyePos.xyz - p.xyz;
    view *= rsqrt(dot(view, view));
    float vdotn = dot(view, n);
    float4 diffuse_scale = u.DiffuseMin + vdotn * u.DiffuseRange;

    float3 nsq = n * n;
    float3 ncr = float3(n.x * n.y, n.z * n.x, n.y * n.z);
    float4 c = u.HarmonicCoeff[0];
    c += u.HarmonicCoeff[1] * nsq.x;
    c += u.HarmonicCoeff[2] * nsq.y;
    c += u.HarmonicCoeff[3] * nsq.z;
    c += u.HarmonicCoeff[4] * ncr.x;
    c += u.HarmonicCoeff[5] * ncr.y;
    c += u.HarmonicCoeff[6] * ncr.z;
    c += u.HarmonicCoeff[7] * n.x;
    c += u.HarmonicCoeff[8] * n.y;
    c += u.HarmonicCoeff[9] * n.z;
    c *= diffuse_scale;
    c.w = diffuse_scale.w;
    o.color = c;

    o.position = p * u.WorldViewProj;
    o.t0 = float4(in.texcoord, 0.0, 0.0);
    return o;
}
// orig: fx/worldbone.efx pixel_shader
fragment float4 worldbone_fs(BoneV in [[stage_in]], texture2d<float> diffusemap [[texture(0)]], sampler samp [[sampler(0)]]) {
    float4 t = diffusemap.sample(samp, in.t0.xy);
    return saturate(float4(in.color.xyz * t.xyz * 2.0, t.w * in.color.w * 2.0));
}
