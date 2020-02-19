float diffuse_intensity;
float4 diffuse_color;

float4 main(float3 light:  TEXCOORD0, float3 normal : TEXCOORD1) :COLOR0
{
    return diffuse_color * diffuse_intensity * dot(normal, light);
}