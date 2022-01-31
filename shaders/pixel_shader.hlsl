
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
	//float4x4 materials;
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 shininess;
};

float4 PS_main(PSIn input) : SV_Target
{
	float3 light = normalize(lightPosition - input.WorldPos);
	float3 reflection = normalize(reflect(light, input.Normal));
	float3 view = normalize(cameraPosition - input.WorldPos);

	return float4(
			ambient.xyz +												//ambient
			mul(diffuse.xyz, light * input.Normal) +					//diffuse
			mul(specular.xyz, pow(reflection * view, 1.0f)),			//specular
			1);															//opacity


	// Debug shading #1: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	// The 4:th component is opacity and should be = 1
	//return float4(input.Normal*0.5+0.5, 1);
	
	// Debug shading #2: map and return texture coordinates as a color (blue = 0)
	//return float4(input.TexCoord, 0, 1);

	//Debug shading #3: map and return materials as color
	//return float4(input.Color, 0, 1);
}