#include "Game.h"

HRESULT Game::CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader )
{	
	HRESULT hr = S_OK;

	DWORD dwShaderFlags =	D3DCOMPILE_ENABLE_STRICTNESS | 
							D3DCOMPILE_IEEE_STRICTNESS;

	std::string shader_code;
	std::ifstream in( shaderFile, std::ios::in | std::ios::binary );

	if ( in )
	{
		in.seekg( 0, std::ios::end );
		shader_code.resize( (unsigned int)in.tellg() );
		in.seekg( 0, std::ios::beg );
		in.read( &shader_code[0], shader_code.size() );
		in.close();
	}

	ID3DBlob* pErrorBlob = nullptr;
	hr = D3DCompile( shader_code.data(),
							 shader_code.size(),
							 NULL,
							 pDefines,
							 nullptr,
							 pEntrypoint,
							 pTarget,
							 dwShaderFlags,
							 NULL,
							 pCompiledShader,
							 &pErrorBlob );

	if( pErrorBlob )
	{
		OutputDebugStringA( (char*)pErrorBlob->GetBufferPointer() );
		pErrorBlob->Release();
	}


	return hr;
}

HRESULT Game::InitializeBasicShaders()
{
	HRESULT hr = S_OK;

	//------------------------
	// Compile Vertex Shader |
	//------------------------
	ID3DBlob* vs = nullptr;
	
	if ( SUCCEEDED( hr = CompileShader( "SimpleShader.hlsl", "VS", "vs_5_0", nullptr, &vs ) ) )
	{
		if( SUCCEEDED( hr = mDevice->CreateVertexShader( vs->GetBufferPointer(),
														  vs->GetBufferSize(),
														  nullptr,
														  &mVertexShader ) ) )
		{
			D3D11_INPUT_ELEMENT_DESC inputDescBasic[] = {				 
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			hr = mDevice->CreateInputLayout( inputDescBasic,
									     	  ARRAYSIZE( inputDescBasic ),
											  vs->GetBufferPointer(),
											  vs->GetBufferSize(),
											  &mInputLayoutBasic );
		}

		vs->Release();
	}



	//-----------------------
	// Compile Pixel Shader |
	//-----------------------
	ID3DBlob* tps = nullptr;

	if( SUCCEEDED( hr = CompileShader( "SimpleShader.hlsl", "PS", "ps_5_0", nullptr, &tps ) ) )
	{
		hr = mDevice->CreatePixelShader( tps->GetBufferPointer(),
										  tps->GetBufferSize(),
										  nullptr,
										  &mPixelShader );
		tps->Release();
	}

	return hr;
}

HRESULT Game::CreateRasterizerStates()
{
	HRESULT hr = S_OK;
	//------------------------------
	// Create Solid Rasterizer State
	//------------------------------
	D3D11_RASTERIZER_DESC solidRazDesc;
	memset( &solidRazDesc, 0, sizeof( solidRazDesc ) );
	solidRazDesc.FillMode			= D3D11_FILL_SOLID;
	solidRazDesc.CullMode			= D3D11_CULL_NONE;
	solidRazDesc.DepthClipEnable	= true;

	hr = mDevice->CreateRasterizerState( &solidRazDesc, &mRasterizerStateSolid );

	// Set current rasterizerstate to solid
	mCurrentRasterizerState = mRasterizerStateSolid;

	//----------------------------------
	// Create Wireframe Rasterizer State
	//----------------------------------
	D3D11_RASTERIZER_DESC wiredRazDesc;
	memset( &wiredRazDesc, 0, sizeof( wiredRazDesc ) );
	wiredRazDesc.FillMode			= D3D11_FILL_WIREFRAME;
	wiredRazDesc.CullMode			= D3D11_CULL_NONE;
	wiredRazDesc.DepthClipEnable	= true;

	hr = mDevice->CreateRasterizerState( &wiredRazDesc, &mRasterizerStateWired );

	return hr;
}

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

	/*if( FAILED( InitializeBasicShaders() ) )
		return E_FAIL;*/
	
	if( FAILED( CreateRasterizerStates() ) )
		return E_FAIL;

	// Just for testing!
	GeometryBox *testBox = new GeometryBox( XMFLOAT3( 0.0f, 0.0f, 0.0f ), 5.0f, 5.0f, 5.0f );


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

	mLevel	= nullptr;
	mCamera	= nullptr;
}

Game::~Game()
{
}

void Game::Release()
{
	SAFE_RELEASE( mRasterizerStateSolid );
	SAFE_RELEASE( mRasterizerStateWired );
	SAFE_RELEASE( mInputLayoutBasic );
	SAFE_RELEASE( mVertexShader );
	SAFE_RELEASE( mPixelShader );


	mLevel->Release();
	SAFE_DELETE( mLevel );
	SAFE_DELETE( mCamera );
}
