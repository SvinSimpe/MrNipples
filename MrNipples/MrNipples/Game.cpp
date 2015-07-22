#include "Game.h"

HRESULT Game::InitializeShaders()
{
	// STANDARD SHADER
	ShaderDesc shaderDesc;
	shaderDesc.shaderFile	= "SimpleShader.hlsl";
	shaderDesc.hasHull		= true;
	shaderDesc.hasDomain	= true;
	mShaders[0]				= new Shader( mDevice, shaderDesc );

	// GEOMETRY PASS SHADER
	shaderDesc.shaderFile	= "GeometryPass.hlsl";
	mShaders[1]				= new Shader( mDevice, shaderDesc );

	// DEPTH SHADER
	shaderDesc.shaderFile	= "DepthShaders.hlsl";
	mShaders[2]				= new Shader( mDevice, shaderDesc );

	// DEFERRED PASS SHADER
	shaderDesc.shaderFile	= "DeferredPass.hlsl";
	shaderDesc.hasHull		= false;
	shaderDesc.hasDomain	= false;
	mShaders[3]				= new Shader( mDevice, shaderDesc );

	return S_OK;
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

HRESULT Game::CreateLightBuffer()
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC lbDesc;
	lbDesc.ByteWidth			= sizeof( PointLightData ) * mPointLightData.size();
	lbDesc.Usage				= D3D11_USAGE_DYNAMIC;
	lbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	lbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	lbDesc.MiscFlags			= 0;
	lbDesc.StructureByteStride	= 0;

	hr = mDevice->CreateBuffer( &lbDesc, nullptr, &mLightBuffer );

	return hr;
}

HRESULT Game::CreateDepthLightCBuffer()
{
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth			= sizeof( DepthLightData );
	cbDesc.Usage				= D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags			= D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags			= 0;
	cbDesc.StructureByteStride	= 0;

	return mDevice->CreateBuffer( &cbDesc, nullptr, &mDepthLightCBuffer );
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

HRESULT Game::UpdateLightBuffer()
{
	HRESULT hr = S_OK;
	
	float zMin = -50.0f;
	float zMax = 300.0f;

	if( mPointLightData.at(0).positionAndRadius.z < zMax && mDirection == 1 )
		mPointLightData.at(0).positionAndRadius.z += 0.005f;

	if( mPointLightData.at(0).positionAndRadius.z > zMax )
		mDirection = 0;

	if( mPointLightData.at(0).positionAndRadius.z > zMin && mDirection == 0 )
		mPointLightData.at(0).positionAndRadius.z -= 0.005f;

	if( mPointLightData.at(0).positionAndRadius.z < zMin )
		mDirection = 1;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	
	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mPointLightData[0], sizeof(PointLightData) * mPointLightData.size() );
	    mDeviceContext->Unmap( mLightBuffer, 0 ); 

		// Set constant buffer to shader stages
		mDeviceContext->VSSetConstantBuffers( 1, 1, &mLightBuffer );
		mDeviceContext->PSSetConstantBuffers( 1, 1, &mLightBuffer );
	}

	return hr;
}

HRESULT Game::UpdateDepthLightCBuffer()
{
	HRESULT hr = S_OK;

	XMFLOAT3 position		= XMFLOAT3( mPointLightData.at(0).positionAndRadius.x, mPointLightData.at(0).positionAndRadius.y, mPointLightData.at(0).positionAndRadius.z );  /*XMFLOAT3( 55.0f, 55.0f, -55.0f );*/
	XMFLOAT3 focusPosition	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 upDirection	= XMFLOAT3( 0.0f, 1.0f, 0.0f );
	XMMATRIX view = XMMatrixLookAtLH(	XMLoadFloat3( &position ),
										XMLoadFloat3( &focusPosition ),
										XMLoadFloat3( &upDirection ) );

	XMMATRIX projection = XMMatrixPerspectiveFovLH( 0.75f, 1280.0f / 720.0f, 0.5f, 5000.0f );

	XMMATRIX world = XMMatrixIdentity();

	XMStoreFloat4x4( &mDepthLightData.worldViewProjection, XMMatrixTranspose( world * view * projection ) );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mDepthLightCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mDepthLightData, sizeof(mDepthLightData) );	
		mDeviceContext->Unmap( mDepthLightCBuffer, 0 );
	}

	return hr;
}

HRESULT Game::RenderStandard( float deltaTime )
{
	// Set Rasterizer State
	mDeviceContext->RSSetState( mCurrentRasterizerState );

	// Set basic vertex description
	mDeviceContext->IASetInputLayout( mShaders[ShaderType::STANDARD]->GetInputLayout() );

	// Set topology
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST );

	//Set shader stages
	mDeviceContext->VSSetShader( mShaders[ShaderType::STANDARD]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( mShaders[ShaderType::STANDARD]->GetHullShader(),	nullptr, 0 );
	mDeviceContext->DSSetShader( mShaders[ShaderType::STANDARD]->GetDomainShader(), nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mShaders[ShaderType::STANDARD]->GetPixelShader(),	nullptr, 0 );

	/// RENDER STUFF
	mLevel->Render( deltaTime );

	return S_OK;
}

HRESULT Game::RenderGeometryPass( float deltaTime )
{
	return S_OK;
}

HRESULT Game::RenderShadowPass( float deltaTime )
{
	return S_OK;
}

HRESULT Game::RenderDeferredPass( float deltaTime )
{
	return S_OK;
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
	if( FAILED( UpdatePerFrameCBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE PER FRAME CONSTANT BUFFER :: Game.cpp" );

	if( FAILED( UpdateLightBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE LIGHT BUFFER :: Game.cpp" );

	if( FAILED( UpdateDepthLightCBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE DEPTH CONSTANT LIGHT BUFFER :: Game.cpp" );

	mLevel->Update( deltaTime );
}

void Game::Render( float deltaTime )
{
	/*
		It is probably a good idea to be able to change between
		rendering techniques with keyboard input. It would be
		desirable to be able to render all render targets to the
		screen simultaneously.
	*/

	RenderStandard( deltaTime );

}

HRESULT Game::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext )
{
	mDevice			= device;
	mDeviceContext	= deviceContext;


	//============================ Create GBuffers ===============================
	mGBuffers = new GBuffer*[NUM_GBUFFERS];
	for (size_t i = 0; i < NUM_GBUFFERS; i++)
		mGBuffers[i] = new GBuffer( device, 1280, 720, DXGI_FORMAT_R32G32B32A32_FLOAT );
	//============================================================================
	

	//=========== Create Game Components ===========
	mLevel			= new Level();
	mCamera			= new Camera();
	mShaders		= new Shader*[ShaderType::NUM_SHADERS];
	//==============================================

	//================== Create Lights ===========================
	mDirection = 1;

	PointLightData light;
	light.positionAndRadius = XMFLOAT4( -40.0f, 20.0f, -50.0f, 350.0f );
	light.ambient			= XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f );
	light.diffuse			= XMFLOAT4( 0.85f, 0.85f, 1.0f, 1.0f );
	light.specular			= XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	light.attenuation		= XMFLOAT3( 0.0f, 0.02f, 0.0f );

	mPointLightData.push_back( light );
	//============================================================

	if( FAILED( InitializeShaders() ) )
		return E_FAIL;


	/*if( FAILED( InitializeBasicShaders() ) )
		return E_FAIL;

	if( FAILED( InitializeDepthShaders() ) )
		return E_FAIL;*/
	
	if( FAILED( CreateRasterizerStates() ) )
		return E_FAIL;

	if( FAILED( CreatePerFrameCBuffer() ) )
		return E_FAIL;

	if( FAILED( CreateLightBuffer() ) )
		return E_FAIL;

	if( FAILED( CreateDepthLightCBuffer() ) )
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

	mGBuffers					= nullptr;

	mLevel		= nullptr;
	mCamera		= nullptr;
	mShaders	= nullptr;
}

Game::~Game()
{
}

void Game::Release()
{
	SAFE_RELEASE( mRasterizerStateSolid );
	SAFE_RELEASE( mRasterizerStateWired );

	for (size_t i = 0; i < NUM_GBUFFERS; i++)
	{
		mGBuffers[i]->Release();
		SAFE_DELETE( mGBuffers[i] );
	}
	SAFE_DELETE_ARRAY( mGBuffers );

	mLevel->Release();
	SAFE_DELETE( mLevel );
	SAFE_DELETE( mCamera );

	for (size_t i = 0; i < ShaderType::NUM_SHADERS; i++)
	{
		mShaders[i]->Relase();
		SAFE_DELETE( mShaders[i] );
	}
	SAFE_DELETE_ARRAY( mShaders );
}