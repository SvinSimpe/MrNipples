#ifndef _GAME_H_
#define _GAME_H_

#include "3DLibs.h"
#include "Level.h"
#include "Camera.h"

class Game
{
	private:
		ID3D11Device*				mDevice;
		ID3D11DeviceContext*		mDeviceContext;
		ID3D11RasterizerState*		mRasterizerStateSolid;
		ID3D11RasterizerState*		mRasterizerStateWired;
		ID3D11RasterizerState*		mCurrentRasterizerState;
		ID3D11InputLayout*			mInputLayoutBasic;
		ID3D11VertexShader*			mVertexShader;
		ID3D11PixelShader*			mPixelShader;
		ID3D11Buffer*				mPerFrameCBuffer;
		PerFrameData				mPerFrameData;

		// Game component
		Level*	mLevel;
		Camera*	mCamera;

	private:
		HRESULT CompileShader( char* shaderFile, char* pEntrypoint, char* pTarget, D3D10_SHADER_MACRO* pDefines, ID3DBlob** pCompiledShader );
		HRESULT InitializeBasicShaders();
		HRESULT CreateRasterizerStates();
		HRESULT CreatePerFrameCBuffer();
		HRESULT UpdatePerFrameCBuffer();
		

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