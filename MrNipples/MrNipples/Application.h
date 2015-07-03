#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include "3Dlibs.h"

class Application
{
	protected:
		//--------
		// Window
		//--------
		HINSTANCE	m_hInstance;
		HWND		m_hWnd;
		int			m_windowHeight;
		int			m_windowWidht;

		//---------
		// DirectX
		//---------
		ID3D11Device*			m_device;
		ID3D11DeviceContext*	m_deviceContext;
		IDXGISwapChain*			m_swapChain;
		ID3D11RenderTargetView*	m_renderTargetView;
		ID3D11Texture2D*		m_depthStencil;
		ID3D11DepthStencilView*	m_depthStencilView;

		bool	m_changeRasterizerState;
		
		// Input
		UINT	m_prevKeyboardState;
		UINT	m_currKeyboardState;

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