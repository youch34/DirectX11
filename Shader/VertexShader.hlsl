cbuffer Cbuf : register(b0)
{
    row_major matrix world;
    row_major matrix view_proj;
};

struct VSout
{
    float4 pos : SV_Position;
    float2 uv : UV;
    float3 normal : Normal;
    float3 wolrdPos : WorldPos;
};

VSout VS(float4 pos : Position, float2 uv : UV, float3 normal : Normal)
{
    VSout output;
    output.pos = mul(pos, world);
    output.pos = mul(output.pos, view_proj);
    output.uv = uv;
    output.normal = normalize(mul(float4(normal, 0.0f), world));
    output.wolrdPos = mul(pos, world);
    return output;
}