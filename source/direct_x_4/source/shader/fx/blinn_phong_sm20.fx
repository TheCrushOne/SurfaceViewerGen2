//-----------------------------------------------------------------------------
// Copyright (c) 2008 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------
//
// Multiple per-pixel point lights using shader model 2.0. Single pass lighting
// limits the number of point lights to 2. More than 2 point lights will cause
// the shader to exceeded the max 64 arithmetic instructions limit imposed by
// shader model 2.0. Multi pass lighting doesn't have this restriction though.
//
//-----------------------------------------------------------------------------

#define MAX_POINT_LIGHTS 2

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

//-----------------------------------------------------------------------------
// Globals.
//-----------------------------------------------------------------------------

float4x4 worldMatrix;
float4x4 worldInverseTransposeMatrix;
float4x4 worldViewProjectionMatrix;

float3 cameraPos;
float4 globalAmbient;

PointLight lights[MAX_POINT_LIGHTS];
Material material;

//-----------------------------------------------------------------------------
// Textures.
//-----------------------------------------------------------------------------

texture colorMapTexture;

sampler2D colorMap = sampler_state
{
	Texture = <colorMapTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
    MipFilter = Linear;
    MaxAnisotropy = 16;
};

//-----------------------------------------------------------------------------
// Vertex Shaders.
//-----------------------------------------------------------------------------

struct VS_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 position : POSITION;
	float3 worldPos : TEXCOORD0;
	float2 texCoord : TEXCOORD1;
	float3 viewDir : TEXCOORD2;
	float3 normal : TEXCOORD3;
};

VS_OUTPUT VS_PointLighting(VS_INPUT IN)
{
	VS_OUTPUT OUT;

	OUT.position = mul(float4(IN.position, 1.0f), worldViewProjectionMatrix);
	OUT.worldPos = mul(float4(IN.position, 1.0f), worldMatrix).xyz;
	OUT.texCoord = IN.texCoord;
	OUT.viewDir = cameraPos - OUT.worldPos;
	OUT.normal = mul(IN.normal, (float3x3)worldInverseTransposeMatrix);
	
	return OUT;
}

//-----------------------------------------------------------------------------
// Pixel Shaders.
//-----------------------------------------------------------------------------

float4 PS_SinglePassPointLighting(VS_OUTPUT IN) : COLOR
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float3 n = normalize(IN.normal);
    float3 v = normalize(IN.viewDir);
    float3 l = float3(0.0f, 0.0f, 0.0f);
    float3 h = float3(0.0f, 0.0f, 0.0f);
    
    float atten = 0.0f;
    float nDotL = 0.0f;
    float nDotH = 0.0f;
    float power = 0.0f;
    
    for (int i = 0; i < MAX_POINT_LIGHTS; ++i)
    {
        l = (lights[i].pos - IN.worldPos) / lights[i].radius;
        atten = saturate(1.0f - dot(l, l));
        
        l = normalize(l);
        h = normalize(l + v);
        
        nDotL = saturate(dot(n, l));
        nDotH = saturate(dot(n, h));
        power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess);
        
        color += (material.ambient * (globalAmbient + (atten * lights[i].ambient))) +
                 (material.diffuse * lights[i].diffuse * nDotL * atten) +
                 (material.specular * lights[i].specular * power * atten);
    }
                   
	return color * tex2D(colorMap, IN.texCoord);
}

float4 PS_MultiPassPointLighting(VS_OUTPUT IN, uniform int i) : COLOR
{      
    float3 l = (lights[i].pos - IN.worldPos) / lights[i].radius;
    float atten = saturate(1.0f - dot(l, l));
    
    l = normalize(l);
    
    float3 n = normalize(IN.normal);
    float3 v = normalize(IN.viewDir);
    float3 h = normalize(l + v);
    
    float nDotL = saturate(dot(n, l));
    float nDotH = saturate(dot(n, h));
    float power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess);
       
    float4 color = (material.ambient * (globalAmbient + (atten * lights[i].ambient))) +
                   (material.diffuse * lights[i].diffuse * nDotL * atten) +
                   (material.specular * lights[i].specular * power * atten);
                   
	return color * tex2D(colorMap, IN.texCoord);
}

//-----------------------------------------------------------------------------
// Techniques.
//-----------------------------------------------------------------------------

technique PerPixelPointLightingSinglePass
{
    pass
    {
        VertexShader = compile vs_2_0 VS_PointLighting();
        PixelShader = compile ps_2_0 PS_SinglePassPointLighting();
    }
}

technique PerPixelPointLightingMultiPass
{
    pass Light1
    {
       
        VertexShader = compile vs_2_0 VS_PointLighting();
        PixelShader = compile ps_2_0 PS_MultiPassPointLighting(0);
    }   
   
    pass Light2
    {    
		AlphaBlendEnable = TRUE;
		SrcBlend = ONE;
		DestBlend = ONE;
        VertexShader = compile vs_2_0 VS_PointLighting();
        PixelShader = compile ps_2_0 PS_MultiPassPointLighting(1);
    }
    
    
}