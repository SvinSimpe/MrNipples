#include "ShadowMapOmni.h"

HRESULT ShadowMapOmni::CreateShadowShaderResourceView( ID3D11Device* device, ID3D11Texture2D* depthMap, DXGI_FORMAT format  )
{
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format						= format;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Texture2D.MipLevels			= 1;
	srvDesc.Texture2D.MostDetailedMip	= 0;

	if( FAILED( device->CreateShaderResourceView( depthMap, &srvDesc, &mDepthMapResourceView ) ) )
		return E_FAIL;

	return S_OK;
}

HRESULT ShadowMapOmni::CreateShadowDepthStencilView( ID3D11Device* device, ID3D11Texture2D* depthMap )
{
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags							= 0;
	dsvDesc.Format							= DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension					= D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsvDesc.Texture2DArray.MipSlice			= 0;
	dsvDesc.Texture2DArray.FirstArraySlice	= 0;
	dsvDesc.Texture2DArray.ArraySize		= 6;

	if( FAILED( device->CreateDepthStencilView( depthMap, &dsvDesc, &mDepthMapStencil ) ))
		return E_FAIL;

	return S_OK;
}

HRESULT ShadowMapOmni::CreateShadowSamplerState( ID3D11Device* device )
{
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.MipLODBias = 0;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samplerDesc.BorderColor[0] = 0.0f;
	samplerDesc.BorderColor[1] = 0.0f;
	samplerDesc.BorderColor[2] = 0.0f;
	samplerDesc.BorderColor[3] = 0.0f;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.MaxLOD = 0.0f;

	if( FAILED( device->CreateSamplerState( &samplerDesc, &mShadowSampler ) ) )
		return E_FAIL;

	return S_OK;
}

ShadowMapOmni::ShadowMapOmni()
{
	mShadowMapSize			= 0;
	mDepthMapStencil		= nullptr;
	mDepthMapResourceView	= nullptr;
	mShadowSampler			= nullptr;
	ZeroMemory( &mViewPort, sizeof(D3D11_VIEWPORT) );
}

ShadowMapOmni::~ShadowMapOmni()
{}

HRESULT ShadowMapOmni::Initialize( ID3D11Device* device, unsigned int shadowMapsize )
{
	mShadowMapSize = shadowMapsize;

	// Viewport that matches the shadow map dimensions
	mViewPort.TopLeftX	= 0.0f;
	mViewPort.TopLeftY	= 0.0f;
	mViewPort.Width		= static_cast<float>( mShadowMapSize );
	mViewPort.Height	= static_cast<float>( mShadowMapSize );
	mViewPort.MinDepth	= 0.0f;
	mViewPort.MaxDepth	= 1.0f;

	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width				= mShadowMapSize;
	textureDesc.Height				= mShadowMapSize;
	textureDesc.MipLevels			= 1;   // CHANGED THIS FROM 0
	textureDesc.ArraySize			= 6;
	textureDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
	textureDesc.Usage				= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MiscFlags			= D3D11_RESOURCE_MISC_TEXTURECUBE;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.SampleDesc.Quality	= 0;

	ID3D11Texture2D* depthMapTexture = 0;
	if( FAILED( device->CreateTexture2D( &textureDesc, 0, &depthMapTexture ) ) )
	{
		OutputDebugString( "Failed to create Texture2D in ShadowMapOmni\n" );
		return E_FAIL;
	}

	if( FAILED( CreateShadowDepthStencilView( device, depthMapTexture ) ) )
	{
		OutputDebugString( "Failed to create Depth Stencil View in ShadowMapOmni\n" );
		return E_FAIL;
	}

	if( FAILED( CreateShadowShaderResourceView( device, depthMapTexture, DXGI_FORMAT_R32_FLOAT ) ) )
	{
		OutputDebugString( "Failed to create Shader Target View in ShadowMapOmni\n" );
		return E_FAIL;
	}

	if( FAILED( CreateShadowSamplerState( device ) ) )
	{
		OutputDebugString( "Failed to create Sampler State in ShadowMapOmni\n" );
		return E_FAIL;
	}

	depthMapTexture->Release();

	return S_OK;
}

void ShadowMapOmni::BindDsvAndSetNullRenderTarget( ID3D11DeviceContext* deviceContext )
{
	deviceContext->RSSetViewports( 1, &mViewPort );

	// Setting a null render target will disable color writes
	ID3D11RenderTargetView* renderTargets[] = { nullptr };
	deviceContext->OMSetRenderTargets( 1, renderTargets, mDepthMapStencil );

	deviceContext->ClearDepthStencilView( mDepthMapStencil, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

void ShadowMapOmni::SetShadowRenderTargetViews( ID3D11DeviceContext* deviceContext, UINT startLocation )
{
	deviceContext->PSSetShaderResources( startLocation, 1, &mDepthMapResourceView );
}

unsigned int ShadowMapOmni::GetShadowMapSize() const
{
	return mShadowMapSize;
}

ID3D11SamplerState* ShadowMapOmni::GetShadowMapSampler() const
{
	return mShadowSampler;
}

void ShadowMapOmni::Release()
{
	SAFE_RELEASE( mDepthMapStencil );
	SAFE_RELEASE( mDepthMapResourceView );
	SAFE_RELEASE( mShadowSampler );
}