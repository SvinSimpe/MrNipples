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
	GeometryBox* box = new GeometryBox( XMFLOAT3( 0.0f, 0.0f, 0.54f ), 1.0f, 1.0f, 1.0f );

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
	ibDesc.ByteWidth			= sizeof( PerInstanceData ) * 2;
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
	
	XMVECTOR normalAxis = XMVectorSet( 0.5f, 1.0f, 0.0f, 0.0f );

	XMMATRIX scale          = XMMatrixScaling( 5.0f, 5.0f, 5.0f );
	//XMMATRIX rotation       = XMMatrixRotationX( mRotation * 10.0f );
	XMMATRIX rotation       = XMMatrixRotationAxis( normalAxis, XMConvertToRadians( 450 * mRotation ) );
	XMMATRIX translation    = XMMatrixTranslation( 0.0f, 0.0f, 0.0f );
	
	XMMATRIX finalTransform = scale * rotation * translation;
	
	XMFLOAT4X4 world;
	
	XMStoreFloat4x4( &world, XMMatrixTranspose( finalTransform ) );
	mBox->PerInstanceData( world );
	
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	
	if( SUCCEEDED( hr ) )
	{
	    memcpy( mappedResource.pData, &mBox->PerInstanceData(), sizeof(PerInstanceData) );
	    mDeviceContext->Unmap( mInstanceBuffer, 0 ); 
	}

	return hr;
}

void Level::AddBox( XMFLOAT3 origin, float width, float height, float depth, float R, float G, float B )
{
	GeometryBox *box = new GeometryBox( origin, width, height, depth );

	/*
		- Store color in PerInstanceData[ID]
		-

		

		XMFLOAT3( R, G, B );

	*/
	
}

void Level::AddGeometry( GeometryBox newBox )
{
	mLevelGeometry.push_back( &newBox );
}

void Level::SetObjectToRender( int ID, bool renderObject )
{
	// Set object as visible
	if( renderObject && !mLevelGeometry[ID]->IsVisible() )
		mLevelGeometry[ID]->IsVisible( true );

	// Set object as NOT visible
	else if( !renderObject && mLevelGeometry[ID]->IsVisible() )
		mLevelGeometry[ID]->IsVisible( false );
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
	//UINT32 vertexSize	= sizeof( Vertex32 );
	//UINT32 offset		= 0;
	//ID3D11Buffer* buffersToSet[] = { mObjectVertexBuffer };
	//mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	//mDeviceContext->Draw( 36, 0 );

	UINT32 stride[2]				= { sizeof(Vertex32), sizeof(PerInstanceData) };
	UINT32 offset[2]				= { 0, 0 };
	ID3D11Buffer* buffersToSet[2]	= { mObjectVertexBuffer, mInstanceBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, stride, offset );

	mDeviceContext->DrawInstanced( NUM_VERTICES_PER_OBJECT, 1, 0, 0 );
}

HRESULT Level::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;

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