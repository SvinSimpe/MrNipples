//========================
// DepthVertexShader.sh ||
//========================

cbuffer CB_PER_FRAME : register(b0)
{
	matrix view;
	matrix proj;
	float4 eyePositionAndTessAmount;
};

cbuffer CB_DEPTH_LIGHT : register(b1)
{
	matrix worldViewProj;
}

struct VertexIn // Pass through
{
	// Per-vertex data
	float3				position	: POSITION;
	float3				normal		: NORMAL;
	float2				texCoord	: TEXCOORD;
	float3				tangent		: TANGENT;
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
	float3				tangent		: TANGENT;
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
	float3 position	: POSITION;
	float3 normal	: NORMAL;
	float2 texCoord	: TEX;
	float3 tangent	: TANGENT;
};

struct DomainOut
{
	float4 position			: SV_POSITION;
};


Texture2D		displacementMap		: register(t0);
SamplerState	samplerState		: register(s0);


//=================
//	VERTEX SHADER
//=================
VertexOut VS( VertexIn input )
{
	VertexOut output	= (VertexOut)0;

	output.position		= input.position;
	output.normal		= input.normal;
	output.texCoord		= input.texCoord;
	output.tangent		= input.tangent;
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
	
	float d = distance( centerW, eyePositionAndTessAmount.xyz );

	// Tessellate the patch based on distance from the eye such that
	// the tessellation is 0 if d >= d1 and 60 if d <= d0.  The interval
	// [d0, d1] defines the range we tessellate in.
	const float d1 = 5000.0f;
	const float d0 = 1000.0f;
	float tess = 16*saturate( ( d1-d ) / ( d1-d0 ) );

	// Tessellation rate/speed
	if( tess <= 1.0f)
		tess = 1.0f;

	output.edgeFactor[0]	= tess; // BLOCK 8
	output.edgeFactor[1]	= tess;	// BLOCK 8
	output.edgeFactor[2]	= tess;	// BLOCK 8
	output.insideFactor		= tess;	// BLOCK 8

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
	output.tangent	= inputPatch[cpid].tangent;
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
   
	// Interpolate world space position with barycentric coordinates
	float3 worldPosition	= baryCoords.x * tri[0].position + baryCoords.y * tri[1].position + baryCoords.z * tri[2].position;
	
	// Normal
	float3 finalNormal = baryCoords.x * tri[0].normal + baryCoords.y * tri[1].normal + baryCoords.z * tri[2].normal;
	//output.normal = normalize( mul( float4( finalNormal, 0 ), tri[0].world ).xyz );

	// Texture coordinate
	float2 finalTex = baryCoords.x * tri[0].texCoord + baryCoords.y * tri[1].texCoord + baryCoords.z * tri[2].texCoord;
	//output.texCoord = finalTex;

	// Tangent
	/*float3 finalTangent	= baryCoords.x * tri[0].tangent + baryCoords.y * tri[1].tangent + baryCoords.z * tri[2].tangent;
*/

	// Sample the displacement map for the magnitude of displacement
	float				displacement	= displacementMap.SampleLevel( samplerState, finalTex.xy, 0 ).r;
	//float				scalar = 0.025f;
	displacement		*= 0.058f;
	float3	direction	= finalNormal; // direction is opposite normal

	// Translate the position
	//worldPosition += ( scalar * ( displacement - 1.0f ) ) * direction;
	worldPosition += direction * displacement;

	// transform to clip space
	output.position			= mul( float4( worldPosition, 1.0f ), tri[0].world );
	//output.worldPosition	= output.position.xyz;
	//output.position		= mul( output.position, view );
	//output.position		= mul( output.position, proj );
	output.position			= mul( output.position, worldViewProj );
	//output.tangent			= normalize( finalTangent );
	//output.color			= tri[0].color;
    
    return output;    
}


void PS( DomainOut input )
{

}