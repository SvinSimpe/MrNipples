#ifndef _SHADOWMAPOMNI_H_
#define _SHADOWMAPOMNI_H_

#include "3DLibs.h"

class ShadowMapOmni // Omni-directional Shadow Map used for point lights
{
	private:
		unsigned int				mShadowMapSize; // i.e. 512 equals 512*512

		ID3D11DepthStencilView*		mDepthMapStencil;
		ID3D11ShaderResourceView*	mDepthMapResourceView;
		ID3D11SamplerState*			mShadowSampler;

		D3D11_VIEWPORT				mViewPort;

	private:	
		HRESULT CreateShadowShaderResourceView( ID3D11Device* device, ID3D11Texture2D* depthMap, DXGI_FORMAT format );
		HRESULT CreateShadowDepthStencilView( ID3D11Device* device, ID3D11Texture2D* depthMap );
		HRESULT CreateShadowSamplerState( ID3D11Device* device );

	public:
		ShadowMapOmni();
		~ShadowMapOmni();

		HRESULT Initialize( ID3D11Device* device, unsigned int shadowMapsize );

		void BindDsvAndSetNullRenderTarget( ID3D11DeviceContext* deviceContext );
		void SetShadowRenderTargetViews( ID3D11DeviceContext* deviceContext, UINT startLocation );

		unsigned int GetShadowMapSize() const;
		ID3D11SamplerState*	GetShadowMapSampler() const;

		void Release();
};
#endif