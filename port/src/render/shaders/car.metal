// Neuschreibung von car.efx, Technik "car" (vs_1_1/ps_1_1). Nicht gegen das Original getestet.
#include "common.h"

struct CarIn {
    float3 position [[attribute(0)]];
    float3 normal   [[attribute(1)]];
    float4 color    [[attribute(2)]];
    float2 texcoord [[attribute(3)]];
};
struct CarUniforms {
    float4x4 WorldViewProj;
    float4x4 WorldView;
    float4   LocalEyePos;
    float4   HarmonicCoeff[10];
    float4   DiffuseMin, DiffuseRange;
    float4   SpecularMin, SpecularRange;
    float4   EnvmapMin, EnvmapRange;
    float    SpecularPower, EnvmapPower;
};
struct CarV {
    float4 position [[position]];
    float4 diffuse_color;
    float4 specular_color;
    float4 t0, t1;
};

static float4 harmonic(constant float4* h, float3 v) {
    float3 sq = v * v;
    float3 cr = float3(v.x * v.y, v.z * v.x, v.y * v.z);
    float4 c = h[0];
    c += h[1] * sq.x;
    c += h[2] * sq.y;
    c += h[3] * sq.z;
    c += h[4] * cr.x;
    c += h[5] * cr.y;
    c += h[6] * cr.z;
    c += h[7] * v.x;
    c += h[8] * v.y;
    c += h[9] * v.z;
    return c;
}

// orig: fx/car.efx vertex_shader
vertex CarV car_vs(CarIn in [[stage_in]], constant CarUniforms& u [[buffer(0)]]) {
    CarV o;
    float4 pos = float4(in.position, 1.0);
    o.position = pos * u.WorldViewProj;
    o.t1 = float4(in.texcoord, 0.0, 0.0);

    float3 view = u.LocalEyePos.xyz - in.position;
    view *= rsqrt(dot(view, view));

    float vdotn = max(0.01, dot(view, in.normal));
    float specvdotn = pow(vdotn, u.SpecularPower);
    float envvdotn = pow(vdotn, u.EnvmapPower);

    float4 diffuse_scale = u.DiffuseMin + vdotn * u.DiffuseRange;
    float4 specular_scale = u.SpecularMin + specvdotn * u.SpecularRange;

    // Diffuse harmonic lighting
    float4 d = harmonic(u.HarmonicCoeff, in.normal);
    d *= diffuse_scale;
    d.w = diffuse_scale.w;
    d *= 0.5;
    o.diffuse_color = d;

    float3 refl = 2.0 * vdotn * in.normal - view;
    o.t0 = float4((float4(refl, 0.0) * u.WorldView).xyz, 1.0);

    // Specular harmonic lighting
    float4 spec = harmonic(u.HarmonicCoeff, refl);
    spec.w = clamp(spec.w, 0.0, 1.0);
    float spec_lum = pow(spec.w, u.SpecularPower);
    o.specular_color = float4((spec * spec_lum * specular_scale * 0.5).xyz, envvdotn);
    return o;
}

// orig: fx/car.efx pixel_shader
fragment float4 car_fs(CarV in [[stage_in]], constant CarUniforms& u [[buffer(0)]],
                       texture2d<float> diffusemap [[texture(0)]], texturecube<float> envmap [[texture(1)]],
                       sampler samp [[sampler(0)]], sampler env_samp [[sampler(1)]]) {
    float4 diffuse_sample = diffusemap.sample(samp, in.t1.xy);
    float4 env_sample = envmap.sample(env_samp, in.t0.xyz);
    float4 diffuse_color = diffuse_sample * in.diffuse_color;
    float3 specular_color = in.specular_color.xyz;
    float3 env_color = env_sample.xyz * (2.0 * (u.EnvmapMin + in.specular_color.w * u.EnvmapRange)).xyz;
    float4 output = 2.0 * diffuse_color;
    output.xyz += 2.0 * diffuse_color.w * (specular_color + 2.0 * env_color);
    return saturate(output);
}
