

float4x4 WorldViewProj : WorldViewProjection;

struct VS_INPUT
{
	float3 Position : POSITION0;
	float4 Color : COLOR0;
	
};

struct VS_OUTPUT
{
	float4 NewPosition : POSITION0;
	float4 InterpolatedColor : COLOR0;
	
};

VS_OUTPUT mainVS(VS_INPUT vsIn)
{
	VS_OUTPUT vsOut;
	vsOut.NewPosition = mul (float4(vsIn.Position, 1.0f), WorldViewProj);
	vsOut.InterpolatedColor = mul(vsIn.Color, (1.0f, 0.2f, 0.3f, 1.0f));
	return vsOut;
}

float4 mainPS(VS_OUTPUT vsIn) : COLOR
{
	return vsIn.InterpolatedColor;
}

technique technique0 
{
	pass p0 
	{
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
