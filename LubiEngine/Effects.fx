struct Light
{
	float3 dir;
	float padding;
	float4 ambient;
	float4 diffuse;
};

cbuffer cbPerFrame
{
	Light light;
};

cbuffer cbPerObject
{
	float4x4 WVP;
	float4x4 World;
	float4x4 LightWVP;
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
	float4 lightPos : TEXCOORD2;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD, float3 normal : NORMAL)
{
	VS_OUTPUT output;
	float4 worldPosition;
	float4 tmpLightPos = float4(1.0f, 8.0f, -5.0f, 1.0f);
	
    output.Pos = mul(inPos, WVP);
	
	output.lightViewPosition = mul(inPos, LightWVP);
	 
	output.TexCoord = inTexCoord;
	
	output.normal = mul(normal, World);
	output.normal = normalize(output.normal);
	
	worldPosition = mul(inPos, World);
	output.lightPos = tmpLightPos.xyzw - worldPosition.xyzw;
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
	
	color = light.ambient;
	
	projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.y = -input.lightViewPosition.y / input.lightViewPosition.w / 2.0f + 0.5f;
	
	if ((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// Sample the shadow map depth value from the depth texture using the sampler at the projected texture coordinate location.
		depthValue = depthMapTexture.Sample(ObjSamplerState, projectTexCoord).r;

		// Calculate the depth of the light.
		lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;

		// Subtract the bias from the lightDepthValue.
		lightDepthValue = lightDepthValue - bias;

		// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
		// If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it.
		// if(lightDepthValue < depthValue)
		if(true)
		{
		    // Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, input.lightPos));

		    if(lightIntensity > 0.0f)
			{
				// Determine the final diffuse color based on the diffuse color and the amount of light intensity.
				color += (light.diffuse * lightIntensity);

				// Saturate the final light color.
				color = saturate(color);
			}
		}
	}
	
	textureColor = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
    
	color = color * textureColor;
    
    return color;
	
	// Old 
	/*
	input.normal = normalize(input.normal);
	
	float4 diffuse = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
    
	float3 finalColor;
	finalColor = diffuse * light.ambient;
	finalColor += saturate(dot(light.dir, input.normal) * light.diffuse * diffuse);
    
    return float4(finalColor, diffuse.a);
	*/
}

