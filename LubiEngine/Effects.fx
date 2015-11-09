struct Light
{
	float3 dir;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	Light light;
	float4x4 VP;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
};

Texture2D ObjTexture;
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
	
	output.lightViewPosition = mul(inPos, World);
    //output.lightViewPosition = mul(inPos, VP);
	
	output.TexCoord = inTexCoord;
	
	output.normal = mul(normal, World);
	
	worldPosition = mul(inPos, World);
	//output.lightPos = light.dir.xyz - worldPosition.xyz;
	
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
	
	input.normal = normalize(input.normal);
	
	bias = 0.001f;
	
	color = light.ambient;
	
	projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
	
	if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		depthValue = ObjTexture.Sample(ObjSamplerState, projectTexCoord).r;
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		lightDepthValue = lightDepthValue - bias;
		
		if(lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(input.normal, input.lightPos));

		    if(lightIntensity > 0.0f)
			{
				color += (light.diffuse * lightIntensity);
				color = saturate(color);
			}
		}
	}
	
	textureColor = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
    
	color = color * textureColor;
    
    return color;
	
	// Old 
	/*input.normal = normalize(input.normal);
	
	float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
    
	float3 finalColor;
	finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);
    
    return float4(finalColor, diffuse.a);*/
	
}

