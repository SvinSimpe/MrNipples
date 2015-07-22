#ifndef _SHADOWMAP_H_
#define _SHADOWMAP_H_

#include "3DLibs.h"

class ShadowMap
{
	private:
		unsigned int				mWidth;
		unsigned int				mHeight;
		ID3D11DepthStencilView*		mDepthMapStencil;
		ID3D11ShaderResourceView*	mDepthMapResource;
		D3D11_VIEWPORT				mViewPort;

	private:	
		HRESULT CreateShaderResourceView( ID3D11Device* device, ID3D11Texture2D* depthMap );
		HRESULT CreateDepthStencilView( ID3D11Device* device, ID3D11Texture2D* depthMap );

	public:
		ShadowMap();
		ShadowMap( ID3D11Device* device, unsigned int width, unsigned int height );
		~ShadowMap();

		void BindDsvAndSetNullRenderTarget( ID3D11DeviceContext* deviceContext );

		void Release();
};
#endif