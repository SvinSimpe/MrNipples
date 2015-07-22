#ifndef _GBUFFER_H_
#define _GBUFFER_H_

#include "3DLibs.h"

class GBuffer
{
	private:
		ID3D11RenderTargetView*		mRenderTargetView;
		ID3D11ShaderResourceView*	mShaderResourceView;

	public:
		GBuffer();
		GBuffer( ID3D11Device* device, unsigned int width, unsigned int height, DXGI_FORMAT format );
		~GBuffer();

		ID3D11RenderTargetView*		RenderTargetView();
		ID3D11ShaderResourceView*	ShaderResourceView();

		void Release();
};
#endif