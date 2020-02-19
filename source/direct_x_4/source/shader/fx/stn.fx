#define NUM_NEON_KPAIRS 4
#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 4
#define NUM_CAR_VISION_LIGHTS 14
#define NUM_SPOT_LIGHTS 14

struct stKinematicPair
{
    float3 pos1;
    float3 pos2;
    float3 pos3;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float radius;
    //bool enabled;
};

//float3 dLightDir = normalize(float3(1.0f, 1.0f, -1.0f));	// Light's direction
//float4 dLightAmbient = { 0.2f, 0.2f, 0.2f, 0.2f };			// Light's ambient color
//float4 dLightDiffuse = { 1.0f, 1.0f, 1.0f, 1.0f };			// Light's diffuse color
struct directionalLight
{
    float3 direction;
    float4 ambient;
    float4 diffuse;
	//bool enabled;
};


struct pointLight
{
    float3 pos;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float radius;
	//bool enabled;
};

struct carVisionLight
{
    float3 pos;
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float radius;
    //bool enabled;
};


struct spotLight
{
    float3 ambient;
    float3 pos;
    float3 diffuse;
    float3 direction;
    float angle;
    float falloff;
    float range;
    //float att0;
    float att1;
    //float att2;
	//bool enabled;
};




struct texMaterial
{
    float4 ambient;
    float4 diffuse;
    float4 emissive;
    float4 specular;
    float shininess;
};

// Некоторые важные переменные

float4x4 worldMatrix;
float4x4 worldInverseTransposeMatrix;
float4x4 worldViewProjectionMatrix;

float3 cameraPos;
//дебаговый
//float4 globalAmbient = { 1.0f, 1.0f, 1.0f, 1.0f };

stKinematicPair skpnLight[NUM_NEON_KPAIRS];
directionalLight dLight[NUM_DIRECTIONAL_LIGHTS];
pointLight pLight[NUM_POINT_LIGHTS];
carVisionLight cVLight[NUM_CAR_VISION_LIGHTS];
spotLight sLight[NUM_SPOT_LIGHTS];

texMaterial material;
bool textureEnabled;


//-----------------------------------------------------------------------------
// Textures.
//-----------------------------------------------------------------------------

texture colorMapTexture;

sampler2D colorMap = sampler_state
{
    Texture = <colorMapTexture>;
    MagFilter = Linear;
    MinFilter = Anisotropic;
    MipFilter = None;// Linear;
    MaxAnisotropy = 16;
};


struct GL_PS_OUTPUT
{
    float4 RGBColor : COLOR0;  // Pixel color    
};

// Шейдер освещения типа "кинематическая пара"
// Внимание! Экспериментальный прототип! Стабильность работы не гарантируется!
struct KP_VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};

struct KP_VS_OUTPUT
{
    float4 position : POSITION;
    float3 worldPos : TEXCOORD0;
    float2 texCoord : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
    float3 normal : TEXCOORD3;
};


KP_VS_OUTPUT KP_vShader(KP_VS_INPUT IN)
{
    KP_VS_OUTPUT OUT;

    OUT.position = mul(float4(IN.position, 1.0f), worldViewProjectionMatrix);
    OUT.worldPos = mul(float4(IN.position, 1.0f), worldMatrix).xyz;
    OUT.texCoord = IN.texCoord;
    OUT.viewDir = cameraPos - OUT.worldPos;
    OUT.normal = mul(IN.normal, (float3x3)worldInverseTransposeMatrix);

    return OUT;
}

GL_PS_OUTPUT KP_pShader(KP_VS_OUTPUT IN) : COLOR
{
    
    GL_PS_OUTPUT OUT;
	
    float4 color = (0.0f, 0.0f, 0.0f, 0.0f);
    
    //t32
    float3 tmp21, tmp23;
    float3 vct21;// = skpnLight[i].pos2 - skpnLight[i].pos1;
    float3 vct23;// = skpnLight[i].pos2 - skpnLight[i].pos3;
    float3 vctt1;// = skpnLight[i].pos1 - IN.worldPos;
    float3 vctt2;// = skpnLight[i].pos2 - IN.worldPos;
    float3 vctt3;// = skpnLight[i].pos3 - IN.worldPos;
    float d1, d2, lt, atten, nDotL, nDotH, power, st;
    float3 n, v;
	
    for (int cnt = 0; cnt < NUM_NEON_KPAIRS; cnt++)    
    {
        vct21 = skpnLight[cnt].pos2 - skpnLight[cnt].pos1;
        vct23 = skpnLight[cnt].pos2 - skpnLight[cnt].pos3;
        vctt1 = skpnLight[cnt].pos1 - IN.worldPos;
        vctt2 = skpnLight[cnt].pos2 - IN.worldPos;
        vctt3 = skpnLight[cnt].pos3 - IN.worldPos;

        tmp21 = vct21 / length(vct21);
        tmp23 = vct23 / length(vct23);
        if (dot(vct21, vctt2)*dot(-vct21, vctt1) >= 0.0f)
            d1 = length(cross(tmp21, vctt2));
        else
            d1 = ((dot(vct21, vctt2) >= 0.0f) ? length((skpnLight[cnt].pos1 - IN.worldPos)) : length((skpnLight[cnt].pos2 - IN.worldPos)));
        if (dot(vct23, vctt2)*dot(-vct23, vctt3) >= 0.0f)
            d2 = length(cross(tmp23, vctt2));
        else
            d2 = ((dot(vct23, vctt2) >= 0.0f) ? length((skpnLight[cnt].pos3 - IN.worldPos)) : length((skpnLight[cnt].pos2 - IN.worldPos) /*/ skpnLight[i].radius*/));
        lt = min(d1, d2);

        //float3 l = (lights[i].pos - IN.worldPos) / lights[i].radius;
        st = skpnLight[cnt].radius - lt;
        if (st > 0)
        {
            //костыль
            atten = 2 * saturate(st*(1 / skpnLight[cnt].radius)/*dot(l, l)*/);
            atten = pow(atten, 3);
            //l = normalize(l);

            n = normalize(IN.normal);
            v = normalize(IN.viewDir);
            //float3 h = normalize(l + v);

            //и еще костыль...
            nDotL = saturate(skpnLight[cnt].radius); //saturate(dot(n, l));
            nDotH = saturate(dot(n, v));
            power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess);

            color += (material.ambient * (/*globalAmbient +*/ (atten * skpnLight[cnt].ambient))) +
                (material.diffuse * skpnLight[cnt].diffuse * nDotL * atten) +
                (material.specular * skpnLight[cnt].specular * power * atten);
        }
    }

    OUT.RGBColor = tex2D(colorMap, IN.texCoord)*color;
    
    return OUT;
}


// Шейдер направленного освещения

struct D_VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD0;
};

struct D_VS_OUTPUT
{
    float4 Position   : POSITION;   // vertex position 
    float4 Diffuse    : COLOR0;     // vertex diffuse color
    float2 TextureUV  : TEXCOORD0;  // vertex texture coords 
};


D_VS_OUTPUT D_vShader(D_VS_INPUT IN)
{
    D_VS_OUTPUT Output;
    float3 vNormalWorldSpace;

    // Animation the vertex based on time and the vertex's object space position
    float4 vAnimatedPos = IN.position;
        

    // Transform the position from object space to homogeneous projection space
    Output.Position = mul(vAnimatedPos, worldViewProjectionMatrix);

    // Transform the normal from object space to world space	
    vNormalWorldSpace = normalize(mul(IN.normal, (float3x3)worldMatrix)); // normal (world space)

    // Compute simple directional lighting equation
    Output.Diffuse.rgb = material.diffuse * dLight[0].diffuse * max(0, dot(vNormalWorldSpace, dLight[0].direction)) +
        material.ambient * dLight[0].ambient;
    Output.Diffuse.a = 1.0f;

    // Just copy the texture coordinate through
    Output.TextureUV = IN.texCoord;

    return Output;
}


GL_PS_OUTPUT D_pShader(D_VS_OUTPUT IN)
{
    GL_PS_OUTPUT Output;

    // Lookup mesh texture and modulate it with diffuse
	
	Output.RGBColor = tex2D(colorMap, IN.TextureUV) * IN.Diffuse;
	

    return Output;
}

// Шейдер конусного освещения(пока что 1 угол вместо 2)

struct S_VS_INPUT
{
    float4 Position : POSITION0;
    float2 UV : TEXCOORD0;
    float3 Normal : NORMAL0;
};


struct S_VS_OUTPUT
{
    float4 Position : POSITION0;
    float2 UV : TEXCOORD0;
    float3 Normal : TEXCOORD1;
    float4 WorldPosition : TEXCOORD2;
};



S_VS_OUTPUT S_vShader(S_VS_INPUT input)
{
    S_VS_OUTPUT output;

    output.Position = mul(input.Position, worldViewProjectionMatrix);

    output.WorldPosition = mul(input.Position, worldMatrix);
    output.UV = input.UV;
    output.Normal = mul(input.Normal, worldMatrix);

    return output;
}


GL_PS_OUTPUT S_pShader(S_VS_OUTPUT input) : COLOR0
{
    GL_PS_OUTPUT OUT;
    float3 diffuseColor;
    float3 totalLight = float3(0, 0, 0);
    float3 lightDir, nr;
    float diffuse, d, a, att, st, atts;
    //if (textureEnabled)
    diffuseColor *= tex2D(colorMap, input.UV).rgb;

    float3 rtLight = float3(0, 0, 0);
           
            //totalLight += AmbientLightColor;
    for (int cnt = 0; cnt < NUM_SPOT_LIGHTS; cnt++)
    {
        diffuseColor = sLight[cnt].diffuse;
        diffuseColor *= tex2D(colorMap, input.UV).rgb;
        nr = sLight[cnt].pos - input.WorldPosition;
        atts = saturate(1 - length(nr)/sLight[cnt].range);
        lightDir = normalize(nr);
        diffuse = saturate(dot(normalize(input.Normal), lightDir));

        // (dot(p - lp, ld) / cos(a))^f
        d = dot(-lightDir, normalize(sLight[cnt].direction));
        a = cos(sLight[cnt].angle);

        st = d / a;
        if (st > 0)
        {
            att = pow((st), sLight[cnt].falloff);
            
            totalLight = diffuse * sLight[cnt].diffuse * att * sLight[cnt].att1 * atts;
            //totalLight = diffuse * sLight[cnt].diffuse*att;
            rtLight += diffuseColor*totalLight;
        }
    }

    OUT.RGBColor = float4(rtLight, 1.0f);

       
    return OUT;
}


// Шейдер точечного освещения

struct P_VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};

struct P_VS_OUTPUT
{
    float4 position : POSITION;
    float3 worldPos : TEXCOORD0;
    float2 texCoord : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
    float3 normal : TEXCOORD3;
    float3 stnormal : TEXCOORD4;
};

P_VS_OUTPUT P_vShader(P_VS_INPUT IN)
{
    P_VS_OUTPUT OUT;

    OUT.position = mul(float4(IN.position, 1.0f), worldViewProjectionMatrix);
    OUT.worldPos = mul(float4(IN.position, 1.0f), worldMatrix).xyz;
    OUT.texCoord = IN.texCoord;
    OUT.viewDir = cameraPos - OUT.worldPos;
    OUT.normal = mul(IN.normal, (float3x3)worldInverseTransposeMatrix);
    OUT.stnormal = IN.normal;

    return OUT;
}

GL_PS_OUTPUT P_pShader(P_VS_OUTPUT IN) : COLOR
{
    GL_PS_OUTPUT OUT;
    
    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);


    float3 n = normalize(IN.normal);
    float3 v = normalize(IN.viewDir);
    float3 l = float3(0.0f, 0.0f, 0.0f);
    float3 h = float3(0.0f, 0.0f, 0.0f);
    float3 rm = mul(IN.stnormal, worldMatrix);

    float3 ln;
        

    float atten = 0.0f;
    float nDotL = 0.0f;
    float nDotH = 0.0f;
    float power = 0.0f;
    
    for (int cnt = 0; cnt < NUM_POINT_LIGHTS; cnt++)
    {
        ln = pLight[cnt].pos - IN.worldPos;
        if (length(ln) < pLight[cnt].radius)
        {

            l = (ln) / pLight[cnt].radius;
            atten = saturate(1.0f - dot(l, l));

            l = normalize(l);
            h = normalize(l + v);

            nDotL = saturate(dot(n, n));
            nDotH = saturate(dot(n, h));
            power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess);

            if (dot(rm, l) > 0)
                color += (material.ambient * (/*globalAmbient +*/ (atten * pLight[cnt].ambient))) +
                    (material.diffuse * pLight[cnt].diffuse * nDotL * atten) +
                    (material.specular * pLight[cnt].specular * power * atten);
        }
    }
    //не работает...
    //if (pLight[i % NUM_POINT_LIGHTS].enabled)
        //OUT.RGBColor = tex2D(colorMap, IN.texCoord);
    //else
    
    OUT.RGBColor = color * tex2D(colorMap, IN.texCoord);
    
        //return OUT;
    

    return OUT;
}


// Шейдер точечного освещения(системы машины)

struct CV_VS_INPUT
{
    float3 position : POSITION;
    float2 texCoord : TEXCOORD0;
    float3 normal : NORMAL;
};

struct CV_VS_OUTPUT
{
    float4 position : POSITION;
    float3 worldPos : TEXCOORD0;
    float2 texCoord : TEXCOORD1;
    float3 viewDir : TEXCOORD2;
    float3 normal : TEXCOORD3;
    float3 stnormal : TEXCOORD4;
};

CV_VS_OUTPUT CV_vShader(CV_VS_INPUT IN)
{
    CV_VS_OUTPUT OUT;

    OUT.position = mul(float4(IN.position, 1.0f), worldViewProjectionMatrix);
    OUT.worldPos = mul(float4(IN.position, 1.0f), worldMatrix).xyz;
    OUT.texCoord = IN.texCoord;
    OUT.viewDir = cameraPos - OUT.worldPos;
    OUT.normal = mul(IN.normal, (float3x3)worldInverseTransposeMatrix);
    OUT.stnormal = IN.normal;

    return OUT;
}

GL_PS_OUTPUT CV_pShader(CV_VS_OUTPUT IN) : COLOR
{
    GL_PS_OUTPUT OUT;

    float4 color = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 nColor = float4(0.0f, 0.0f, 0.0f, 0.0f);

    float3 n = normalize(IN.normal);
    float3 v = normalize(IN.viewDir);
    float3 l = float3(0.0f, 0.0f, 0.0f);
    float3 h = float3(0.0f, 0.0f, 0.0f);
    float3 rm = mul(IN.stnormal, worldMatrix);

    float spike = 0.10f;

    float4 nTex = normalize(tex2D(colorMap, IN.texCoord));

    float atten = 0.0f;
    float nDotL = 0.0f;
    float nDotH = 0.0f;
    float power = 0.0f;

    for (int cnt = 0; cnt < NUM_CAR_VISION_LIGHTS; cnt++)
    {      

        if (length(cVLight[cnt].pos - IN.worldPos) < cVLight[cnt].radius)
        {
            nColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
            l = (cVLight[cnt].pos - IN.worldPos) / cVLight[cnt].radius;
            atten = saturate(1.0f - dot(l, l));
            l = normalize(l);
            h = normalize(l + v);

            nDotL = saturate(dot(n, n));
            nDotH = saturate(dot(n, h));
            power = (nDotL == 0.0f) ? 0.0f : pow(nDotH, material.shininess);

            //возврат к нормальной ск. через мм
            if (dot(rm, l) > 0)
                nColor = (material.ambient * (/*globalAmbient +*/ (atten * cVLight[cnt].ambient))) +
                (material.diffuse * cVLight[cnt].diffuse * nDotL * atten) +
                (material.specular * cVLight[cnt].specular * power * atten);
            if (length((normalize(nColor) - nTex).xyz) < spike)
                color += nColor;
        }
    }
    //не работает...
    //if (pLight[i % NUM_POINT_LIGHTS].enabled)
    //OUT.RGBColor = tex2D(colorMap, IN.texCoord);
    //else
    //спец. костыль - подсвечиваются только части текстур с близким к источнику светом
    //if (length((normalize(color) - normalize(tex2D(colorMap, IN.texCoord))).xyz) < spike)
        OUT.RGBColor = color * tex2D(colorMap, IN.texCoord);
    //else
        //OUT.RGBColor = nColor * tex2D(colorMap, IN.texCoord);

    //return OUT;


    return OUT;
}




// Многошаговая техника, обрабатываются все источники света(ОБТ)
// Внимание!!Просадка фпс на слабых гп!!(неоптимизированный код)
technique ALP_MULTICAST
{    

    // Направленный
    pass LC0P0
    {
        ZENABLE = true;
        VertexShader = compile vs_3_0 D_vShader();
        PixelShader = compile ps_3_0 D_pShader();
    }
    
    // "КП"
    pass LC1P0
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = ONE;
        DestBlend = ONE;
        VertexShader = compile vs_3_0 KP_vShader();
        PixelShader = compile ps_3_0 KP_pShader();
    }
      

    // Конусные
    pass LC2P0
    {
        VertexShader = compile vs_3_0 S_vShader();
        PixelShader = compile ps_3_0 S_pShader();
    }
       
    
    // Точечные
    pass LC3P0
    {
        VertexShader = compile vs_3_0 P_vShader();
        PixelShader = compile ps_3_0 P_pShader();
    }

    
    // Точечные(системы машины)
    pass LC4P0
    {
        //AlphaBlendEnable = TRUE;
        //SrcBlend = ONE;
        //DestBlend = ONE;
        VertexShader = compile vs_3_0 CV_vShader();
        PixelShader = compile ps_3_0 CV_pShader();
    }
}

technique HLP_MULTICAST
{
    // Направленный
    pass LC0P0
    {
        ZENABLE = true;
        VertexShader = compile vs_3_0 D_vShader();
        PixelShader = compile ps_3_0 D_pShader();
    }

    // Конусные
    pass LC2P0
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = ONE;
        DestBlend = ONE;
        VertexShader = compile vs_3_0 S_vShader();
        PixelShader = compile ps_3_0 S_pShader();
    }


    // Точечные
    pass LC3P0
    {
        VertexShader = compile vs_3_0 P_vShader();
        PixelShader = compile ps_3_0 P_pShader();
    }
}


technique GLP_MULTICAST
{
    // Направленный
    pass LC0P0
    {
        ZENABLE = true;
        VertexShader = compile vs_3_0 D_vShader();
        PixelShader = compile ps_3_0 D_pShader();
    }

    // "КП"
    pass LC1P0
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = ONE;
        DestBlend = ONE;
        VertexShader = compile vs_3_0 KP_vShader();
        PixelShader = compile ps_3_0 KP_pShader();
    }

    // Конусные
    pass LC2P0
    {
        VertexShader = compile vs_3_0 S_vShader();
        PixelShader = compile ps_3_0 S_pShader();
    }


    // Точечные
    pass LC3P0
    {
        VertexShader = compile vs_3_0 P_vShader();
        PixelShader = compile ps_3_0 P_pShader();
    }
}


//склад полезных резервов \/

//pass LC1P1
//{        
//    VertexShader = compile vs_3_0 KP_vShader();
//    PixelShader = compile ps_3_0 KP_pShader(1);
//}

//pass LC1P2
//{        
//    VertexShader = compile vs_3_0 KP_vShader();
//    PixelShader = compile ps_3_0 KP_pShader(2);
//}

//pass LC1P3
//{        
//    VertexShader = compile vs_3_0 KP_vShader();
//    PixelShader = compile ps_3_0 KP_pShader(3);
//}  

//pass LC2P1
//{
//    VertexShader = compile vs_3_0 S_vShader();
//    PixelShader = compile ps_3_0 S_pShader(1);
//}

//pass LC2P2
//{
//    VertexShader = compile vs_3_0 S_vShader();
//    PixelShader = compile ps_3_0 S_pShader(2);
//}

//pass LC2P3
//{
//    VertexShader = compile vs_3_0 S_vShader();
//    PixelShader = compile ps_3_0 S_pShader(3);
//}

//   pass LC3P1
//   {
//       VertexShader = compile vs_3_0 P_vShader();
//       PixelShader = compile ps_3_0 P_pShader(1);
//   }

//   pass LC3P2
//   {
//       VertexShader = compile vs_3_0 P_vShader();
//       PixelShader = compile ps_3_0 P_pShader(2);
//   }

//   pass LC3P3
//   {
//       VertexShader = compile vs_3_0 P_vShader();
//       PixelShader = compile ps_3_0 P_pShader(3);
//   }

//   pass LC3P4
//   {
//       VertexShader = compile vs_3_0 P_vShader();
//       PixelShader = compile ps_3_0 P_pShader(4);
//   }

//pass LC3P5
//{
//	VertexShader = compile vs_3_0 P_vShader();
//	PixelShader = compile ps_3_0 P_pShader(5);
//}

//pass LC3P6
//{
//	VertexShader = compile vs_3_0 P_vShader();
//	PixelShader = compile ps_3_0 P_pShader(6);
//}

//pass LC3P7
//{
//	VertexShader = compile vs_3_0 P_vShader();
//	PixelShader = compile ps_3_0 P_pShader(7);
//}