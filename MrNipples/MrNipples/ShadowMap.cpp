#include "ShadowMap.h"

HRESULT ShadowMap::CreateShaderResourceView( ID3D11Device* device, ID3D11Texture2D* depthMap )
{
	HRESULT hr = S_OK;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format						= DXGI_FORMAT_R32_FLOAT;
	srvDesc.ViewDimension				= D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels			= 1;
	srvDesc.Texture2D.MostDetailedMip	= 0;

	hr = device->CreateShaderResourceView( depthMap, &srvDesc, &mDepthMapResource );

	return hr;
}

HRESULT ShadowMap::CreateDepthStencilView( ID3D11Device* device, ID3D11Texture2D* depthMap )
{
	HRESULT hr = S_OK;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags				= 0;
	dsvDesc.Format				= DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Texture2D.MipSlice	= 0;

	hr = device->CreateDepthStencilView( depthMap, &dsvDesc, &mDepthMapStencil );

	return hr;
}

ShadowMap::ShadowMap()
{
	mWidth				= 0;
	mHeight				= 0;
	mDepthMapStencil	= nullptr;
	mDepthMapResource	= nullptr;
	ZeroMemory( &mViewPort, sizeof(D3D11_VIEWPORT) );
}

ShadowMap::ShadowMap( ID3D11Device* device, unsigned int width, unsigned int height )
{
	mWidth				= width;
	mHeight				= height;
	mDepthMapResource	= nullptr;
	mDepthMapStencil	= nullptr;

	/// Viewport that matches the shadow map dimensions
	mViewPort.TopLeftX	= 0.0f;
	mViewPort.TopLeftY	= 0.0f;
	mViewPort.Width		= static_cast<float>( width );
	mViewPort.Height	= static_cast<float>( height );
	mViewPort.MinDepth	= 0.0f;
	mViewPort.MaxDepth	= 1.0f;

	/// Use typless format because the DSV is going to interpret
	/// the bits as DXGI_FORMAT_D24_UNORM_S8_UINT, whereas the SRV is going
	/// to interpret the bits as DXGI_FORMAT_R24_UNORM_X8_TYPELESS.
	D3D11_TEXTURE2D_DESC textureDesc;
	textureDesc.Width				= mWidth;
	textureDesc.Height				= mHeight;
	textureDesc.MipLevels			= 1;
	textureDesc.ArraySize			= 1;
	textureDesc.Format				= DXGI_FORMAT_R32_TYPELESS;
	textureDesc.Usage				= D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags		= 0;
	textureDesc.MiscFlags			= 0;
	textureDesc.SampleDesc.Count	= 1;
	textureDesc.SampleDesc.Quality	= 0;

	ID3D11Texture2D* depthMap = 0;
	if( FAILED( device->CreateTexture2D( &textureDesc, 0, &depthMap ) ) )
		OutputDebugString( "Failed to create Texture2D in ShadowMap\n" );

	if( FAILED( CreateDepthStencilView( device, depthMap ) ) )
		OutputDebugString( "Failed to create DepthStencilView in ShadowMap\n" );

	if( FAILED( CreateShaderResourceView( device, depthMap ) ) )
		OutputDebugString( "Failed to create ShaderResourceView in ShadowMap\n" );

	depthMap->Release();
}

ShadowMap::~ShadowMap()
{}

void ShadowMap::BindDsvAndSetNullRenderTarget( ID3D11DeviceContext* deviceContext )
{
	deviceContext->RSSetViewports( 1, &mViewPort );

	// Setting a null render target will disable color writes
	ID3D11RenderTargetView* renderTargets[] = { nullptr };
	deviceContext->OMSetRenderTargets( 1, renderTargets, mDepthMapStencil );

	deviceContext->ClearDepthStencilView( mDepthMapStencil, D3D11_CLEAR_DEPTH, 1.0f, 0 );
}

ID3D11ShaderResourceView* ShadowMap::ShaderResourceView()
{
	return mDepthMapResource;
}

void ShadowMap::Release()
{
	SAFE_RELEASE( mDepthMapStencil );
	SAFE_RELEASE( mDepthMapResource );
}