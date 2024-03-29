cbuffer ConstantBuffer
{
    float4x4 transformation;
    float4x4 rotation;    // the rotation matrix
    float4 lightVector;      // the light's vector
    float4 lightColor;      // the light's color
    float4 ambientColor;    // the ambient light's color
	float4 diffuseColor;	// the color of material
}

Texture2D Texture;
SamplerState ss;

struct VOut
{
	float4 position : SV_POSITION;
    float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VOut VShader(float4 position : POSITION, float4 normal : NORMAL, float2 texcoord : TEXCOORD)
{
    VOut output;

    output.position = mul(transformation, position);

    // set the ambient light
    output.color = ambientColor;

    // calculate the diffuse light and add it to the ambient light

	float4 vectorBackToLight = -lightVector;
    float4 adjustedNormal = normalize(mul(rotation, normal));
    float diffusebrightness = saturate(dot(adjustedNormal, vectorBackToLight));
    output.color += lightColor * diffusebrightness;

	output.texcoord = texcoord;

    return output;
}

float4 PShader(float4 position : POSITION, float4 color : COLOR, float2 texcoord : TEXCOORD, float4 diffuseColor : COLOR) : SV_TARGET
{
    return color * Texture.Sample(ss, texcoord) * diffuseColor;
}