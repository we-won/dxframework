
cbuffer cbPerObject
{
	float4x4 WVP;
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 depthPosition : TEXCOORD0;
};

VS_OUTPUT VS(float4 inPos : POSITION)
{
	VS_OUTPUT output;
	
    output.Pos = mul(inPos, WVP);
	
	output.depthPosition = output.Pos;
	
    return output;
}

float4 PS(VS_OUTPUT input) : SV_TARGET
{
	float depthValue;
	float4 color;
	
	depthValue = input.depthPosition.z / input.depthPosition.w;
	
	color = float4(depthValue, depthValue, depthValue, 1.0f);
	
	return color;
}

