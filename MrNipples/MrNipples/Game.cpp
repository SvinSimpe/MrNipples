#include "Game.h"

void Game::SetRasterizerStateWired( bool isWired )
{
	// Change to WIRED
	if( isWired && mCurrentRasterizerState == mRasterizerStateSolid )
		mCurrentRasterizerState = mRasterizerStateWired;

	// Change to SOLID
	else if( !isWired && mCurrentRasterizerState == mRasterizerStateWired )
		mCurrentRasterizerState = mRasterizerStateSolid;
}

void Game::Update( float deltaTime )
{
}

void Game::Render( float deltaTime )
{
	// Set Rasterizer State
	//mDeviceContext->RSSetState( mCurrentRasterizerState );


		//// Set basic vertex description
		//m_deviceContext->IASetInputLayout( m_inputLayoutBasic );
		//// Set topology
		//m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

		////Set shader stages
		//m_deviceContext->VSSetShader( m_vertexShader, nullptr, 0 );
		//m_deviceContext->HSSetShader( nullptr, nullptr, 0 );
		//m_deviceContext->DSSetShader( nullptr, nullptr, 0 );
		//m_deviceContext->GSSetShader( nullptr, nullptr, 0 );
		//m_deviceContext->PSSetShader( m_pixelShader, nullptr, 0 );


					// Set position vertex description
					//m_deviceContext->IASetInputLayout( m_inputLayoutBasic );
					// Set topology
					//m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST);

					// Set tessellation shader stages
					//m_deviceContext->VSSetShader( m_vertexShader, nullptr, 0 );
					//m_deviceContext->HSSetShader( m_tessHullShader, nullptr, 0 );
					//m_deviceContext->DSSetShader( m_tessDomainShader, nullptr, 0 );
					//m_deviceContext->GSSetShader( nullptr, nullptr, 0 );
					//m_deviceContext->PSSetShader( m_pixelShader, nullptr, 0 );

	/// Set PerFrameCBuffer
	//m_deviceContext->VSSetConstantBuffers( 0, 1, &m_perFrameCBuffer );
	//m_deviceContext->PSSetConstantBuffers( 0, 1, &m_perFrameCBuffer );

	/// Light Buffer
	//m_deviceContext->PSSetConstantBuffers( 2, 1, &m_perFrameLightCBuffer );

	/// RENDER STUFF
	//m_grid->Render( deltaTime );

	// Swap Front and Back Buffer
	//return mSwapChain->Present( 0, 0 );

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