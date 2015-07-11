#include "Level.h"

HRESULT Level::CreateVertexBuffer( unsigned int nrOfObjects )
{
	HRESULT hr = S_OK;

	if( nrOfObjects > MAX_NUM_OF_OBJECTS )
		nrOfObjects = MAX_NUM_OF_OBJECTS;

	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth			= sizeof(Vertex32) * NUM_VERTICES_PER_OBJECT * nrOfObjects;
	vbd.StructureByteStride = sizeof(Vertex32);
	vbd.Usage				= D3D11_USAGE_DYNAMIC;
	vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags			= 0;
	

	///TEST
	GeometryBox* box = new GeometryBox( XMFLOAT3( 0.0f, 0.0f, 0.0f ), 2.0f, 2.0f, 2.0f );

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.SysMemPitch		= 0;
	vinitData.SysMemSlicePitch	= 0;
	vinitData.pSysMem			= &box->VertexFaces()[0];

	hr = mDevice->CreateBuffer(&vbd, &vinitData, &mObjectVertexBuffer);
	
	return hr;
}

HRESULT Level::CreatePerObjectCBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth			= sizeof( PerObjectData );
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags			= 0;
	cbDesc.StructureByteStride	= 0;

	hr = mDevice->CreateBuffer( &cbDesc, nullptr, &mPerObjectCBuffer );

	return hr;
}

HRESULT Level::UpdateObjectVertexBuffer()
{
	HRESULT hr = S_OK;
	
	return hr;
}

HRESULT Level::UpdatePerObjectCBuffer()
{
	HRESULT hr = S_OK;
	
	XMFLOAT4X4 world;

	XMStoreFloat4x4( &world, XMMatrixTranspose( XMMatrixRotationY( mRotation * 10.0f ) ) );
	mBox->PerObjectData( world );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mPerObjectCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if(SUCCEEDED( hr ))
	{
		memcpy( mappedResource.pData, &mBox->PerObjectData(), sizeof(PerObjectData) );
		/*memcpy( mappedResource.pData, &m_perObjectData, sizeof(m_perObjectData) );*/	
		mDeviceContext->Unmap( mPerObjectCBuffer, 0 );

		// Set constant buffer to shader stages
		mDeviceContext->VSSetConstantBuffers( 1, 1, &mPerObjectCBuffer );
		mDeviceContext->PSSetConstantBuffers( 1, 1, &mPerObjectCBuffer );	
	}

	return hr;
}

void Level::AddBox( XMFLOAT3 origin, float width, float height, float depth, float R, float G, float B )
{
	GeometryBox *box = new GeometryBox( origin, width, height, depth );

	/*
		- Store color in PerObjectData[ID]
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

	if( FAILED( UpdatePerObjectCBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE PER OBJECT CONSTANT BUFFER :: Level.cpp" );
}

void Level::Render( float deltaTime )
{
	UINT32 vertexSize	= sizeof( Vertex32 );
	UINT32 offset		= 0;
	ID3D11Buffer* buffersToSet[] = { mObjectVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, &vertexSize, &offset );

	mDeviceContext->Draw( 36, 0 );
}

HRESULT Level::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;

	if( FAILED( CreateVertexBuffer( 5 ) ) )
		return E_FAIL;

	if( FAILED( CreatePerObjectCBuffer() ) )
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