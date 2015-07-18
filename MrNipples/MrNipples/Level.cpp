#include "Level.h"

HRESULT Level::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth			= sizeof(Vertex48) * NUM_VERTICES_PER_OBJECT;
	vbd.StructureByteStride = sizeof(Vertex48);
	vbd.Usage				= D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags		= 0;
	vbd.MiscFlags			= 0;
	

	///TEST
	GeometryBox* box = new GeometryBox( XMFLOAT3( 0.0f, 0.0f, 0.0f ), 1.0f, 1.0f, 1.0f );

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.SysMemPitch		= 0;
	vinitData.SysMemSlicePitch	= 0;
	vinitData.pSysMem			= &box->VertexFaces()[0];

	hr = mDevice->CreateBuffer( &vbd, &vinitData, &mObjectVertexBuffer );
	
	return hr;
}

HRESULT Level::CreatePerInstanceBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC ibDesc;
	ibDesc.ByteWidth			= sizeof( PerInstanceData ) * mInstanceData.size();
	ibDesc.Usage				= D3D11_USAGE_DYNAMIC;
	ibDesc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	ibDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	ibDesc.MiscFlags			= 0;
	ibDesc.StructureByteStride	= 0;

	hr = mDevice->CreateBuffer( &ibDesc, nullptr, &mInstanceBuffer );

	return hr;
}

HRESULT Level::CreateLightBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC lbDesc;
	lbDesc.ByteWidth			= sizeof( PointLightData ) * mPointLightData.size();
	lbDesc.Usage				= D3D11_USAGE_DYNAMIC;
	lbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	lbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	lbDesc.MiscFlags			= 0;
	lbDesc.StructureByteStride	= 0;

	hr = mDevice->CreateBuffer( &lbDesc, nullptr, &mLightBuffer );

	return hr;
}

HRESULT Level::UpdateObjectVertexBuffer()
{
	HRESULT hr = S_OK;
	
	return hr;
}

HRESULT Level::UpdatePerInstanceBuffer()
{
	HRESULT hr = S_OK;
		
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	
	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mInstanceData[0], sizeof(PerInstanceData) * mInstanceData.size() );
	    mDeviceContext->Unmap( mInstanceBuffer, 0 ); 
	}

	return hr;
}

HRESULT Level::UpdateLightBuffer()
{
	HRESULT hr = S_OK;
	
	float zMin = -50.0f;
	float zMax = 500.0f;

	if( mPointLightData.at(0).positionAndRadius.z < zMax && mDirection == 1 )
		mPointLightData.at(0).positionAndRadius.z += 0.005f;

	if( mPointLightData.at(0).positionAndRadius.z > zMax )
		mDirection = 0;

	if( mPointLightData.at(0).positionAndRadius.z > zMin && mDirection == 0 )
		mPointLightData.at(0).positionAndRadius.z -= 0.005f;

	if( mPointLightData.at(0).positionAndRadius.z < zMin )
		mDirection = 1;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	
	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mPointLightData[0], sizeof(PointLightData) * mPointLightData.size() );
	    mDeviceContext->Unmap( mLightBuffer, 0 ); 

		// Set constant buffer to shader stages
		mDeviceContext->VSSetConstantBuffers( 1, 1, &mLightBuffer );
		mDeviceContext->PSSetConstantBuffers( 1, 1, &mLightBuffer );
	}

	return hr;
}

void Level::AddBox( XMFLOAT3 scale, XMFLOAT3 rotation, XMFLOAT3 translation, XMFLOAT3 color )
{
	PerInstanceData newInstance;

	// Construct World matrix
	XMStoreFloat4x4( &newInstance.world,  XMMatrixScaling( scale.x, scale.y, scale.z ) *
															XMMatrixRotationRollPitchYaw( XMConvertToRadians( rotation.x ),
																						  XMConvertToRadians( rotation.y ),
																						  XMConvertToRadians( rotation.z ) ) *
															XMMatrixTranslation( translation.x, translation.y, translation.z ) ) ;

	newInstance.color = XMFLOAT4( color.x, color.y, color.z, 1.0f );

	// Add new instance to collection
	mInstanceData.push_back( newInstance );
}

void Level::CheckCollision()
{
	/*
		Content of geometry list
		------------------------
		1		Mr Nipples head
		2->N	Mr Nipples body
		N+1		Apple
		N+2->K	Boxes	
		------------------------

		Perform collision check with
		head VS All other objects in list
		starting three body parts from head.
		Mr Nipples CANNOT collide with his
		"neck" or iow first part of his body
	*/

}

void Level::Update( float deltaTime )
{
	mRotation += deltaTime * 0.2f;

	if( FAILED( UpdateObjectVertexBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE OBJECT VERTEX BUFFER :: Level.cpp" );

	if( FAILED( UpdatePerInstanceBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE PER INSTANCE BUFFER :: Level.cpp" );

	if( FAILED( UpdateLightBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE LIGHT BUFFER :: Level.cpp" );
}

void Level::Render( float deltaTime )
{
	//=============Textures=============

	//--------------Brick---------------
	mDeviceContext->DSSetShaderResources( 0, 1, &mBrickTextureData.displacementMap );
	mDeviceContext->DSSetSamplers( 0, 1, &mSamplerState );
	ID3D11ShaderResourceView* pixelViews[3] = { mBrickTextureData.colorMap, mBrickTextureData.specularMap, mBrickTextureData.normalMap };
	mDeviceContext->PSSetShaderResources( 1, 3, pixelViews );
	
	//----------------------------------

	mDeviceContext->PSSetSamplers( 0, 1, &mSamplerState );
	//mDeviceContext->PSSetShaderResources( 0, 1, &mShaderResourceView );

	UINT32 stride[2]				= { sizeof(Vertex48), sizeof(PerInstanceData) };
	UINT32 offset[2]				= { 0, 0 };
	ID3D11Buffer* buffersToSet[2]	= { mObjectVertexBuffer, mInstanceBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, stride, offset );

	mDeviceContext->DrawInstanced( NUM_VERTICES_PER_OBJECT, mInstanceData.size(), 0, 0 );
}

HRESULT Level::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;

					
	//=================GEOMETRY===================
	for ( size_t i = 0; i < 100; i++ )
	{
		float R = (float)(rand() % 255) * 0.00392f; // "Same" as divided by 255 but faster
		float G = (float)(rand() % 255) * 0.00392f; //
		float B = (float)(rand() % 255) * 0.00392f;	//
		
		AddBox( XMFLOAT3( 10.0f, 10.0f, 10.0f ),
				XMFLOAT3( 0.0f, 0.0f, (float)(i) * 5.0f ),
				XMFLOAT3( 0.0f , 0.0f, (float)i * 10.0f ),
				XMFLOAT3( R, G, B ) );
	}
	//============================================


				
	//==================LIGHT======================
	mDirection = 1;

	PointLightData light;
	light.positionAndRadius = XMFLOAT4( -40.0f, 20.0f, 0.0f, 350.0f );
	light.ambient			= XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
	light.diffuse			= XMFLOAT4( 0.85f, 0.85f, 1.0f, 1.0f );
	light.specular			= XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	light.attenuation		= XMFLOAT3( 0.0f, 0.02f, 0.0f );

	mPointLightData.push_back( light );
	//=============================================


					
	//==================TEXTURE======================

	//------------------BRICK----------------------
	ID3D11Texture2D* tempTexture = nullptr;

	// COLOR-------------------------------------------------------------------------------------------------------------
	if( FAILED( CreateDDSTextureFromFile( mDevice , L"Textures/Cobble/cobbleTexture_COLOR.DDS", (ID3D11Resource**)&tempTexture, nullptr ) ) )
		return E_FAIL;
	if( FAILED( mDevice->CreateShaderResourceView( tempTexture, nullptr, &mBrickTextureData.colorMap ) ) )
		return E_FAIL;
	// SPECULAR----------------------------------------------------------------------------------------------------------
	if( FAILED( CreateDDSTextureFromFile( mDevice , L"Textures/Cobble/cobbleTexture_SPEC.DDS", (ID3D11Resource**)&tempTexture, nullptr ) ) )
		return E_FAIL;
	if( FAILED( mDevice->CreateShaderResourceView( tempTexture, nullptr, &mBrickTextureData.specularMap ) ) )
		return E_FAIL;
	// NORMAL------------------------------------------------------------------------------------------------------------
	if( FAILED( CreateDDSTextureFromFile( mDevice , L"Textures/Cobble/cobbleTexture_NRM.DDS", (ID3D11Resource**)&tempTexture, nullptr ) ) )
		return E_FAIL;
	if( FAILED( mDevice->CreateShaderResourceView( tempTexture, nullptr, &mBrickTextureData.normalMap ) ) )
		return E_FAIL;
	// DISPLACEMENT------------------------------------------------------------------------------------------------------
	if( FAILED( CreateDDSTextureFromFile( mDevice , L"Textures/Cobble/cobbleTexture2_DISP.DDS", (ID3D11Resource**)&tempTexture, nullptr ) ) )
		return E_FAIL;
	if( FAILED( mDevice->CreateShaderResourceView( tempTexture, nullptr, &mBrickTextureData.displacementMap ) ) )
		return E_FAIL;
	//-------------------------------------------------------------------------------------------------------------------



	ID3D11Texture2D* crateTexture = nullptr;
		
	if( FAILED( CreateDDSTextureFromFile( mDevice , L"Textures/cobble.DDS", (ID3D11Resource**)&crateTexture, nullptr ) ) )
		return E_FAIL;

	D3D11_SAMPLER_DESC saD;
	memset( &saD, 0, sizeof( saD ) );
	saD.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	saD.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	saD.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	saD.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	saD.MipLODBias		= 0.0f;
	saD.MaxAnisotropy	= 1;
	saD.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	saD.MaxLOD			= D3D11_FLOAT32_MAX;

	if( FAILED( mDevice->CreateSamplerState( &saD, &mSamplerState ) ) )
		return E_FAIL;

	if( FAILED( mDevice->CreateShaderResourceView( crateTexture, nullptr, &mShaderResourceView ) ) )
		return E_FAIL;

	SAFE_RELEASE( crateTexture );
	//===============================================



	if( FAILED( CreateVertexBuffer() ) )
		return E_FAIL;

	if( FAILED( CreatePerInstanceBuffer() ) )
		return E_FAIL;
	
	if( FAILED( CreateLightBuffer() ) )
		return E_FAIL;

	return S_OK;
}

Level::Level()
{
	mDevice				= nullptr;
	mDeviceContext		= nullptr;
	mObjectVertexBuffer	= nullptr;
	mInstanceBuffer		= nullptr;
	mLightBuffer		= nullptr;
	mShaderResourceView	= nullptr;
	mSamplerState		= nullptr;
	
	mBox = new GeometryBox( XMFLOAT3( 0.0f, 0.0f, 0.0f ), 2.0f, 2.0f, 2.0f );

	mRotation = 0.0f;
}	

Level::~Level()
{
}

void Level::Release()
{
	//Remove if class does NOT have any
	//resources to release!

	SAFE_RELEASE( mObjectVertexBuffer );
	SAFE_RELEASE( mInstanceBuffer );
	SAFE_RELEASE( mLightBuffer );

	SAFE_RELEASE( mBrickTextureData.colorMap );
	SAFE_RELEASE( mBrickTextureData.specularMap );
	SAFE_RELEASE( mBrickTextureData.normalMap );
	SAFE_RELEASE( mBrickTextureData.displacementMap );

	SAFE_RELEASE( mShaderResourceView );
	SAFE_RELEASE( mSamplerState );	
}