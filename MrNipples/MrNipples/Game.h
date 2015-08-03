#ifndef _GAME_H_
#define _GAME_H_

#include "3DLibs.h"
#include "ShadowMapOmni.h"
#include "ShadowMap.h"
#include "Level.h"
#include "Camera.h"
#include "GBuffer.h"
#include "Shader.h"
#include "AssetManager.h"

const unsigned int NUM_GBUFFERS = 4;

class Game
{
	private:
		HWND*						mHWnd;			
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;
		ID3D11DepthStencilView*		mDepthStencilView;
		ID3D11RenderTargetView*		mRenderTargetView;
		ID3D11RasterizerState*		mRasterizerStateSolid;
		ID3D11RasterizerState*		mRasterizerStateWired;
		ID3D11RasterizerState*		mCurrentRasterizerState;
		ID3D11SamplerState*			mSamplerState;
		ID3D11Buffer*				mPerFrameCBuffer;
		PerFrameData				mPerFrameData;
		

		// Deferred
		GBuffer**					mGBuffers;
		ID3D11Buffer*				mQuadVertexBuffer;

		// Light
		std::vector<PointLightData>		mPointLightData;
		ID3D11Buffer*					mLightBuffer;
		int								mDirection;		// TEMPORARY FOR MOVING LIGHT!

		// Shadow
		ShadowMapOmni*				mShadowMapOmni;
		ShadowMap*					mShadowMap;
		DepthLightData				mDepthLightData;
		ID3D11Buffer*				mDepthLightCBuffer;

		// Game component
		Level*			mLevel;
		Camera*			mCamera;
		Shader**		mShaders;
		AssetManager*	mAssetManager;
		float			mTesselationAmount;

	private:
		HRESULT CreateDepthStencilView();
		HRESULT InitializeShaders();
		HRESULT CreateRasterizerStates();
		HRESULT CreateSamplerState();
		HRESULT CreateQuadVertexBuffer();

		HRESULT CreatePerFrameCBuffer();
		HRESULT CreateLightBuffer();
		HRESULT CreateDepthLightCBuffer();

		HRESULT UpdatePerFrameCBuffer();
		HRESULT UpdateLightBuffer();
		HRESULT UpdateDepthLightCBuffer();

	public:
		HRESULT RenderShadowOmniPass( float deltaTime );
		HRESULT RenderStandard( float deltaTime );		// Forward rendering with tesselation
		HRESULT RenderGeometryPass( float deltaTime );
		HRESULT RenderShadowPass( float deltaTime );
		HRESULT RenderDeferredPass( float deltaTime );
		

	public:
		void	SetRasterizerStateWired( bool isWired );
		void	SetTesselationAmount( float tesselationAmount );

		void	Update( float deltaTime );
		void	Render( float deltaTime );
		HRESULT Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext, ID3D11RenderTargetView* renderTargetView );
				Game();
				~Game();
		void	Release();
};

#endif