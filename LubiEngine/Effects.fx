cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
	float4x4 LightWVP;
};

cbuffer LightBuffer2
{
    float3 lightPosition;
	float padding;
};

Texture2D ObjTexture : register(t0);
Texture2D depthMapTexture : register(t1);

SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPosition : TEXCOORD1;
	float3 lightPos : TEXCOORD2;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;
	float4 worldPosition;

    output.Pos = mul(inPos, WVP);
	
	output.lightViewPosition = mul(inPos, LightWVP);
	 
	output.TexCoord = inTexCoord;
	
	output.normal = mul(normal, (float3x3)World);
	output.normal = normalize(output.normal);
	
	worldPosition = mul(inPos, World);
	output.lightPos = lightPosition.xyz - worldPosition.xyz;
	output.lightPos = normalize(output.lightPos);
	
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float bias;
	float4 color;
	float2 projectTexCoord;
	float depthValue;
	float lightDepthValue;
    float lightIntensity;
	float4 textureColor;

	bias = 0.001f;
	
	color = ambientColor;
	
	projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
	
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = depthMapTexture.Sample(ObjSamplerState, projectTexCoord).r;

		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		lightDepthValue = lightDepthValue - bias;

		//if(lightDepthValue < depthValue)
		if(true)
		{
			lightIntensity = saturate(dot(input.normal, input.lightPos));

		    if(lightIntensity > 0.0f)
			{
				color += (diffuseColor * lightIntensity);
				color = saturate(color);
			}
		}
	}
	
	textureColor = ObjTexture.Sample(ObjSamplerState, input.TexCoord);
    
	color = color * textureColor;
    
    return color;
}

