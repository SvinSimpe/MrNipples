#include "Level.h"

void Level::AddGeometry( GeometryBox newBox )
{
	mLevelGeometry.push_back( newBox );
}

void Level::SetObjectToRender( int ID, bool renderObject )
{
	// Set object as visible
	if( renderObject && !mLevelGeometry[ID].IsVisible() )
		mLevelGeometry[ID].IsVisible( true );

	// Set object as NOT visible
	else if( !renderObject && mLevelGeometry[ID].IsVisible() )
		mLevelGeometry[ID].IsVisible( false );
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
}

void Level::Render( float deltaTime )
{


}

HRESULT Level::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;

	return S_OK;
}

Level::Level()
{
	mDevice			= nullptr;
	mDeviceContext	= nullptr;
}	

Level::~Level()
{
}

void Level::Release()
{
	//Remove if class does NOT have any
	//resources to release!
}