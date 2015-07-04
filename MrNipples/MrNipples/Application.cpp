#include "Application.h"
#include <sstream>
#include <string>

void Application::Render( float deltaTime )
{
	// Clear Back Buffer
	static float clearColor[4] = { 0.4f, 0.35f, 0.15f, 1.0f };

	mDeviceContext->ClearRenderTargetView( mRenderTargetView, clearColor );

	// Clear Depth Buffer
	mDeviceContext->ClearDepthStencilView( mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

	

	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );

	// Swap Front and Back Buffer
	mSwapChain->Present( 0, 0 );
}

void Application::PrintFPSinWindowHeader()
{
	//long time = time * 0.9 + last_frame * 0.1

	//std::wstringstream wss;
	//std::wstring ws;

	//wss.clear();
	//		
	//wss << "FPS: " << mTimer.GetFPS() << "  DeltaTime: " << mTimer.GetDeltaTime();
	//ws = wss.str();
	//
	//SetWindowTextW( mHWnd, ws.c_str() );
	//ws.clear();
	//wss.str(ws);
}

void Application::SetViewPort()
{
	RECT rc;
	GetClientRect( mHWnd, &rc );

	int width	= rc.right - rc.left;
	int height	= rc.bottom - rc.top;

	//-------------
	// Set Viewport
	//-------------
	D3D11_VIEWPORT vp;
	vp.Width	= (float)width;
	vp.Height	= (float)height;
	vp.MinDepth	= 0.0f;
	vp.MaxDepth	= 1.0f;
	vp.TopLeftX	= 0;
	vp.TopLeftY	= 0;
	mDeviceContext->RSSetViewports( 1, &vp );

	mDeviceContext->OMSetRenderTargets( 1, &mRenderTargetView, mDepthStencilView );
}

void Application::HandleInput( UINT msg, WPARAM wParam, LPARAM lParam, float deltaTime )
{
	mCurrKeyboardState = msg;

	switch ( msg )
	{
		case WM_KEYDOWN: 
            switch (wParam) 
            { 
                case VK_LEFT: 
                    
                    // Process the LEFT ARROW key. 
 

                    break; 
 
                case VK_RIGHT: 
                    
                    // Process the RIGHT ARROW key. 
 
                    break; 
 
                case VK_UP: 
                    
                    // Process the UP ARROW key. 

                    break; 
 
                case VK_DOWN: 
                    
                    // Process the DOWN ARROW key. 

                    break; 

			    case VK_CONTROL: 
                    
                    // Process the Control  key. 
					if( mPrevKeyboardState == WM_KEYUP )
					{
						if( mChangeRasterizerState )
							mChangeRasterizerState = false; 
						else
							mChangeRasterizerState = true; 

						mGame->SetRasterizerStateWired( mChangeRasterizerState );
					}	
                    break;

			     case VK_DELETE: 
                    
                    // Process the DEL key.
					BOOL isFullscreen = false;
					mSwapChain->GetFullscreenState( &isFullscreen, nullptr );
					
					if( isFullscreen )
						mSwapChain->SetFullscreenState( false, nullptr );
					else
						mSwapChain->SetFullscreenState( true, nullptr );


                    break; 
			}
	}

	mPrevKeyboardState = mCurrKeyboardState;
}

Application* g_app = 0;

LRESULT CALLBACK MainWndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return g_app->WndProc( hwnd, msg, wParam, lParam );
}

LRESULT CALLBACK Application::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch ( msg )
	{
		case WM_PAINT:
			hdc = BeginPaint( hWnd, &ps );
			EndPaint( hWnd, &ps );
			break;

		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;

		case WM_KEYDOWN:
			switch ( wParam )
			{
				case VK_ESCAPE:
					PostQuitMessage( 0 );
					break;
			}
			break;
		
		default:
			return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

Application::Application()
{
	mHInstance		= NULL;
	mHWnd			= NULL;
	mWindowHeight	= 720;
	mWindowWidht	= 1280;

	mDevice				= nullptr;
	mDeviceContext		= nullptr;
	mSwapChain			= nullptr;
	mRenderTargetView	= nullptr;
	mDepthStencil		= nullptr;
	mDepthStencilView	= nullptr;

	mChangeRasterizerState	= false;
	mPrevKeyboardState		= 0;
	mCurrKeyboardState		= 0;

	mGame					= nullptr;
}

Application::~Application()
{

}

HRESULT Application::InitializeWindow( HINSTANCE hInstance, int nCmdShow )
{
	HRESULT hr = S_OK;

	mHInstance = hInstance;

	//----------------------
	// Register Window Class
	//----------------------
	WNDCLASSEX wc;
	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= mHInstance;
	wc.hIcon			= 0;
	wc.hCursor			= LoadCursor( NULL, IDC_HAND );
	wc.hbrBackground	= (HBRUSH)( COLOR_WINDOW + 1 );
	wc.lpszMenuName		= NULL;
	wc.lpszClassName	= "Mr Nipples      (^<^)";
	wc.hIconSm			= 0;

	if( FAILED( hr = RegisterClassEx( &wc ) ) )
		return hr;

	//-----------------------
	// Adjust & Create Window
	//-----------------------
	RECT rc = { 0, 0, mWindowWidht, mWindowHeight };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if( !( mHWnd = CreateWindow(	"Mr Nipples      (^<^)",
									"Mr Nipples      (^<^)",
									WS_OVERLAPPEDWINDOW,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									rc.right - rc.left,
									rc.bottom - rc.top,
									NULL,
									NULL,
									mHInstance,
									NULL ) ) )
	{
		return E_FAIL;
	}

	ShowWindow( mHWnd, nCmdShow );
	ShowCursor( TRUE );


	return hr;
}

HRESULT Application::InitializeDirectX11()
{
	HRESULT hr = E_FAIL;

	RECT rc;
	GetClientRect( mHWnd, &rc );

	int width	= rc.right - rc.left;
	int height	= rc.bottom - rc.top;


	//-------------------------------------------
	// Create Swap Chain, Device & Device Context
	//-------------------------------------------

	D3D_DRIVER_TYPE driverTypes[] = { D3D_DRIVER_TYPE_HARDWARE,
									  D3D_DRIVER_TYPE_WARP,
									  D3D_DRIVER_TYPE_REFERENCE };

	DXGI_SWAP_CHAIN_DESC sd;
	memset( &sd, 0, sizeof( sd ) );
	sd.BufferCount				= 1;
	sd.BufferDesc.Width						= width;
	sd.BufferDesc.Height					= height;
	sd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator		= 60;
	sd.BufferDesc.RefreshRate.Denominator	= 1;
	sd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow							= mHWnd;
	sd.SampleDesc.Count						= 1;
	sd.Windowed								= TRUE;
	sd.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	D3D_FEATURE_LEVEL featureLevelsToTry[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D_FEATURE_LEVEL initiatedFeatureLevel;

	for ( UINT driverTypeIndex = 0; driverTypeIndex < ARRAYSIZE( driverTypes ) && FAILED( hr ); driverTypeIndex++ )
	{
		hr = D3D11CreateDeviceAndSwapChain( nullptr,
											driverTypes[driverTypeIndex],
											NULL,
											0,
											featureLevelsToTry,
											ARRAYSIZE( featureLevelsToTry ),
											D3D11_SDK_VERSION,
											&sd,
											&mSwapChain,
											&mDevice,
											&initiatedFeatureLevel,
											&mDeviceContext );
	}

	if( FAILED( hr ) )
		return hr;


	//--------------------------
	// Create Render Target View
	//--------------------------
	ID3D11Texture2D* pBackBuffer;
	if ( SUCCEEDED( hr = mSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pBackBuffer ) ) )
	{
		hr = mDevice->CreateRenderTargetView( pBackBuffer, nullptr, &mRenderTargetView );
		SAFE_RELEASE( pBackBuffer );
	}
	
	if( FAILED( hr ) )
		return hr;


	//--------------------------
	// Create Depth Stencil View
	//--------------------------
	D3D11_TEXTURE2D_DESC dsd;
	dsd.Width				= width;
	dsd.Height				= height;
	dsd.MipLevels			= 1;
	dsd.ArraySize			= 1;
	dsd.Format				= DXGI_FORMAT_D32_FLOAT;
	dsd.SampleDesc.Count	= 1;
	dsd.SampleDesc.Quality	= 0;
	dsd.Usage				= D3D11_USAGE_DEFAULT;
	dsd.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	dsd.CPUAccessFlags		= 0;
	dsd.MiscFlags			= 0;

	if( FAILED( hr = mDevice->CreateTexture2D( &dsd, nullptr, &mDepthStencil ) ) )
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof( dsvd ) );
	dsvd.Format				= dsd.Format;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice	= 0;

	if( FAILED( hr = mDevice->CreateDepthStencilView( mDepthStencil, &dsvd, &mDepthStencilView ) ) )
		return hr;

	SetViewPort();

	// Game component
	mGame = new Game();
	hr = mGame->Initialize( mDevice, mDeviceContext );

	return hr;
}

int Application::Run()
{
	__int64 cntsPerSec = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);
	float secsPerCnt = 1.0f / (float)cntsPerSec;

	__int64 prevTimeStamp = 0;
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTimeStamp);

	MSG msg = {0};

	while ( WM_QUIT != msg.message )
	{

		if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage ( &msg );
		}
		else
		{
			__int64 currTimeStamp = 0;
			QueryPerformanceCounter((LARGE_INTEGER*)&currTimeStamp);
			float deltaTime = (currTimeStamp - prevTimeStamp) * secsPerCnt;


			HandleInput( msg.message, msg.wParam, msg.lParam, deltaTime );
			mGame->Update( deltaTime );
			Render( deltaTime );

			///TEST
			//if( m_changeRasterizerState )
			//{
			//	if( m_currentRasterizerState == m_rasterizerStateSolid )
			//		m_currentRasterizerState = m_rasterizerStateWired;
			//	else
			//		m_currentRasterizerState = m_rasterizerStateSolid;

			//	m_changeRasterizerState = false;
			//}

			prevTimeStamp = currTimeStamp;

		}
	}

	return (int)msg.wParam;
}

void Application::Release()
{
	mGame->Release();

	SAFE_RELEASE( mDevice );
	SAFE_RELEASE( mDeviceContext );
	SAFE_RELEASE( mSwapChain );
	SAFE_RELEASE( mRenderTargetView );
	SAFE_RELEASE( mDepthStencil );
	SAFE_RELEASE( mDepthStencilView );
}