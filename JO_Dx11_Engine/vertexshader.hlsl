cbuffer mycBuffer : register(b0)
{
    float4x4 mat;
	float4x4 gWorldView;
	float4x4 gWorldViewProj;
	float4x4 gBoneTransforms[69];
};


struct VS_INPUT
{
    float3 inPos : POSITION;
    float2 inTexCoord : TEXCOORD;
	float3 Normal : NORMAL;
	float4 Weights : WEIGHTS;
	uint4  BoneIndices : BONEINDICES;
};

struct VS_OUTPUT
{
    float4 outPosition : SV_POSITION;
    float2 outTexCoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input) 
{
    VS_OUTPUT output;
    output.outPosition = mul(float4(input.inPos, 1.0f), mat);
    output.outTexCoord = input.inTexCoord;
    return output;
}