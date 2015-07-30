/////////////////////////////
//		Vertex Shader
/////////////////////////////

struct VertexIn 
{
	// Per-vertex data
	float3				position	: POSITION;
	float3				normal		: NORMAL;
	float2				texCoord	: TEXCOORD;
};

struct VertexOut
{
	float4 position		: SV_Position;
	float3 normal		: NORMAL;
	float2 texCoord		: TEXCOORD;
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

VertexOut VS( VertexIn input )
{
	VertexOut output	= (VertexOut)0;
	output.position		= float4( input.position, 1.0f );
	output.normal		= input.normal;
	output.texCoord		= input.texCoord;

	return output;
}


/////////////////////////////
//		Pixel Shader
/////////////////////////////
Texture2D		worldPosTexture		: register( t0 );
Texture2D		normalTexture		: register( t1 );
Texture2D		diffuseTexture		: register( t2 );
Texture2D		specularTexture		: register( t3 );
Texture2D		shadowTexture		: register( t4 );

SamplerState	samplerState;

float4 PS( VertexOut input ) : SV_Target
{	
	float3 worldPosSample	= worldPosTexture.Sample( samplerState, input.texCoord ).xyz;
	float3 normalSample		= normalTexture.Sample( samplerState, input.texCoord ).xyz;
	float3 diffuseSample	= diffuseTexture.Sample( samplerState, input.texCoord ).xyz;
	float3 specularSample	= specularTexture.Sample( samplerState, input.texCoord ).xyz;	
	float3 shadowSample		= shadowTexture.Sample( samplerState, input.texCoord ).xyz;	

	//return float4( shadowSample, 1.0f );

	//return float4( diffuseSample * specularSample, 1.0f );

	//						LIGHTING
	///====================================================
	float3  finalDiffuse	= float3( 0.01f, 0.01f, 0.01f );
	float3	lightVec		= pointLight.positionAndRadius.xyz - worldPosSample;
	float	lightVecLength	= length( lightVec );
	
	if( lightVecLength > pointLight.positionAndRadius.w )
		return float4( finalDiffuse * diffuseSample, 1.0f );  // pointLight.diffuse.xyz * diffuseSample;

		

	
	lightVec /= lightVecLength;
	float diffuseFactor = saturate( dot( lightVec, normalSample ) );		

	float finalAtt	= 1.0f / (	pointLight.attenuation[0] + pointLight.attenuation[1] *
								lightVecLength + pointLight.attenuation[2] *
								lightVecLength * lightVecLength );

	finalDiffuse	= diffuseFactor * pointLight.diffuse.xyz * diffuseSample;
	finalDiffuse	*= finalAtt;

	float4 finalSpecular = float4( finalDiffuse * specularSample, 1.0f );

	return float4( finalDiffuse + finalSpecular.xyz , 1.0f );
	///====================================================
}