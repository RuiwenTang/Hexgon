#include <metal_stdlib>
#include <simd/simd.h>

using namespace metal;

typedef struct
{
    float2 position [[attribute(0)]];
}
VertexIn;

typedef struct
{
    float4 position [[position]];
}
VertexOut;

vertex VertexOut vMain(VertexIn inp [[stage_in]])
{
    VertexOut outp;

    outp.position = float4(inp.position, 0.0, 1.0);

    return outp;
}

fragment float4 fMain(VertexOut inp [[stage_in]])
{
    return float4(1.0, 0.0, 0.0, 1.0);
}