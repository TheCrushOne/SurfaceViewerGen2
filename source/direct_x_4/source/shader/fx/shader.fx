//--------------------------------------------------------------------------------------
// File: BasicHLSL.fx
//
// The effect file for the BasicHLSL sample.  
// 
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

#define MAX_NUM_LIGHTS 2

struct PointLight
{
    float3 pos;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float radius;
};

struct Material
{
    float4 ambient;
    float4 diffuse;
    float4 emissive;
    float4 specular;
    float shininess;
};
//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
float4 g_MaterialAmbientColor;      // Material's ambient color
float4 g_MaterialDiffuseColor;      // Material's diffuse color
int g_nNumLights;

float3 g_LightDir[3];               // Light's direction in world space
float4 g_LightDiffuse[3];           // Light's diffuse color
float4 g_LightAmbient;              // Light's ambient color



PointLight lights[MAX_NUM_LIGHTS];
Material material;
int g_pLightNum;

texture g_MeshTexture;              // Color texture for mesh

float    g_fTime;                   // App's time in seconds
float4x4 g_mWorld;                  // World matrix for object
float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
float4x4 worldInverseTransposeMatrix;
float3 cameraPos;

//--------------------------------------------------------------------------------------
// Texture samplers
//--------------------------------------------------------------------------------------
sampler MeshTextureSampler =
sampler_state
{
    Texture = <g_MeshTexture>;
    MipFilter = LINEAR;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
};


//--------------------------------------------------------------------------------------
// Vertex shader output structure
//--------------------------------------------------------------------------------------
struct VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color (note that COLOR0 is clamped from 0..1)
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
    float3 worldPos : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
    float3 normal : TEXCOORD3;
};



//--------------------------------------------------------------------------------------
// This shader computes standard transform and lighting
//--------------------------------------------------------------------------------------
VS_OUTPUT RenderSceneVS(float4 vPos : POSITION,
    float3 vNormal : NORMAL,
    float2 vTexCoord0 : TEXCOORD0,
    uniform int nNumLights,
    uniform bool bTexture,
    uniform bool bAnimate)
{
    VS_OUTPUT Output;
    float3 vNormalWorldSpace;

    float4 vAnimatedPos = vPos;
    Output.worldPos = mul(vPos, g_mWorld).xyz;
    Output.viewDir = cameraPos - Output.worldPos;
    Output.normal = mul(vNormal, (float3x3)worldInverseTransposeMatrix);
        // Animation the vertex based on time and the vertex's object space position
    //if (bAnimate)
        //vAnimatedPos += float4(vNormal, 0) * (sin(g_fTime + 5.5) + 0.5) * 5;

    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(vAnimatedPos, g_mWorldViewProjection);

    // Transform the normal from object space to world space    
    vNormalWorldSpace = normalize(mul(vNormal, (float3x3)g_mWorld)); // normal (world space)

    // Compute simple directional lighting equation
    float3 vTotalLightDiffuse = float3(0, 0, 0);
    for (int i = 0; i < nNumLights; i++)
        vTotalLightDiffuse += g_LightDiffuse[i] * max(0, dot(vNormalWorldSpace, g_LightDir[i]));

    Output.Diffuse.rgb = g_MaterialDiffuseColor * vTotalLightDiffuse +
        g_MaterialAmbientColor * g_LightAmbient;
    Output.Diffuse.a = 1.0f;

    // Just copy the texture coordinate through
    if (bTexture)
        Output.TextureUV = vTexCoord0;
    else
        Output.TextureUV = 0;

    return Output;
}


//--------------------------------------------------------------------------------------
// Pixel shader output structure
//--------------------------------------------------------------------------------------
struct PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};



float4 calculateSingleColor(int i, float3 n, float3 v, float3 wp)
{
    float4 color = (0.0f, 0.0f, 0.0f, 0.0f);

    
    float3 l = float3(0.0f, 0.0f, 0.0f);
    float3 h = float3(0.0f, 0.0f, 0.0f);

    float atten = 0.0f;
    float nDotL = 0.0f;
    float nDotH = 0.0f;
    float power = 0.0f;

    l = (lights[i].pos - wp) / lights[i].radius;
    atten = saturate(1.0f - dot(l, l));

    l = normalize(l);
    h = normalize(l + v);

    nDotL = saturate(dot(n, l));
    nDotH = saturate(dot(n, h));

    power = ((nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess));

    color += (material.ambient * ((atten * lights[i].ambient))) +
    (material.diffuse * lights[i].diffuse * nDotL * atten) +
    (material.specular * lights[i].specular * power * atten);

    return color;

}


//--------------------------------------------------------------------------------------
// This shader outputs the pixel's color by modulating the texture's
//       color with diffuse material color
//--------------------------------------------------------------------------------------
PS_OUTPUT RenderScenePS(VS_OUTPUT In,
    uniform bool bTexture)
{
    PS_OUTPUT Output;
    float3 n = normalize(In.normal);
    float3 v = normalize(In.viewDir);
    //float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 color = In.Diffuse;
           

    //g_pLightNum = 3;
    
    for (int i = 0; i < MAX_NUM_LIGHTS; i++)
    {
        color += calculateSingleColor(i, n, v, In.worldPos);
    }

    // Lookup mesh texture and modulate it with diffuse
    if (bTexture)
        Output.RGBColor = color*tex2D(MeshTextureSampler, In.TextureUV);
    else
        Output.RGBColor = color;// In.Diffuse;

    return Output;
}


//--------------------------------------------------------------------------------------
// Renders scene to render target
//--------------------------------------------------------------------------------------
technique RenderSceneWithTexture1Light
{
    pass P0
    {
        VertexShader = compile vs_2_0 RenderSceneVS(1, true, true);
        PixelShader = compile ps_2_0 RenderScenePS(true); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture2Light
{
    pass P0
    {
        VertexShader = compile vs_2_0 RenderSceneVS(2, true, true);
        PixelShader = compile ps_2_0 RenderScenePS(true); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneWithTexture3Light
{
    pass P0
    {
        VertexShader = compile vs_2_0 RenderSceneVS(3, true, true);
        PixelShader = compile ps_2_0 RenderScenePS(true); // trivial pixel shader (could use FF instead if desired)
    }
}

technique RenderSceneNoTexture
{
    pass P0
    {
        VertexShader = compile vs_2_0 RenderSceneVS(1, false, false);
        PixelShader = compile ps_2_0 RenderScenePS(false); // trivial pixel shader (could use FF instead if desired)
    }
}
