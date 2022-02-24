
Texture2D texDiffuse : register(t0);
Texture2D texNormal : register(t1);

struct PSIn
{
	float4 Pos  : SV_Position;
	float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float3 Binormal : BINORMAL;
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

SamplerState texSampler : register(s0);

float4 PS_main(PSIn input) : SV_Target
{
	float4 diffuseTexture = texDiffuse.Sample(texSampler, input.TexCoord);
    float4 normalTexture = texNormal.Sample(texSampler, input.TexCoord) * 2 -1;
	
    float3x3 TBN = transpose(float3x3(input.Tangent, input.Binormal, input.Normal));
	
    float3 newNormal = mul(TBN, normalTexture.xyz);
	
    float3 light = normalize(lightPosition - input.WorldPos);
    float3 reflection = normalize(reflect(-light, newNormal));
    float3 view = normalize(cameraPosition - input.WorldPos);
		
    float3 A = ambient.xyz * 0.1f;
    float3 D = mul(diffuseTexture.xyz, max(dot(light, newNormal), 0.0f));
    float3 S = mul(specular.xyz, pow(max(dot(reflection, view), 0.0f), 200.0f));

	//Phong material shading
	return float4(A + D + S, 1);
	
	//Debug shading #1: return tangent or binormal as color
    //return float4(input.Tangent * 0.5 + 0.5, 1);
    //return float4(input.Binormal * 0.5 + 0.5, 1);
	
	//Debug shading #2: map and return normal as a color, i.e. from [-1,1]->[0,1] per component
	//return float4(input.Normal*0.5+0.5, 1);
	
	// Debug shading #3: map and return texture coordinates as a color (blue = 0)
	//return float4(input.TexCoord, 0, 1);
}