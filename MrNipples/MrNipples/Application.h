#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "3Dlibs.h"
#include "Game.h"

class Application
{
	protected:
		//--------
		// Window
		//--------
		HINSTANCE	mHInstance;
		HWND		mHWnd;
		int			mWindowHeight;
		int			mWindowWidht;

		//---------
		// DirectX
		//---------
		ID3D11Device*			mDevice;
		ID3D11DeviceContext*	mDeviceContext;
		IDXGISwapChain*			mSwapChain;
		ID3D11RenderTargetView*	mRenderTargetView;
		ID3D11Texture2D*		mDepthStencil;
		ID3D11DepthStencilView*	mDepthStencilView;

		bool	mChangeRasterizerState;
		
		// Input
		UINT	mPrevKeyboardState;
		UINT	mCurrKeyboardState;

		// Game component
		Game *mGame;


	private:
		void Render( float deltaTime );

	protected:
		void PrintFPSinWindowHeader();
		void SetViewPort();
		void HandleInput( UINT msg, WPARAM wParam, LPARAM lParam, float deltaTime  );

	public:
		LRESULT CALLBACK WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		Application();
		~Application();

		HRESULT InitializeWindow( HINSTANCE hInstance, int nCmdShow );
		HRESULT InitializeDirectX11();

		int Run();

		void Release();
};
#endif