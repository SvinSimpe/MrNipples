#include "Level.h"

HRESULT Level::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth			= sizeof(Vertex32) * NUM_VERTICES_PER_OBJECT;
	vbd.StructureByteStride = sizeof(Vertex32);
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

HRESULT Level::UpdateObjectVertexBuffer()
{
	HRESULT hr = S_OK;
	
	return hr;
}

HRESULT Level::UpdatePerInstanceBuffer()
{
	HRESULT hr = S_OK;
	
	//XMMATRIX scale          = XMMatrixScaling( 5.0f, 5.0f, 5.0f );
	//XMMATRIX rotation       = XMMatrixIdentity();// XMMatrixRotationY( mRotation * 10.0f );
	//XMMATRIX translation    = XMMatrixTranslation( 0.0f, 0.0f, 0.0f );
	//
	//XMMATRIX finalTransform = scale * rotation * translation;
	//
	//XMFLOAT4X4 world;
	//
	//XMStoreFloat4x4( &world, XMMatrixTranspose( finalTransform ) );
	//mBox->PerInstanceData( world );
	int i = 1;
	for each ( PerInstanceData instance in mInstanceData )
	{
		XMStoreFloat4x4( &instance.world,  XMMatrixScaling( 5.0f, 5.0f, 5.0f ) *
															XMMatrixRotationRollPitchYaw( XMConvertToRadians( 0.0f ),
																						  XMConvertToRadians( mRotation * 10.0f ),
																						  XMConvertToRadians( 0.0f ) ) *
															XMMatrixTranslation( 0.0f, 0.0f, (float)(i * 10) ) ) ;
		i++;
	}
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	
	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mInstanceData[0], sizeof(PerInstanceData) * mInstanceData.size() );
	    mDeviceContext->Unmap( mInstanceBuffer, 0 ); 
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

	//XMMATRIX scaleM          = XMMatrixScaling( scale.x, scale.y, scale.z );
	//XMMATRIX rotationM       = XMMatrixIdentity();// XMMatrixRotationY( mRotation * 10.0f );
	//XMMATRIX translationM    = XMMatrixTranslation( translation.x, translation.y, translation.z );
	//
	//XMMATRIX finalTransform = scaleM * rotationM * translationM;
	//XMStoreFloat4x4( &newInstance.world, finalTransform );


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
}

void Level::Render( float deltaTime )
{
	UINT32 stride[2]				= { sizeof(Vertex32), sizeof(PerInstanceData) };
	UINT32 offset[2]				= { 0, 0 };
	ID3D11Buffer* buffersToSet[2]	= { mObjectVertexBuffer, mInstanceBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, stride, offset );

	mDeviceContext->DrawInstanced( NUM_VERTICES_PER_OBJECT, mInstanceData.size(), 0, 0 );
}

HRESULT Level::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;

		///				   TEST
	//========================================
	for ( size_t i = 0; i < 10000; i++ )
	{
		float R = (float)(rand() % 255) / 255.0f;
		float G = (float)(rand() % 255) / 255.0f;
		float B = (float)(rand() % 255) / 255.0f;
		
		AddBox( XMFLOAT3( 5.0f, 5.0f, 5.0f ), XMFLOAT3( 0.0f, (float)(i) * 10.0f, 0.0f ), XMFLOAT3( 0.0f , 0.0f, (float)i * 10.0f ), XMFLOAT3( R, G, B ) );
	}
	//========================================

	if( FAILED( CreateVertexBuffer() ) )
		return E_FAIL;

	if( FAILED( CreatePerInstanceBuffer() ) )
		return E_FAIL;

	return S_OK;
}

Level::Level()
{
	mDevice			= nullptr;
	mDeviceContext	= nullptr;

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
}