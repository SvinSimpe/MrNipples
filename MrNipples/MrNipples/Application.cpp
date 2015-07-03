#include "Application.h"
#include <sstream>
#include <string>

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
	GetClientRect( m_hWnd, &rc );

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
	m_deviceContext->RSSetViewports( 1, &vp );

	m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilView );
}

void Application::HandleInput( UINT msg, WPARAM wParam, LPARAM lParam, float deltaTime )
{
	m_currKeyboardState = msg;

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
					if( m_prevKeyboardState == WM_KEYUP )
						m_changeRasterizerState = true; 
                    break;

			     case VK_DELETE: 
                    
                    // Process the DEL key.
					BOOL isFullscreen = false;
					m_swapChain->GetFullscreenState( &isFullscreen, nullptr );
					
					if( isFullscreen )
						m_swapChain->SetFullscreenState( false, nullptr );
					else
						m_swapChain->SetFullscreenState( true, nullptr );


                    break; 
			}
	}

	m_prevKeyboardState = m_currKeyboardState;
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
	m_hInstance		= NULL;
	m_hWnd			= NULL;
	m_windowHeight	= 720;
	m_windowWidht	= 1280;

	m_device			= nullptr;
	m_deviceContext		= nullptr;
	m_swapChain			= nullptr;
	m_renderTargetView	= nullptr;
	m_depthStencil		= nullptr;
	m_depthStencilView	= nullptr;

	m_changeRasterizerState = false;
	m_prevKeyboardState		= 0;
	m_currKeyboardState		= 0;
}

Application::~Application()
{

}

HRESULT Application::InitializeWindow( HINSTANCE hInstance, int nCmdShow )
{
	HRESULT hr = S_OK;

	m_hInstance = hInstance;

	//----------------------
	// Register Window Class
	//----------------------
	WNDCLASSEX wc;
	wc.cbSize			= sizeof( WNDCLASSEX );
	wc.style			= CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc		= MainWndProc;
	wc.cbClsExtra		= 0;
	wc.cbWndExtra		= 0;
	wc.hInstance		= m_hInstance;
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
	RECT rc = { 0, 0, m_windowWidht, m_windowHeight };
	AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

	if( !( m_hWnd = CreateWindow(	"Mr Nipples      (^<^)",
									"Mr Nipples      (^<^)",
									WS_OVERLAPPEDWINDOW,
									CW_USEDEFAULT,
									CW_USEDEFAULT,
									rc.right - rc.left,
									rc.bottom - rc.top,
									NULL,
									NULL,
									m_hInstance,
									NULL ) ) )
	{
		return E_FAIL;
	}

	ShowWindow( m_hWnd, nCmdShow );
	ShowCursor( TRUE );


	return hr;
}

HRESULT Application::InitializeDirectX11()
{
	HRESULT hr = E_FAIL;

	RECT rc;
	GetClientRect( m_hWnd, &rc );

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
	sd.OutputWindow							= m_hWnd;
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
											&m_swapChain,
											&m_device,
											&initiatedFeatureLevel,
											&m_deviceContext );
	}

	if( FAILED( hr ) )
		return hr;


	//--------------------------
	// Create Render Target View
	//--------------------------
	ID3D11Texture2D* pBackBuffer;
	if ( SUCCEEDED( hr = m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), (void**)&pBackBuffer ) ) )
	{
		hr = m_device->CreateRenderTargetView( pBackBuffer, nullptr, &m_renderTargetView );
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

	if( FAILED( hr = m_device->CreateTexture2D( &dsd, nullptr, &m_depthStencil ) ) )
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory( &dsvd, sizeof( dsvd ) );
	dsvd.Format				= dsd.Format;
	dsvd.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Texture2D.MipSlice	= 0;

	if( FAILED( hr = m_device->CreateDepthStencilView( m_depthStencil, &dsvd, &m_depthStencilView ) ) )
		return hr;

	SetViewPort();

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
	SAFE_RELEASE( m_device );
	SAFE_RELEASE( m_deviceContext );
	SAFE_RELEASE( m_swapChain );
	SAFE_RELEASE( m_renderTargetView );
	SAFE_RELEASE( m_depthStencil );
	SAFE_RELEASE( m_depthStencilView );
}