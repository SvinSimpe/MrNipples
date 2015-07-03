#include "Level.h"

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