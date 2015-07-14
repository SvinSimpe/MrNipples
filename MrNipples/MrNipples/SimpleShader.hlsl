cbuffer CB_PER_FRAME : register(b0)
{
	matrix view;
	matrix proj;
	float3 eyePosition;
};

cbuffer CB_PER_OBJECT : register(b1)
{
	matrix world;
	float3 color;
};

struct VertexIn
{
	float3 position	: POSITION;
	float3 normal	: NORMAL;

};

struct PixelIn
{
	float4 position	: SV_POSITION;
	float3 normal	: NORMAL;
};

PixelIn VS( VertexIn input )
{
	PixelIn output = (PixelIn)0;

	output.position = mul( float4( input.position, 1.0f ), world );
	output.position	= mul( output.position, view );
	output.position	= mul( output.position, proj );

	output.normal	= normalize( mul( input.normal, (float3x3)world ) );

	return output;
}

float4 PS( PixelIn input ) : SV_Target
{
	return float4( color, 0.0f );
}