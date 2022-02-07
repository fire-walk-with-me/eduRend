
Texture2D texDiffuse : register(t0);

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
	float2 TexCoord : TEX;
	float3 WorldPos : POS;
};

//-----------------------------------------------------------------------------------------
// Pixel Shader
//-----------------------------------------------------------------------------------------

cbuffer CameraAndLightBuffer : register(b0)
{
	float3 cameraPosition;
	float3 lightPosition;
};

cbuffer ColorAndShininessBuffer : register(b1)
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float1 shininess;
};

SamplerState texSampler : register(s0)
{

};

float4 PS_main(PSIn input) : SV_Target
{
	float3 light = normalize(lightPosition - input.WorldPos);
	float3 reflection = normalize(reflect(-light, input.Normal));
	float3 view = normalize(cameraPosition - input.WorldPos);

	float3 A = ambient.xyz * 0.5f;
	float3 D = mul(diffuse.xyz, dot(light, input.Normal));
	float3 S = mul(specular.xyz, pow(max(dot(reflection, view), 0), 200.0f));

	return float4(A + D + S, 1);

	//return float4(
	//	ambient.xyz +												//ambient
	//	mul(diffuse.xyz, light * input.Normal) +					//diffuse
	//	mul(specular.xyz, pow(max(reflection * view, 0), 200.0f)),	//specular
	//	1);															//opacity

	// Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	// The 4:th component is opacity and should be = 1
	//return float4(input.Normal*0.5+0.5, 1);
	
	// Debug shading #2: map and return texture coordinates as a color (blue = 0)
	//return float4(input.TexCoord, 0, 1);
}