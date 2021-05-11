cbuffer ConstBuffer
{
	matrix	matView;
	matrix	matProj;
};




struct VT_INPUT
{
	float4	pos : POSITION;
	float4	uv	: TEXCOORD;
};
struct VT_OUTPUT
{
	float4 pos : SV_POSITION;
	float4 uv	: TEXCOORD;
};

Texture2D		Texture;
SamplerState	Sampler;

VT_OUTPUT	VS_PT(VT_INPUT _input)
{
	VT_OUTPUT result;

	result.pos	= mul(_input.pos, matView);
	result.pos	= mul(result.pos, matProj);
	result.uv	= _input.uv;

	return result;
}

float4	PS_PT(VT_OUTPUT _input) : SV_Target
{
	float finalColor = 0;

	//	최종 조명계산
	finalColor = float4(1, 1, 1, 1);

	return Texture.Sample(Sampler, _input.uv) * finalColor;
}