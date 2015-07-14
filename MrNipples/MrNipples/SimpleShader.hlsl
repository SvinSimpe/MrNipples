cbuffer CB_PER_FRAME : register(b0)
{
	matrix view;
	matrix proj;
	float3 eyePosition;
};

struct VertexIn
{
	float3				position	: POSITION;
	float3				normal		: NORMAL;

	// Instanced data
	row_major float4x4	world		: WORLD;
	float4				color		: COLOR;
	uint				instanceID	: SV_InstanceID;

};

struct PixelIn
{
	float4 position	: SV_POSITION;
	float3 normal	: NORMAL;
	float4 color	: COLOR;
};

PixelIn VS( VertexIn input )
{
	PixelIn output = (PixelIn)0;

	output.position = mul( float4( input.position, 1.0f ), input.world );
	output.position	= mul( output.position, view );
	output.position	= mul( output.position, proj );

	output.normal	= normalize( mul( input.normal, (float3x3)input.world ) );
	output.color	= input.color;

	return output;
}

float4 PS( PixelIn input ) : SV_Target
{
	//return float4( input.normal, 1.0f );
	return float4( input.color );
}