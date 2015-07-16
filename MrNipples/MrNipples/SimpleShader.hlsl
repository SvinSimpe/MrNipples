cbuffer CB_PER_FRAME : register(b0)
{
	matrix view;
	matrix proj;
	float3 eyePosition;
};

struct PointLight
{
	float4	positionAndRadius;
	float4	ambient;
	float4	diffuse;
	float4	specular;
	float3	attenuation;
};

cbuffer CB_LIGHT : register(b1)
{
	PointLight	pointLight;
};

struct VertexIn
{
	// Per-vertex data
	float3				position	: POSITION;
	float3				normal		: NORMAL;
	// Per-instance data
	row_major float4x4	world		: WORLD;
	float4				color		: COLOR;
	uint				instanceID	: SV_InstanceID;
};



struct PixelIn
{
	float4 position			: SV_POSITION;
	float3 worldPosition	: WORLD_POSITION;
	float3 normal			: NORMAL;
	float4 color			: COLOR;
};

PixelIn VS( VertexIn input )
{
	PixelIn output = (PixelIn)0;

	output.position			= mul( float4( input.position, 1.0f ), input.world );
	output.worldPosition	= output.position.xyz;
	output.position			= mul( output.position, view );
	output.position			= mul( output.position, proj );

	output.normal	= normalize( mul( input.normal, (float3x3)input.world ) );
	output.color	= input.color;

	return output;
}

float4 PS( PixelIn input ) : SV_Target
{
	//////////////////////////////////////////////////////////
	/*	IF LIGHTING SEEMS INCORRECT, CONSIDER THE NORMAL
		CALCULATION WHEN OBJECT IS TRANSFORMED. SEE p.277
		IN FRANK D. LUNA ( INVERSE TRANSPOSE )				*/
	//////////////////////////////////////////////////////////

	//return float4( input.normal, 1.0f );

	//return float4( input.color );


	//						LIGHTING
	///====================================================
	float3  finalDiffuse	= float3( 0.02f, 0.02f, 0.02f );
	float3	lightVec		= pointLight.positionAndRadius.xyz - input.worldPosition;
	float	lightVecLength	= length( lightVec );
	
	if( lightVecLength > pointLight.positionAndRadius.w ) 
		return float4( finalDiffuse * input.color, 1.0f );
	
	lightVec /= lightVecLength;
	float diffuseFactor = saturate( dot( lightVec, input.normal ) );		

	float finalAtt	= 1.0f / (	pointLight.attenuation[0] + pointLight.attenuation[1] *
								lightVecLength + pointLight.attenuation[2] *
								lightVecLength * lightVecLength );

	finalDiffuse	= diffuseFactor * pointLight.diffuse.xyz * input.color;/* * tex.Sample( samplerState, input.texCoord ).xyz;*/
	finalDiffuse	*= finalAtt;

	return float4( finalDiffuse, 1.0f );
	///====================================================
}