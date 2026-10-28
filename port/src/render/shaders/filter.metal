// Neuschreibung von filter.efx (Technik "filter", Pässe p0..p3 = 1..4 Texturen). Nicht gegen das Original getestet.
// Die Passwahl trifft der Host (eine Pipeline je Variante).
#include "common.h"

struct FilterIn {
    float3 position [[attribute(0)]];
    float4 color    [[attribute(1)]];
    float2 tex0 [[attribute(2)]];
    float2 tex1 [[attribute(3)]];
    float2 tex2 [[attribute(4)]];
    float2 tex3 [[attribute(5)]];
};
struct FilterUniforms {
    float4 FilterWeights[4];
};
struct FilterV {
    float4 position [[position]];
    float2 tex0, tex1, tex2, tex3;
};
// orig: fx/filter.efx vertex_shader (Position ist bereits im Clipraum)
vertex FilterV filter_vs(FilterIn in [[stage_in]]) {
    FilterV o;
    o.position = float4(in.position, 1.0);
    o.tex0 = in.tex0; o.tex1 = in.tex1; o.tex2 = in.tex2; o.tex3 = in.tex3;
    return o;
}
// orig: fx/filter.efx pixel_shader1..4
fragment float4 filter_fs1(FilterV in [[stage_in]], constant FilterUniforms& u [[buffer(0)]],
                           texture2d<float> t0 [[texture(0)]], sampler s [[sampler(0)]]) {
    return saturate(t0.sample(s, in.tex0) * u.FilterWeights[0]);
}
fragment float4 filter_fs2(FilterV in [[stage_in]], constant FilterUniforms& u [[buffer(0)]],
                           texture2d<float> t0 [[texture(0)]], texture2d<float> t1 [[texture(1)]], sampler s [[sampler(0)]]) {
    return saturate(t0.sample(s, in.tex0) * u.FilterWeights[0] + t1.sample(s, in.tex1) * u.FilterWeights[1]);
}
fragment float4 filter_fs3(FilterV in [[stage_in]], constant FilterUniforms& u [[buffer(0)]],
                           texture2d<float> t0 [[texture(0)]], texture2d<float> t1 [[texture(1)]],
                           texture2d<float> t2 [[texture(2)]], sampler s [[sampler(0)]]) {
    return saturate(t0.sample(s, in.tex0) * u.FilterWeights[0] + t1.sample(s, in.tex1) * u.FilterWeights[1]
         + t2.sample(s, in.tex2) * u.FilterWeights[2]);
}
fragment float4 filter_fs4(FilterV in [[stage_in]], constant FilterUniforms& u [[buffer(0)]],
                           texture2d<float> t0 [[texture(0)]], texture2d<float> t1 [[texture(1)]],
                           texture2d<float> t2 [[texture(2)]], texture2d<float> t3 [[texture(3)]], sampler s [[sampler(0)]]) {
    return saturate(t0.sample(s, in.tex0) * u.FilterWeights[0] + t1.sample(s, in.tex1) * u.FilterWeights[1]
         + t2.sample(s, in.tex2) * u.FilterWeights[2] + t3.sample(s, in.tex3) * u.FilterWeights[3]);
}
