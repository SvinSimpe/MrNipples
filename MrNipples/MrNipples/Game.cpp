#include "Game.h"

void Game::Update( float deltaTime )
{
}

void Game::Render( float deltaTime )
{
}

HRESULT Game::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;

	return S_OK;
}

Game::Game()
{
	mDevice					= nullptr;
	mDeviceContext			= nullptr;
	mRasterizerStateSolid	= nullptr;
	mRasterizerStateWired	= nullptr;
	mCurrentRasterizerState = nullptr;	
	mInputLayoutBasic		= nullptr;
	mVertexShader			= nullptr;
	mPixelShader			= nullptr;
}

Game::~Game()
{
}

void Game::Release()
{
	SAFE_RELEASE( mRasterizerStateSolid );
	SAFE_RELEASE( mRasterizerStateWired );
	SAFE_RELEASE( mCurrentRasterizerState );
	SAFE_RELEASE( mInputLayoutBasic );
	SAFE_RELEASE( mVertexShader );
	SAFE_RELEASE( mPixelShader );
}