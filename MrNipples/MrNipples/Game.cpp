#include "Game.h"

HRESULT Game::CreateDepthStencilView()
{
	HRESULT hr = S_OK;

	////////////////////////////////////////////////////
	//			Create depth stencil view				
	////////////////////////////////////////////////////
	ID3D11Texture2D*	depthStencil;
	D3D11_TEXTURE2D_DESC depthDesc;
	depthDesc.Width					= 1280;
	depthDesc.Height				= 720;
	depthDesc.MipLevels				= 1;
	depthDesc.ArraySize				= 1;
	depthDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count		= 1;
	depthDesc.SampleDesc.Quality	= 0;
	depthDesc.Usage					= D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags				= D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags		= 0;
	depthDesc.MiscFlags				= 0;

	if( FAILED( hr = mDevice->CreateTexture2D( &depthDesc, NULL, &depthStencil ) ) )
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof( dsvd ) );
	dsvd.Format				= depthDesc.Format;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice = 0;

	if( FAILED( hr = mDevice->CreateDepthStencilView( depthStencil, &dsvd, &mDepthStencilView ) ) )
	{
		SAFE_RELEASE( depthStencil );
		return hr;
	}

	SAFE_RELEASE( depthStencil );

	return hr;
}

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
	shaderDesc.hasPixel		= true;
	mShaders[2]				= new Shader( mDevice, shaderDesc );

	// DEFERRED PASS SHADER
	shaderDesc.shaderFile		= "DeferredPass.hlsl";
	shaderDesc.hasHull			= false;
	shaderDesc.hasDomain		= false;
	shaderDesc.hasPixel			= true;
	shaderDesc.inputLayoutType	= InputLayoutType::VERTEX32;
	mShaders[3]					= new Shader( mDevice, shaderDesc );

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

HRESULT Game::CreateSamplerState()
{
	D3D11_SAMPLER_DESC saD;
	memset( &saD, 0, sizeof( saD ) );
	saD.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	saD.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	saD.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	saD.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	saD.MipLODBias		= 0.0f;
	saD.MaxAnisotropy	= 1;
	saD.ComparisonFunc	= D3D11_COMPARISON_NEVER;
	saD.MaxLOD			= D3D11_FLOAT32_MAX;

	if( FAILED( mDevice->CreateSamplerState( &saD, &mSamplerState ) ) )
		return E_FAIL;

	return S_OK;
}

HRESULT Game::CreateQuadVertexBuffer()
{
	////////////////////////////////////////////////////
	//			Create Vertex Buffer for quad					
	////////////////////////////////////////////////////
	Vertex32 vertices[4];
	
	vertices[0].position = XMFLOAT3( -1.0f,  1.0f, 0.0f );	vertices[0].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		vertices[0].texCoord = XMFLOAT2( 0.0f, 0.0f );
	vertices[1].position = XMFLOAT3(  1.0f,  1.0f, 0.0f );	vertices[1].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		vertices[1].texCoord = XMFLOAT2( 1.0f, 0.0f );
	vertices[2].position = XMFLOAT3( -1.0f, -1.0f, 0.0f );	vertices[2].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		vertices[2].texCoord = XMFLOAT2( 0.0f, 1.0f );
	vertices[3].position = XMFLOAT3(  1.0f, -1.0f, 0.0f );	vertices[3].normal = XMFLOAT3( 0.0f, 0.0f, -1.0f );		vertices[3].texCoord = XMFLOAT2( 1.0f, 1.0f );

	D3D11_BUFFER_DESC vbd;
	ZeroMemory( &vbd, sizeof( vbd ) );
	vbd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	vbd.Usage			= D3D11_USAGE_DEFAULT;
	vbd.ByteWidth		= sizeof( vertices );

	D3D11_SUBRESOURCE_DATA subData;
	subData.pSysMem		= vertices;

	return mDevice->CreateBuffer( &vbd, &subData, &mQuadVertexBuffer );
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
	
	// Set camera position and tesselation amount from keyboard input
	mPerFrameData.eyePositionAndTessAmount = XMFLOAT4( mCamera->GetEyePosition().x,
													   mCamera->GetEyePosition().y,
													   mCamera->GetEyePosition().z,
													   mTesselationAmount );

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
	{
		XMStoreFloat4x4( &mPointLightData.at(0).world, XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation( 0.0f, 0.0f, 0.005f ) );

		XMVECTOR oldLightPosition = XMVectorSet( mPointLightData.at(0).positionAndRadius.x,
												 mPointLightData.at(0).positionAndRadius.y,
												 mPointLightData.at(0).positionAndRadius.z, 0.0f );

		XMVECTOR newLightPosition =  XMVector3TransformCoord( oldLightPosition, XMLoadFloat4x4( &mPointLightData.at(0).world ) );

		float radius = mPointLightData.at(0).positionAndRadius.w;

		XMStoreFloat4( &mPointLightData.at(0).positionAndRadius, newLightPosition );

		mPointLightData.at(0).positionAndRadius.w = radius;
	}

	if( mPointLightData.at(0).positionAndRadius.z > zMax )
		mDirection = 0;

	if( mPointLightData.at(0).positionAndRadius.z > zMin && mDirection == 0 )
	{
		XMStoreFloat4x4( &mPointLightData.at(0).world, XMMatrixIdentity() * XMMatrixIdentity() * XMMatrixTranslation( 0.0f, 0.0f, -0.005f ) );
	
		XMVECTOR oldLightPosition = XMVectorSet( mPointLightData.at(0).positionAndRadius.x,
												 mPointLightData.at(0).positionAndRadius.y,
												 mPointLightData.at(0).positionAndRadius.z, 0.0f );

		XMVECTOR newLightPosition =  XMVector3TransformCoord( oldLightPosition, XMLoadFloat4x4( &mPointLightData.at(0).world ) );

		float radius = mPointLightData.at(0).positionAndRadius.w;

		XMStoreFloat4( &mPointLightData.at(0).positionAndRadius, newLightPosition );

		mPointLightData.at(0).positionAndRadius.w = radius;
	}
		

	if( mPointLightData.at(0).positionAndRadius.z < zMin )
		mDirection = 1;



	mPointLightData.at(0).ambient			= XMFLOAT4( 2.0f, 6.0f, 8.0f, 1.0f );
	mPointLightData.at(0).diffuse			= XMFLOAT4( 0.85f, 0.85f, 1.0f, 1.0f );
	mPointLightData.at(0).specular			= XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	mPointLightData.at(0).attenuation		= XMFLOAT3( 0.0f, 0.02f, 0.0f );

	


	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mLightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );
	
	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mPointLightData[0], sizeof(PointLightData) * mPointLightData.size() );
	    mDeviceContext->Unmap( mLightBuffer, 0 ); 
	}

	return hr;
}

HRESULT Game::UpdateDepthLightCBuffer()
{
	HRESULT hr = S_OK;

	XMFLOAT3 position		= XMFLOAT3( mPointLightData.at(0).positionAndRadius.x, mPointLightData.at(0).positionAndRadius.y, mPointLightData.at(0).positionAndRadius.z );
	XMFLOAT3 focusPosition	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	XMFLOAT3 upDirection	= XMFLOAT3( 0.0f, 1.0f, 0.0f );
	XMMATRIX view = XMMatrixLookAtLH(	XMLoadFloat3( &position ),
										XMLoadFloat3( &focusPosition ),
										XMLoadFloat3( &upDirection ) );

	XMMATRIX projection = XMMatrixPerspectiveFovLH( 0.75f, 1280.0f / 720.0f, 0.5f, 5000.0f );

	XMMATRIX world = XMMatrixIdentity();// XMLoadFloat4x4( &mPointLightData.at(0).world );

	XMStoreFloat4x4( &mDepthLightData.worldViewProjection, XMMatrixTranspose( world * view * projection ) );

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	hr = mDeviceContext->Map( mDepthLightCBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource );

	if( SUCCEEDED( hr ) )
	{
		memcpy( mappedResource.pData, &mDepthLightData, sizeof(mDepthLightData) );	
		mDeviceContext->Unmap( mDepthLightCBuffer, 0 );

		mDeviceContext->DSSetConstantBuffers( 1, 1, &mDepthLightCBuffer );
	}

	return hr;
}

HRESULT Game::RenderStandard( float deltaTime )
{
	RenderShadowPass( deltaTime );


	mDeviceContext->ClearRenderTargetView( mRenderTargetView, CLEAR_COLOR_BLACK );

	// Clear Depth Buffer
	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Set Render target
	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

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

	// Update Light Buffer
	if( FAILED( UpdateLightBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE LIGHT BUFFER :: Game.cpp" );


	// Set constant buffer to shader stages
	mDeviceContext->VSSetConstantBuffers( 1, 1, &mLightBuffer );
	mDeviceContext->PSSetConstantBuffers( 1, 1, &mLightBuffer );

	// Set Rasterizer State
	mDeviceContext->RSSetState( mCurrentRasterizerState );

	// Set textures
	mAssetManager->SetTexture( TextureType::COBBLE );

	// Set Sampler State
	mDeviceContext->DSSetSamplers( 0, 1, &mSamplerState );
	mDeviceContext->PSSetSamplers( 0, 1, &mSamplerState );

	//TEST
	ID3D11ShaderResourceView* shadowView = mShadowMap->ShaderResourceView();
	mDeviceContext->PSSetShaderResources( 4, 1, &shadowView );

	// Render Level
	mLevel->Render( deltaTime );

	return S_OK;
}

HRESULT Game::RenderGeometryPass( float deltaTime )
{
	mDeviceContext->RSSetState( mCurrentRasterizerState );

	// Clear GBuffer Render Target Views
	for( int i = 0; i < NUM_GBUFFERS; i++ )
		mDeviceContext->ClearRenderTargetView( mGBuffers[i]->RenderTargetView(), CLEAR_COLOR_RED );

	// Clear Depth Stencil View
	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	// Set Render Targets
	ID3D11RenderTargetView* renderTargetsToSet[] = { 
														mGBuffers[0]->RenderTargetView(), 
														mGBuffers[1]->RenderTargetView(), 
														mGBuffers[2]->RenderTargetView(),
														mGBuffers[3]->RenderTargetView()
													};
	mDeviceContext->OMSetRenderTargets( NUM_GBUFFERS, renderTargetsToSet, mDepthStencilView );

	// Set basic vertex description
	mDeviceContext->IASetInputLayout( mShaders[ShaderType::GEOMETRY_PASS]->GetInputLayout() );

	// Set topology
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST );

	//Set shader stages
	mDeviceContext->VSSetShader( mShaders[ShaderType::GEOMETRY_PASS]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( mShaders[ShaderType::GEOMETRY_PASS]->GetHullShader(),	nullptr, 0 );
	mDeviceContext->DSSetShader( mShaders[ShaderType::GEOMETRY_PASS]->GetDomainShader(), nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mShaders[ShaderType::GEOMETRY_PASS]->GetPixelShader(),	nullptr, 0 );

	// Set textures
	mAssetManager->SetTexture( TextureType::COBBLE );

	// Set Sampler State
	mDeviceContext->DSSetSamplers( 0, 1, &mSamplerState );
	mDeviceContext->PSSetSamplers( 0, 1, &mSamplerState );

	// RENDER STUFF
	mLevel->Render( deltaTime );
	
	return S_OK;
}

HRESULT Game::RenderShadowPass( float deltaTime )
{
	// Change to Shadow Map Viewport & disable color writes
	mShadowMap->BindDsvAndSetNullRenderTarget( mDeviceContext );

	// Set Rasterizer State
	//mDeviceContext->RSSetState( mCurrentRasterizerState );

	// Set topology
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_3_CONTROL_POINT_PATCHLIST );
	
	// Set vertex description
	mDeviceContext->IASetInputLayout( mShaders[ShaderType::DEPTH]->GetInputLayout() );

	

	// Set depth shader stages
	mDeviceContext->VSSetShader( mShaders[ShaderType::DEPTH]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( mShaders[ShaderType::DEPTH]->GetHullShader(), nullptr, 0 );
	mDeviceContext->DSSetShader( mShaders[ShaderType::DEPTH]->GetDomainShader(), nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( nullptr, nullptr, 0 );

	// Set textures
	mDeviceContext->DSSetShaderResources( 0, 1, &mAssetManager->GetTexture( TextureType::COBBLE )->displacementMap );

	// Set Sampler State
	mDeviceContext->DSSetSamplers( 0, 1, &mSamplerState );

	// Depth Light Buffer
	UpdateDepthLightCBuffer();

	mLevel->Render( deltaTime );

	return S_OK;
}

HRESULT Game::RenderDeferredPass( float deltaTime )
{
	if( mCurrentRasterizerState == mRasterizerStateWired )
		mCurrentRasterizerState = mRasterizerStateSolid;

	// Clear back buffer
	mDeviceContext->ClearRenderTargetView( mRenderTargetView, CLEAR_COLOR_GREEN );

	// Set rendertarget
	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

	// Set topology
	mDeviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP );

	// Set inputlayout
	mDeviceContext->IASetInputLayout( mShaders[ShaderType::DEFERRED_PASS]->GetInputLayout() );

	// Set shaders
	mDeviceContext->VSSetShader( mShaders[ShaderType::DEFERRED_PASS]->GetVertexShader(), nullptr, 0 );
	mDeviceContext->HSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->DSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->GSSetShader( nullptr, nullptr, 0 );
	mDeviceContext->PSSetShader( mShaders[ShaderType::DEFERRED_PASS]->GetPixelShader(), nullptr, 0 );

	// Set sampler state
	mDeviceContext->PSSetSamplers( 0, 1, &mSamplerState );


	ID3D11ShaderResourceView* viewsToSet[] = { mGBuffers[0]->mShaderResourceView,
											   mGBuffers[1]->mShaderResourceView,
											   mGBuffers[2]->mShaderResourceView,
											   mGBuffers[3]->mShaderResourceView,
											   mShadowMap->ShaderResourceView() };


	mDeviceContext->PSSetShaderResources( 0, 5, viewsToSet );

	// Update Light Buffer
	if( FAILED( UpdateLightBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE LIGHT BUFFER :: Game.cpp" );

	if( FAILED( UpdateDepthLightCBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE DEPTH LIGHT CONSTANT BUFFER :: Game.cpp" );

	mDeviceContext->PSSetConstantBuffers( 1, 1, &mLightBuffer );
	mDeviceContext->PSSetConstantBuffers( 2, 1, &mDepthLightCBuffer );

	UINT32 stride[]				= { sizeof(Vertex32) };
	UINT32 offset[]				= { 0 };
	ID3D11Buffer* buffersToSet[]	= { mQuadVertexBuffer };
	mDeviceContext->IASetVertexBuffers( 0, 1, buffersToSet, stride, offset );

	mDeviceContext->Draw( 4, 0 );
	///==============================================================================================

	// TEST! Just to remove warning
	ID3D11ShaderResourceView* pSRV[] = {nullptr};
	mDeviceContext->PSSetShaderResources(0, 1, pSRV);



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

void Game::SetTesselationAmount( float tesselationAmount )
{
	mTesselationAmount = tesselationAmount;
}

void Game::Update( float deltaTime )
{
	if( FAILED( UpdatePerFrameCBuffer() ) )
		OutputDebugStringA( "FAILED TO UPDATE PER FRAME CONSTANT BUFFER :: Game.cpp" );

	//if( FAILED( UpdateLightBuffer() ) )
	//	OutputDebugStringA( "FAILED TO UPDATE LIGHT BUFFER :: Game.cpp" );

	//if( FAILED( UpdateDepthLightCBuffer() ) )
	//	OutputDebugStringA( "FAILED TO UPDATE DEPTH CONSTANT LIGHT BUFFER :: Game.cpp" );

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
	//// Set Rasterizer State
	//mDeviceContext->RSSetState( mCurrentRasterizerState );

	//// Set textures
	//mAssetManager->SetTexture( TextureType::COBBLE );

	//// Set Sampler State
	//mDeviceContext->DSSetSamplers( 0, 1, &mSamplerState );
	//mDeviceContext->PSSetSamplers( 0, 1, &mSamplerState );

	//RenderGeometryPass( deltaTime );
	//RenderShadowPass( deltaTime );
	//RenderDeferredPass( deltaTime );
	//RenderStandard( deltaTime );

}

HRESULT Game::Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView )
{
	mDevice				= device;
	mDeviceContext		= deviceContext;
	mRenderTargetView	= renderTargetView;
	mAssetManager		= new AssetManager();
	mShadowMap			= new ShadowMap( device, 1280, 720 );
	mTesselationAmount	= 1.0f;

	//============================ Create GBuffers ===============================
	mGBuffers = new GBuffer*[NUM_GBUFFERS];													 
	for (size_t i = 0; i < NUM_GBUFFERS; i++)												// RT0 = Position	RT1 = Normal
		mGBuffers[i] = new GBuffer( device, 1280, 720, DXGI_FORMAT_R16G16B16A16_FLOAT );	// RT2 = Diffuse	RT3 = Specular
	//============================================================================
	



	//=========== Create Game Components ===========
	mLevel			= new Level();
	mCamera			= new Camera();
	mShaders		= new Shader*[ShaderType::NUM_SHADERS];
	//==============================================

	//================== Create Lights ===========================
	mDirection = 1;

	PointLightData light;
	light.positionAndRadius = XMFLOAT4( -40.0f, 20.0f, 50.0f, 350.0f );
	light.ambient			= XMFLOAT4( 2.0f, 6.0f, 8.0f, 1.0f );
	light.diffuse			= XMFLOAT4( 0.85f, 0.85f, 1.0f, 1.0f );
	light.specular			= XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	light.attenuation		= XMFLOAT3( 0.0f, 0.02f, 0.0f );

	XMStoreFloat4x4( &light.world, XMMatrixIdentity() );

	mPointLightData.push_back( light );
	////---------------------------------------------------------------------------------------------

	//PointLightData light2;
	//light2.positionAndRadius	= XMFLOAT4( 80.0f, 10.0f, 100.0f, 350.0f );
	//light2.ambient				= XMFLOAT4( 2.0f, 8.0f, 6.0f, 1.0f );
	//light2.diffuse				= XMFLOAT4( 0.85f, 0.85f, 1.0f, 1.0f );
	//light2.specular				= XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	//light2.attenuation			= XMFLOAT3( 0.0f, 0.02f, 0.0f );

	//XMStoreFloat4x4( &light2.world, XMMatrixIdentity() );

	//mPointLightData.push_back( light2 );
	////---------------------------------------------------------------------------------------------

	//PointLightData light3;
	//light3.positionAndRadius = XMFLOAT4( 0.0f, 10.0f, 150.0f, 350.0f );
	//light3.ambient				= XMFLOAT4( 8.0f, 6.0f, 6.0f, 1.0f );
	//light3.diffuse				= XMFLOAT4( 0.85f, 0.85f, 1.0f, 1.0f );
	//light3.specular				= XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f );
	//light3.attenuation			= XMFLOAT3( 0.0f, 0.02f, 0.0f );

	//XMStoreFloat4x4( &light3.world, XMMatrixIdentity() );

	//mPointLightData.push_back( light3 );
	////============================================================

	if( FAILED( CreateDepthStencilView() ) )
		return E_FAIL;

	if( FAILED( InitializeShaders() ) )
		return E_FAIL;
	
	if( FAILED( CreateRasterizerStates() ) )
		return E_FAIL;

	if( FAILED( CreateSamplerState() ) )
		return E_FAIL;

	if( FAILED( CreateQuadVertexBuffer() ) )
		return E_FAIL;
	
	if( FAILED( CreatePerFrameCBuffer() ) )
		return E_FAIL;

	if( FAILED( CreateLightBuffer() ) )
		return E_FAIL;

	if( FAILED( CreateDepthLightCBuffer() ) )
		return E_FAIL;

	

	if( FAILED( mLevel->Initialize( mDevice, mDeviceContext ) ) )
		return E_FAIL;

	if( FAILED( mAssetManager->Initialize( mDevice, mDeviceContext ) ) )
		return E_FAIL;

	return S_OK;
}

Game::Game()
{
	mDevice						= nullptr;
	mDeviceContext				= nullptr;
	mDepthStencilView			= nullptr;
	mRasterizerStateSolid		= nullptr;
	mRasterizerStateWired		= nullptr;
	mCurrentRasterizerState		= nullptr;	
	mSamplerState				= nullptr;

	mGBuffers					= nullptr;

	mLevel				= nullptr;
	mCamera				= nullptr;
	mShaders			= nullptr;
	mTesselationAmount	= 0.0f;
}

Game::~Game()
{
}

void Game::Release()
{
	SAFE_RELEASE( mDepthStencilView );
	SAFE_RELEASE( mRasterizerStateSolid );
	SAFE_RELEASE( mRasterizerStateWired );
	SAFE_RELEASE( mSamplerState );

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