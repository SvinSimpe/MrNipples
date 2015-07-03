#ifndef _GAME_H_
#define _GAME_H_

#include "3DLibs.h"

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

	public:
		void	Update( float deltaTime );
		void	Render( float deltaTime );
		HRESULT Initialize( ID3D11Device* device, ID3D11DeviceContext* deviceContext );
				Game();
				~Game();
		void	Release();
};

#endif