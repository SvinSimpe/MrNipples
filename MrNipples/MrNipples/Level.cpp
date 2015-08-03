#include "Level.h"

HRESULT Level::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vbd;
	vbd.ByteWidth			= sizeof(Vertex48) * ( BOX_VERTEX_COUNT + PLANE_VERTEX_COUNT );
	/*vbd.ByteWidth			= sizeof(Vertex48) * NUM_VERTICES_PER_OBJECT;*/
	vbd.StructureByteStride = sizeof(Vertex48);
	vbd.Usage				= D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags		= 0;
	vbd.MiscFlags			= 0;
	


	Vertex48* data = new Vertex48[BOX_VERTEX_COUNT + PLANE_VERTEX_COUNT];

	for (size_t i = 0; i < BOX_VERTEX_COUNT; i++)
	{
		data[i] = mBox->VertexFaces()[i];
	}
	for (size_t i = BOX_VERTEX_COUNT; i < BOX_VERTEX_COUNT + PLANE_VERTEX_COUNT; i++)
	{
		data[i] = mPlane->VertexFaces()[i-BOX_VERTEX_COUNT];
	}

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.SysMemPitch		= 0;
	vinitData.SysMemSlicePitch	= 0;
	vinitData.pSysMem			= &data[0];

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
	
	// Spinderella
	//for (size_t i = 0; i < mInstanceData.size(); i++)
	//{
	//	// Construct World matrix
	//	XMStoreFloat4x4( &mInstanceData.at(i).world,  XMMatrixScaling( 5.0f, 5.0f, 5.0f ) *
	//									   XMMatrixRotationRollPitchYaw( XMConvertToRadians( 0.0f ),
	//																	 XMConvertToRadians( mRotation * ( (float)i * 40 ) ),
	//																	 XMConvertToRadians( (float)i * 10.0f ) ) *
	//									   XMMatrixTranslation( 0.0f, 0.0f, (float)i * 17.0f ) ) ;
	//}


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	
	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mInstanceData[0], sizeof(PerInstanceData) * mInstanceData.size() );
	    mDeviceContext->Unmap( mInstanceBuffer, 0 ); 
	}

	return hr;
}

HRESULT Level::RenderOpaque( float deltaTime )
{


	return S_OK;
}

HRESULT Level::RenderTransparent( float deltaTime )
{



	return S_OK;
}

void Level::AddGeometry( XMFLOAT3 scale, XMFLOAT3 rotation, XMFLOAT3 translation, XMFLOAT3 color )
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
}

void Level::Render( float deltaTime )
{
	/*
		Should probably add RenderTransparent and RenderOpaque functions
		that utilizes Forward rendering and Deferred rendering on different
		sets of objects in the scene.

		Each object can store a float ranging from 0.0 to 1.0 representing
		the opacity of the object. Perhaps implementing function for batching
		all objects with 1.0 to one container and the rest to another.
		
		Some optimization can be done in terms of sensitivity towards this
		floating value. Objects that has values from 0.9x up to 1.0 may be
		perceived as opaque and will be rendered as such. This MAY result in
		a small performance increase *fingers crossed*.
	*/

	UINT32 stride[2]				= { sizeof(Vertex48), sizeof(PerInstanceData) };
	UINT32 offset[2]				= { 0, 0 };
	ID3D11Buffer* buffersToSet[2]	= { mObjectVertexBuffer, mInstanceBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 2, buffersToSet, stride, offset );

	
	mDeviceContext->DrawInstanced( BOX_VERTEX_COUNT, mNumGeometryPerType[GeometryType::BOX], 0, 0 );

	mDeviceContext->DrawInstanced( PLANE_VERTEX_COUNT, mNumGeometryPerType[GeometryType::PLANE],
													   BOX_VERTEX_COUNT,
													   mNumGeometryPerType[GeometryType::BOX] );
}

HRESULT Level::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;

	mBox	= new GeometryBox( XMFLOAT3( 0.0f, 0.0f, 0.0f ), 2.0f, 2.0f, 2.0f );
	mPlane	= new GeometryPlane( XMFLOAT3( 0.0f, 0.0f, 0.0f ), 40.0f, 40.0f );

	mNumGeometryPerType = new unsigned int[GeometryType::NUM_GEOMETRY_TYPES];
	mNumGeometryPerType[GeometryType::BOX]		= 0;
	mNumGeometryPerType[GeometryType::PLANE]	= 0;
					
	//=================GEOMETRY BOX===================
	//for ( size_t i = 0; i < 100; i++ )
	//{
	//	float R = (float)(rand() % 255) * 0.00392f; // "Same" as divided by 255 but faster
	//	float G = (float)(rand() % 255) * 0.00392f; //
	//	float B = (float)(rand() % 255) * 0.00392f;	//
	//	
	//	AddGeometry( XMFLOAT3( 5.0f, 5.0f, 5.0f ),
	//				 XMFLOAT3( 0.0f, 0.0f, (float)(i) * 5.0f ),
	//				 XMFLOAT3( 0.0f , 0.0f, (float)i * 10.0f ),
	//				 XMFLOAT3( R, G, B ) );
	//	
	//	mNumGeometryPerType[GeometryType::BOX]++;
	//}


				// GIANT TEST

				for ( size_t i = 0; i < 8; i++ )
				{
					float R = (float)(rand() % 255) * 0.00392f; // "Same" as divided by 255 but faster
					float G = (float)(rand() % 255) * 0.00392f; //
					float B = (float)(rand() % 255) * 0.00392f;	//
					
					AddGeometry( XMFLOAT3( 5.0f, 5.0f, 5.0f ),
								 XMFLOAT3( 0.0f, 0.0f, 0.0f ),
								 XMFLOAT3( -40.0f , -40.0f, -150.0f + (float)i * 50.0f ),
								 XMFLOAT3( R, G, B ) );
					
					mNumGeometryPerType[GeometryType::BOX]++;
				}


				for ( size_t i = 0; i < 8; i++ )
				{
					float R = (float)(rand() % 255) * 0.00392f; // "Same" as divided by 255 but faster
					float G = (float)(rand() % 255) * 0.00392f; //
					float B = (float)(rand() % 255) * 0.00392f;	//
					
					AddGeometry( XMFLOAT3( 5.0f, 5.0f, 5.0f ),
								 XMFLOAT3( 0.0f, 0.0f, 0.0f ),
								 XMFLOAT3( 40.0f , -40.0f, -150.0f + (float)i * 50.0f ),
								 XMFLOAT3( R, G, B ) );
					
					mNumGeometryPerType[GeometryType::BOX]++;
				}
				////


	//================================================

	//=================GEOMETRY PLANE===================
	for ( size_t i = 0; i < 1; i++ )
	{
		float R = (float)(rand() % 255) * 0.00392f; // "Same" as divided by 255 but faster
		float G = (float)(rand() % 255) * 0.00392f; //
		float B = (float)(rand() % 255) * 0.00392f;	//
		
		AddGeometry( XMFLOAT3( 40.0f, 40.0f,  40.0f ),
					 XMFLOAT3(  0.0f,  0.0f,  0.0f ),
					 XMFLOAT3(  0.0f, -50.0f, 10.0f ),
					 XMFLOAT3( R, G, B ) );
		
		mNumGeometryPerType[GeometryType::PLANE]++;
	}
	//================================================


	if( FAILED( CreateVertexBuffer() ) )
		return E_FAIL;

	if( FAILED( CreatePerInstanceBuffer() ) )
		return E_FAIL;

	return S_OK;
}

Level::Level()
{
	mDevice				= nullptr;
	mDeviceContext		= nullptr;
	mObjectVertexBuffer	= nullptr;
	mInstanceBuffer		= nullptr;
	mSamplerState		= nullptr;
	
	mBox				= nullptr;
	mPlane				= nullptr;
	
	mRotation			= 0.0f;

	mNumGeometryPerType = nullptr;
}	

Level::~Level()
{
}

void Level::Release()
{
	SAFE_RELEASE( mObjectVertexBuffer );
	SAFE_RELEASE( mInstanceBuffer );
	SAFE_RELEASE( mSamplerState );	

	mBox->Release();
	mPlane->Release();
	SAFE_DELETE( mBox );
	SAFE_DELETE( mPlane );
}