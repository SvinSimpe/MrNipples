#ifndef _GAME_H_
#define _GAME_H_

#include "3DLibs.h"
#include "ShadowMap.h"
#include "Level.h"
#include "Camera.h"
#include "GBuffer.h"
#include "Shader.h"

const unsigned int NUM_GBUFFERS = 4;

class Game
{
	private:
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;
		ID3D11RasterizerState*		mRasterizerStateSolid;
		ID3D11RasterizerState*		mRasterizerStateWired;
		ID3D11RasterizerState*		mCurrentRasterizerState;
		ID3D11Buffer*				mPerFrameCBuffer;
		PerFrameData				mPerFrameData;

		// Deferred
		GBuffer**					mGBuffers;

		// Light
		std::vector<PointLightData>		mPointLightData;
		ID3D11Buffer*					mLightBuffer;
		int								mDirection;		// TEMPORARY FOR MOVING LIGHT!

		// Shadow
		ShadowMap*					mShadowMap;
		DepthLightData				mDepthLightData;
		ID3D11Buffer*				mDepthLightCBuffer;

		// Game component
		Level*		mLevel;
		Camera*		mCamera;
		Shader**	mShaders;

	private:
		HRESULT InitializeShaders();
		HRESULT CreateRasterizerStates();

		HRESULT CreatePerFrameCBuffer();
		HRESULT CreateLightBuffer();
		HRESULT CreateDepthLightCBuffer();

		HRESULT UpdatePerFrameCBuffer();
		HRESULT UpdateLightBuffer();
		HRESULT UpdateDepthLightCBuffer();

		HRESULT RenderStandard( float deltaTime );		// Forward rendering with tesselation
		HRESULT RenderGeometryPass( float deltaTime );
		HRESULT RenderShadowPass( float deltaTime );
		HRESULT RenderDeferredPass( float deltaTime );
		

	public:
		void	SetRasterizerStateWired( bool isWired );

		void	Update( float deltaTime );
		void	Render( float deltaTime );
		HRESULT Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
				Game();
				~Game();
		void	Release();
};

#endif