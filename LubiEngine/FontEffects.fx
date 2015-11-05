cbuffer cbPerObject
{
	float4x4 WVP;
};

Texture2D ObjTexture;
SamplerState ObjSamplerState;

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float2 TexCoord : TEXCOORD;
};

VS_OUTPUT VS(float4 inPos : POSITION, float2 inTexCoord : TEXCOORD)
{
	VS_OUTPUT output;
	
    output.Pos = mul(inPos, WVP);
    output.TexCoord = inTexCoord;

    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float4 color = ObjTexture.Sample( ObjSamplerState, input.TexCoord );
    
	if(color.r < 0.5f)
	{
		color.a = 0.0f;
		color.r = 0.0f;
		color.g = 0.0f;
		color.b = 0.0f;
	}

    return color;
}