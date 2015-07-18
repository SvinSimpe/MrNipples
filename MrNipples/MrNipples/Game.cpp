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
			D3D11_INPUT_ELEMENT_DESC inputDescInstanced[] = {				 
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "NORMAL",	  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,		 0, 24, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,	 0, 32, D3D11_INPUT_PER_VERTEX_DATA,   0 },
				{ "WORLD",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "WORLD",	  3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
				{ "COLOR",	  0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 64, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
			};

			hr = mDevice->CreateInputLayout( inputDescInstanced,
									     	  ARRAYSIZE( inputDescInstanced ),
											  vs->GetBufferPointer(),
											  vs->GetBufferSize(),
											  &mInputLayoutInstanced );
		}

		vs->Release();
	}

	//----------------------
	// Compile Hull Shader |
	//----------------------
	ID3DBlob* ths = nullptr;

	if( SUCCEEDED( hr = CompileShader( "SimpleShader.hlsl", "HS", "hs_5_0", nullptr, &ths ) ) )
	{
		hr = mDevice->CreateHullShader(  ths->GetBufferPointer(),
										  ths->GetBufferSize(),
										  nullptr,
										  &mHullShader );
		ths->Release();
	}


	//------------------------
	// Compile Domain Shader |
	//------------------------
	ID3DBlob* tds = nullptr;

	if( SUCCEEDED( hr = CompileShader( "SimpleShader.hlsl", "DS", "ds_5_0", nullptr, &tds ) ) )
	{
		hr = mDevice->CreateDomainShader(  tds->GetBufferPointer(),
											tds->GetBufferSize(),
											nullptr,
											&mDomainShader );
		tds->Release();
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
	solidRazDesc.CullMode			= D3D11_CULL_BACK;
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
	wiredRazDesc.CullMode			= D3D11_CULL_BACK;
	wiredRazDesc.DepthClipEnable	= true;


	hr = mDevice->CreateRasterizerState( &wiredRazDesc, &mRasterizerStateWired );

	return hr;
}

HRESULT Game::CreatePerFrameCBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof( mPerFrameData );
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	hr = mDevice->CreateBuffer( &cbDesc, nullptr, &mPerFrameCBuffer );

	return hr;
}

HRESULT Game::UpdatePerFrameCBuffer()
{
HRESULT hr = S_OK;

	XMStoreFloat4x4( &mPerFrameData.view, XMMatrixTranspose( XMLoadFloat4x4( &mCamera->GetViewMatrix() ) ) );
	XMStoreFloat4x4( &mPerFrameData.projection, XMMatrixTranspose( XMLoadFloat4x4( &mCamera->GetProjectionMatrix() ) ) );
	mPerFrameData.eyePosition = mCamera->GetEyePosition();

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mPerFrameCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mPerFrameData, sizeof(mPerFrameData) );	
		mDeviceContext->Unmap( mPerFrameCBuffer, 0 );

		// Set constant buffer to shader stages
		mDeviceContext->VSSetConstantBuffers( 0, 1, &mPerFrameCBuffer );
		mDeviceContext->PSSetConstantBuffers( 0, 1, &mPerFrameCBuffer );
		mDeviceContext->HSSetConstantBuffers( 0, 1, &mPerFrameCBuffer );
		mDeviceContext->DSSetConstantBuffers( 0, 1, &mPerFrameCBuffer );
	}

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
	UpdatePerFrameCBuffer();
	mLevel->Update( deltaTime );
}

void Game::Render( float deltaTime )
{
	// Set Rasterizer State
	mDeviceContext->RSSetState( mCurrentRasterizerState );


	// Set basic vertex description
	mDeviceContext->IASetInputLayout( mInputLayoutInstanced );

	// Set topology
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST );
	//mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);


	//Set shader stages
	mDeviceContext->VSSetShader( mVertexShader, nullptr, 0 );
	mDeviceContext->HSSetShader( mHullShader, nullptr, 0 );
	mDeviceContext->DSSetShader( mDomainShader, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mPixelShader, nullptr, 0 );

	/// RENDER STUFF
	mLevel->Render( deltaTime );
}

HRESULT Game::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;
	mLevel			= new Level();
	mCamera			= new Camera();


	if( FAILED( InitializeBasicShaders() ) )
		return E_FAIL;
	
	if( FAILED( CreateRasterizerStates() ) )
		return E_FAIL;

	if( FAILED( CreatePerFrameCBuffer() ) )
		return E_FAIL;

	if( FAILED( mLevel->Initialize( mDevice, mDeviceContext ) ) )
		return E_FAIL;

	return S_OK;
}

Game::Game()
{
	mDevice						= nullptr;
	mDeviceContext				= nullptr;
	mRasterizerStateSolid		= nullptr;
	mRasterizerStateWired		= nullptr;
	mCurrentRasterizerState		= nullptr;	
	mInputLayoutInstanced		= nullptr;
	mVertexShader				= nullptr;
	mHullShader					= nullptr;
	mDomainShader				= nullptr;
	mPixelShader				= nullptr;

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
	SAFE_RELEASE( mInputLayoutInstanced );
	SAFE_RELEASE( mVertexShader );
	SAFE_RELEASE( mHullShader );
	SAFE_RELEASE( mDomainShader );
	SAFE_RELEASE( mPixelShader );


	mLevel->Release();
	SAFE_DELETE( mLevel );
	SAFE_DELETE( mCamera );
}