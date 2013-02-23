// A basic framework for a graphics engine.  This provides the core functionality for creating and destroying the window,
// initialisation of Direct3D 11 and the management of a scene graph. 
#include "Framework.h"

// DirectX libraries that are needed - make sure that $(DSSDK_DIR)lib\x86 is on the VC++ Directories Lib path
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
// The following is needed for the timeGetTime function
#pragma comment(lib, "winmm.lib")

// Default value for window caption if none is specified
#define DEFAULT_WINDOW_CAPTION	  L"Direct3D 11 Application"

// Default window dimensions used if none is specified
#define DEFAULT_WINDOW_LEFT		  0
#define DEFAULT_WINDOW_TOP		  0
#define DEFAULT_WINDOW_RIGHT	  640
#define DEFAULT_WINDOW_BOTTOM	  480

#define DEFAULT_FRAMES_PER_SECOND 25

// Reference to ourselves - primarily used to access the message handler correctly
Framework *		_thisFramework = NULL;

// Forward declaration
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	UNREFERENCED_PARAMETER(prevInstance);
	UNREFERENCED_PARAMETER(cmdLine);
	UNREFERENCED_PARAMETER(showCmd);

	if (_thisFramework)
	{
		return _thisFramework->Run(hInstance);
	}
	return -1;
}

Framework::Framework(void)
{
	_thisFramework = this;
	_device = NULL;
	_deviceContext = NULL;
	_swapChain = NULL;
	_renderTarget = NULL;
	_zBuffer = NULL;
	_sceneGraph = NULL;
	_renderer = NULL;
	_resourcesManager = NULL;
	_gamePadController = NULL;
	_camera = NULL;
}

Framework::~Framework(void)
{
}

wstring Framework::WindowCaption(void)
{
	return DEFAULT_WINDOW_CAPTION;
}

RECT Framework::WindowDimensions(void)
{
	RECT windowRectangle = {DEFAULT_WINDOW_LEFT, 
							DEFAULT_WINDOW_TOP,
							DEFAULT_WINDOW_RIGHT,
							DEFAULT_WINDOW_BOTTOM};
	return windowRectangle;
}

unsigned int Framework::FramesPerSecond(void)
{
	return DEFAULT_FRAMES_PER_SECOND;
}

// Accessors

Framework * Framework::This(void)
{
	return _thisFramework;
}

ID3D11Device * Framework::Device(void)
{
	return _device;
}

ID3D11DeviceContext * Framework::DeviceContext(void)
{
	return _deviceContext;
}

Renderer * Framework::GetCurrentRenderer(void)
{
	return _renderer;
}

void Framework::SetCurrentRenderer(Renderer * renderer)
{
   _renderer = renderer;
}

ResourcesManager * Framework::GetResourcesManager(void)
{
	return _resourcesManager;
}

Camera * Framework::GetCamera(void)
{
	return _camera;
}

 SceneGraph * Framework::GetSceneGraph(void)
{
    return _sceneGraph;
}

// Main entry point from WinMain

int Framework::Run(HINSTANCE hInstance)
{
	if (Startup(hInstance))
	{
		Initialise();
		_sceneGraph->Initialise();
		int result = MainLoop();
		ShutdownAll();
		return result;
	}
	else
	{
		return -1;
	}
}

// Private methods

bool Framework::Startup(HINSTANCE hInstance)
{
	_hInstance = hInstance;
	_hWindow = NULL;
	if (!InitialiseMainWindow())
	{
		return false;
	}
	if (!InitialiseDirect3D())
	{
		return false;
	}
    // TODO Uncomment out the following once SceneGraph has been implemented
	_resourcesManager = new ResourcesManager();
    _sceneGraph = new SceneGraph(L"root");
	_gamePadController = new GamePadController();
	_camera = new Camera();
	return true;
}

#pragma warning( disable : 4701 ) // disable potentially unitialised variable warning for msg 

int Framework::MainLoop(void)
{
	MSG msg;
	LARGE_INTEGER counterFrequency;
	LARGE_INTEGER nextTime;		
	LARGE_INTEGER currentTime;
	LARGE_INTEGER lastTime;
	double timeFactor;
	// MS per frame - default if no performance counter is available
	DWORD msPerFrame = 1000 / FramesPerSecond();				
	bool usePerformanceCounter = false;
	bool updateFlag = true;

	if (QueryPerformanceFrequency(&counterFrequency))
	{
		// We can use a performance counter
		usePerformanceCounter = true;
		msPerFrame = (DWORD)(counterFrequency.QuadPart / FramesPerSecond());
		timeFactor = 1.0 / counterFrequency.QuadPart;
		QueryPerformanceCounter(&nextTime);
		lastTime = nextTime;
	}
	else
	{
		// We do not have a performance counter, so use timeGetTime
		nextTime.QuadPart = timeGetTime();
		timeFactor = 0.001;
		lastTime = nextTime;
	}

	msg.message = WM_NULL;
	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{	
			_gamePadController->ProcessGameController(_camera);

			if (updateFlag)
			{
				if (usePerformanceCounter)
				{
					QueryPerformanceCounter(&currentTime);
				}
				else
				{
					currentTime.QuadPart = timeGetTime();
				}
				_timeSpan = (currentTime.QuadPart - lastTime.QuadPart) * timeFactor;
				lastTime = currentTime;
				UpdateAll();
				updateFlag = false;
			}
			// Use appropriate mechanism to get time
			if (usePerformanceCounter)
			{
				QueryPerformanceCounter(&currentTime);
			}
			else
			{
				currentTime.QuadPart = timeGetTime();
			}
			// Is it time to render the frame?
			if (currentTime.QuadPart > nextTime.QuadPart)
			{
				RenderAll();
				// Set time for next frame
				nextTime.QuadPart += msPerFrame;
				// If we get more than a frame ahead, allow one to be dropped
				// Otherwise, we will never catch up if we let the error accumulate
				// and message handling will suffer
				if (nextTime.QuadPart < currentTime.QuadPart)
				{
					nextTime.QuadPart = currentTime.QuadPart + msPerFrame;
				}
				updateFlag = true;
			}
		}
	}
	return (int)msg.wParam;
}

void Framework::UpdateAll(void)
{
	Update();
	_sceneGraph->Update(XMMatrixIdentity());
	_camera->Update();
}

void Framework::RenderAll(void)
{
	float clearColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	_deviceContext->ClearRenderTargetView(_renderTarget, clearColor);
	_deviceContext->ClearDepthStencilView(_zBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
	PreRender();	
    _sceneGraph->Render();
	PostRender();
	_swapChain->Present(0, 0);
}


#pragma warning( default : 4701 )

bool Framework::InitialiseDirect3D(void)
{
	RECT dimensions;

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE, 
		D3D_DRIVER_TYPE_WARP, 
		D3D_DRIVER_TYPE_SOFTWARE
	};

	unsigned int totalDriverTypes = ARRAYSIZE( driverTypes );

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};

	unsigned int totalFeatureLevels = ARRAYSIZE( featureLevels );

	GetClientRect(_hWindow, &dimensions );
	unsigned int width = dimensions.right - dimensions.left;
	unsigned int height = dimensions.bottom - dimensions.top;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory( &swapChainDesc, sizeof( swapChainDesc ) );
	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Width = width;
	swapChainDesc.BufferDesc.Height = height;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// Set the refresh rate to 0 and let DXGI determine the best option (refer to DXGI best practices)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = _hWindow;
	// Start out windowed
	swapChainDesc.Windowed = true;
	// Enable multi-sampling to give smoother lines (set to 1 if performance becomes an issue)
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;
	
	// Loop through the driver types to determine which one is available to us
	D3D_DRIVER_TYPE driverType = D3D_DRIVER_TYPE_UNKNOWN;

	for(unsigned int driver = 0; driver < totalDriverTypes && driverType == D3D_DRIVER_TYPE_UNKNOWN; driver++)
	{
		if (SUCCEEDED(D3D11CreateDeviceAndSwapChain(0, 
						 							driverTypes[driver], 
													0,
													0, 
													featureLevels, 
													totalFeatureLevels,
													D3D11_SDK_VERSION, 
													&swapChainDesc, 
													&_swapChain,
													&_device, 
													0, 
													&_deviceContext)))

		{
			driverType = driverTypes[driver];
		}
	}
	if (driverType == D3D_DRIVER_TYPE_UNKNOWN)
	{
		// Unable to find a suitable device driver
		return false;
	}
	return SetRenderTargetsAndViewport(width, height);
}

bool Framework::SetRenderTargetsAndViewport(unsigned int width, unsigned int height)
{
	ID3D11Texture2D* backBuffer;
	if (FAILED(_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
	{
		return false;
	}

	if (FAILED(_device->CreateRenderTargetView(backBuffer, NULL, &_renderTarget)))
	{
		return false;
	}
	backBuffer->Release();

	// Create a texture for the depth buffer
	D3D11_TEXTURE2D_DESC zBufferTexture;
	ZeroMemory(&zBufferTexture, sizeof(zBufferTexture));
	zBufferTexture.Width = width;
	zBufferTexture.Height = height;
	zBufferTexture.ArraySize = 1;
	zBufferTexture.MipLevels = 1;
	zBufferTexture.SampleDesc.Count = 4;
	zBufferTexture.Format = DXGI_FORMAT_D32_FLOAT;
	zBufferTexture.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D * depthBuffer;
	if (FAILED(_device->CreateTexture2D(&zBufferTexture, NULL, &depthBuffer)))
	{
		return false;
	}

	// Create the depth buffer
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	if (FAILED(_device->CreateDepthStencilView(depthBuffer, &dsvd, &_zBuffer)))
	{
		return false;
	}
	depthBuffer->Release();

	_deviceContext->OMSetRenderTargets(1, &_renderTarget, _zBuffer );

	// Set up the viewport.
	D3D11_VIEWPORT viewPort;
	viewPort.Width = static_cast<float>(width);
	viewPort.Height = static_cast<float>(height);
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	_deviceContext->RSSetViewports(1, &viewPort);
	return true;
}

bool Framework::InitialiseMainWindow(void)
{
	WNDCLASSEX wndClass = { 0 };
    wndClass.cbSize = sizeof( WNDCLASSEX ) ;
    // Tell Windows that it does not need to redraw the window since Direct3D is doing it.
    wndClass.style = 0; 
    wndClass.lpfnWndProc = WndProc;
    wndClass.hInstance = _hInstance;
    wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );
    // Set our background colour to transparent since Direct3D will be doing all of the drawing in the window
    wndClass.hbrBackground = (HBRUSH)0; 
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = L"DX11WindowClass";

    if( !RegisterClassEx( &wndClass ) )
	{
        return false;
	}
    RECT rc = this->WindowDimensions();
	// We are going to create a non-resizable window
	DWORD windowStyle = WS_CAPTION | WS_BORDER | WS_MINIMIZEBOX | WS_SYSMENU;
    AdjustWindowRect( &rc, windowStyle, FALSE );

    _hWindow = CreateWindowEx(NULL, L"DX11WindowClass", this->WindowCaption().c_str(),
			  				  windowStyle, 
							  CW_USEDEFAULT, CW_USEDEFAULT, 
							  rc.right - rc.left,
						      rc.bottom - rc.top, 
							  NULL, NULL, _hInstance, NULL );

    if (!_hWindow)
	{
        return false;
	}
    ShowWindow(_hWindow, SW_SHOW);
	UpdateWindow(_hWindow);
	return true;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (_thisFramework != NULL)
	{
		// If framework is started, then we can call our own message proc
		return _thisFramework->MsgProc(hwnd, msg, wParam, lParam);
	}
	else
	{
		// otherwise, we just pass control to the default message proc
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT Framework::MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
		case WM_CLOSE:
			// WM_CLOSE is sent when the user presses the 'X' button in the
			// caption bar menu.
			DestroyWindow(_hWindow);
			return 0;

		case WM_DESTROY:
			// WM_DESTROY is sent when the window is being destroyed.
			PostQuitMessage(0);
			return 0;

		case WM_KEYDOWN:
			HandleKeyboardMessage(wParam, lParam);
			return 0;

		case WM_SIZE:
			// As long as we are not minimizing the window, resize the back buffers on a resize (which happens if we
			// switch between full-screen and windowed).  This code works correctly on all hardware tested so far that
            // supports Direct3D 10/11, but does not appear to handle full-screen correctly when the WARP driver is used.
            // It appears as though we do not get the correct screen size passed to us in lParam when the window is 
            // maximised when the WARP driver is used.  More investigation is needed here.
			if (wParam != SIZE_MINIMIZED && _swapChain)
			{
				// Release all outstanding references to the swap chain's buffers
				_deviceContext->OMSetRenderTargets(0, NULL, NULL);
	            _renderTarget->Release();
				
				// Release the z buffer
				_zBuffer->Release();

				HRESULT hr;
				// Preserve the existing buffer count and format.
				// Use the height and width that are provided in WMSIZE
				hr = _swapChain->ResizeBuffers(0, 
											   LOWORD(lParam), 
											   HIWORD(lParam), 
											   DXGI_FORMAT_UNKNOWN, 
											   0);
                                            
				// We really should provide some error handling here 

				SetRenderTargetsAndViewport(LOWORD(lParam), HIWORD(lParam));
			}
			break;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

void Framework::ShutdownAll(void)
{
	if (_swapChain != NULL)
	{
		// Reset to windowed mode if we were full screen
		_swapChain->SetFullscreenState(FALSE, NULL);
	}
	if (_sceneGraph != NULL)
	{
		_sceneGraph->Shutdown();
		delete _sceneGraph;
		_sceneGraph = NULL;
	}

	if (_resourcesManager != NULL)
	{
		
		delete _resourcesManager;
	}

	if (_gamePadController != NULL)
	{
		delete _gamePadController;
	}

	SAFE_RELEASE(_zBuffer);
	SAFE_RELEASE(_renderTarget);
	SAFE_RELEASE(_swapChain);
	SAFE_RELEASE(_device);
	SAFE_RELEASE(_deviceContext);
	Shutdown();
}