//	�������
cbuffer		ConstBuffer
{
	matrix	matWorld;	//	���� �������
	matrix	matTotal;

	float4	lightDir;
	float4	lightColor;
};

struct VS_OUTPUT
{
	float4 Pos		: SV_POSITION;
	float4 Color	: COLOR;
};

//	���� ���̴�
float4	VS( float4 pos : POSITION ) : SV_POSITION
{
	pos = mul(pos, matTotal);

	return pos;
}
//	�ȼ� ���̴�
float4	PS(float4 pos : SV_POSITION) : SV_Target
{
	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}

//	���� ���̴�
VS_OUTPUT	VS_Color(float4 Pos : POSITION, float4 Color : COLOR)
{
	VS_OUTPUT	Out = (VS_OUTPUT)0;

	Out.Pos		= mul(Pos, matTotal);
	Out.Color	= Color;

	return Out;
}
//	�ȼ� ���̴�
float4 PS_Color(VS_OUTPUT Input) : SV_Target
{
	return Input.Color;
}
//////////////////////////////////////////////////////////////////
struct VS_PNC_INPUT
{
	float4		pos		: POSITION;
	float3		normal	: NORMAL;
	float4		color	: COLOR;
};
struct VS_PNC_OUTPUT
{
	float4		pos		: SV_POSITION;
	float3		normal	: NORMAL;
	float4		color	: COLOR;
};

//	���� ���̴�
VS_PNC_OUTPUT	VS_PNC(VS_PNC_INPUT _input)
{
	VS_PNC_OUTPUT	Out = (VS_PNC_OUTPUT)0;

	Out.pos		= mul(_input.pos, matTotal);
	Out.normal	= mul(float4(_input.normal, 0), matWorld).xyz;
	Out.color	= _input.color;

	return Out;
}
float4	PS_PNC(VS_PNC_OUTPUT _input) : SV_Target
{
	//	�Ի簢�� �����ؼ� 0~1�� �����, ���� ���� * ���� ����
	float	rate		= -1 * dot((float3)lightDir, _input.normal);
	float4	finalcolor = saturate(rate * lightColor + float4(0.2f, 0.2f, 0.2f, 1) );

	return  _input.color * finalcolor;
}