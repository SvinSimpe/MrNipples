#include "GBuffer.h"

GBuffer::GBuffer()
{
	mRenderTargetView	= nullptr;
	mShaderResourceView = nullptr;
}

GBuffer::GBuffer( ID3D11Device* device, unsigned int width, unsigned int height, DXGI_FORMAT format )
{
			ID3D11Texture2D* tempTexture;

			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory( &textureDesc, sizeof( textureDesc ) );
			textureDesc.Width				= width;
			textureDesc.Height				= height;
			textureDesc.BindFlags			= D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			textureDesc.Usage				= D3D11_USAGE_DEFAULT;
			textureDesc.MipLevels			= 0;
			textureDesc.ArraySize			= 1;
			textureDesc.SampleDesc.Count	= 1;
			textureDesc.Format				= format;

			
			if( FAILED( device->CreateTexture2D( &textureDesc, nullptr, &tempTexture ) ) )
				OutputDebugStringA( "FAILED TO CREATE GBUFFER TEXTURE :: GBuffer.cpp" );

			D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
			ZeroMemory( &renderTargetDesc, sizeof( renderTargetDesc ) );
			renderTargetDesc.Format			= textureDesc.Format;
			renderTargetDesc.ViewDimension	= D3D11_RTV_DIMENSION_TEXTURE2D;

			if( FAILED( device->CreateRenderTargetView( tempTexture, &renderTargetDesc, &mRenderTargetView ) ) )
				OutputDebugStringA( "FAILED TO CREATE GBUFFER RENDER TARGET VIEW :: GBuffer.cpp" );
			
			if( FAILED( device->CreateShaderResourceView( tempTexture, nullptr, &mShaderResourceView ) ) )
				OutputDebugStringA( "FAILED TO CREATE GBUFFER SHADER RESOURCE VIEW :: GBuffer.cpp" );

			SAFE_RELEASE( tempTexture );
}

GBuffer::~GBuffer()
{}

ID3D11RenderTargetView* GBuffer::RenderTargetView()
{
	return mRenderTargetView;
}

ID3D11ShaderResourceView* GBuffer::ShaderResourceView()
{
	return mShaderResourceView;
}

void GBuffer::Release()
{
	SAFE_RELEASE( mRenderTargetView );
	SAFE_RELEASE( mShaderResourceView );
}