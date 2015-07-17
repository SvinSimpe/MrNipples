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

struct VertexIn // Pass through
{
	// Per-vertex data
	float3				position	: POSITION;
	float3				normal		: NORMAL;
	float2				texCoord	: TEXCOORD;
	// Per-instance data
	row_major float4x4	world		: WORLD;
	float4				color		: COLOR;
	uint				instanceID	: SV_InstanceID;
};

struct VertexOut
{
	// Per-vertex data
	float3				position	: POSITION;
	float3				normal		: NORMAL;
	float2				texCoord	: TEXCOORD;
	// Per-instance data
	row_major float4x4	world		: WORLD;
	float4				color		: COLOR;
};

struct HullConstantOut
{
	float edgeFactor[3]	: SV_TessFactor;
	float insideFactor	: SV_InsideTessFactor;
};

struct HullOut // Pass through
{
	float3 position			: POSITION;
	float3 normal			: NORMAL;
	float2 texCoord			: TEX;
};

struct DomainOut
{
	float4 position			: SV_POSITION;
	float3 worldPosition	: WORLD_POSITION;
	float3 normal			: NORMAL;
	float2 texCoord			: TEXCOORD;
	float4 color			: COLOR;
};

Texture2D		tex				: register(t0);
SamplerState	samplerState	: register(s0);


//=================
//	VERTEX SHADER
//=================
VertexOut VS( VertexIn input )
{
	VertexOut output	= (VertexOut)0;

	output.position		= input.position;
	output.normal		= input.normal;
	output.texCoord		= input.texCoord;
	output.world		= input.world;
	output.color		= input.color;
	

	//output.position			= mul( float4( input.position, 1.0f ), input.world );
	//output.worldPosition		= output.position.xyz;
	//output.position			= mul( output.position, view );
	//output.position			= mul( output.position, proj );

	//output.normal		= normalize( mul( input.normal, (float3x3)input.world ) );
	//output.texCoord		= input.texCoord;
	//output.color		= input.color;

	return output;
}


//========================
//	CONSTANT HULL SHADER
//========================
HullConstantOut HSConstant( InputPatch<VertexOut, 3> patch, uint pid : SV_PrimitiveID )
{
	HullConstantOut output = (HullConstantOut)0;
	

	float3 centerL = 0.25f*( patch[0].position + patch[1].position + patch[2].position );

	float3 centerW = mul( float4( centerL, 1.0f ), patch[0].world ).xyz;
	
	float d = distance( centerW, eyePosition );

	// Tessellate the patch based on distance from the eye such that
	// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
	// [d0, d1] defines the range we tessellate in.
	const float d1 = 500.0f;
	const float d0 = 10.0f;
	float tess = 8*saturate( ( d1-d ) / ( d1-d0 ) );

	// Tessellation rate/speed
	if( tess <= 1.0f)
		tess = 1.0f;

	output.edgeFactor[0]	= tess;
	output.edgeFactor[1]	= tess;
	output.edgeFactor[2]	= tess;
	output.insideFactor		= tess;

	return output;
}

//===============
//	HULL SHADER
//===============
[domain("tri")]
[partitioning("fractional_odd")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HSConstant")]
//[maxtessfactor(7.0)]
VertexOut HS( InputPatch<VertexOut, 3> inputPatch, uint cpid : SV_OutputControlPointID, uint pid : SV_PrimitiveID )
{
	VertexOut output = (VertexOut)0;

	output.position	= inputPatch[cpid].position;
	output.normal	= inputPatch[cpid].normal;
	output.texCoord	= inputPatch[cpid].texCoord;
	output.world	= inputPatch[cpid].world;
	output.color	= inputPatch[cpid].color;

    return output;
}


//=================
//	DOMAIN SHADER
//=================
[domain("tri")]
DomainOut DS( HullConstantOut input, float3 baryCoords : SV_DomainLocation, const OutputPatch<VertexOut, 3> tri )
{
    DomainOut output = (DomainOut)0;
   
	float3 worldPosition	= baryCoords.x * tri[0].position + baryCoords.y * tri[1].position + baryCoords.z * tri[2].position;
	
	// Displacement here
	

	output.position			= mul( float4( worldPosition, 1.0f ), tri[0].world );
	output.worldPosition	= output.position.xyz;
	output.position			= mul( output.position, view );
	output.position			= mul( output.position, proj );
	output.color			= float4( 0.93f, 0.82f, 0.47f, 1.0f );
    output.color			= float4( baryCoords.yx, 1-baryCoords.x, 1 );
	

	// Normal
	float3 finalNormal = baryCoords.x * tri[0].normal + baryCoords.y * tri[1].normal + baryCoords.z * tri[2].normal;
	output.normal = mul( float4( finalNormal, 0 ), tri[0].world ).xyz;


	// Texture coordinate
	float2 finalTex = baryCoords.x * tri[0].texCoord + baryCoords.y * tri[1].texCoord + baryCoords.z * tri[2].texCoord;
	output.texCoord = finalTex;
    
    return output;    
}



//=================
//	PIXEL SHADER
//=================
float4 PS( DomainOut input ) : SV_Target
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
	float3  finalDiffuse	= float3( 0.01f, 0.01f, 0.01f );
	float3	lightVec		= pointLight.positionAndRadius.xyz - input.worldPosition;
	float	lightVecLength	= length( lightVec );
	
	if( lightVecLength > pointLight.positionAndRadius.w ) 
		return float4( finalDiffuse * input.color.xyz, 1.0f );

	
	lightVec /= lightVecLength;
	float diffuseFactor = saturate( dot( lightVec, input.normal ) );		

	float finalAtt	= 1.0f / (	pointLight.attenuation[0] + pointLight.attenuation[1] *
								lightVecLength + pointLight.attenuation[2] *
								lightVecLength * lightVecLength );

	finalDiffuse	= diffuseFactor * pointLight.diffuse.xyz  /* * input.color; */ * tex.Sample( samplerState, input.texCoord ).xyz;
	finalDiffuse	*= finalAtt;

	return float4( finalDiffuse, 1.0f );
	///====================================================
}