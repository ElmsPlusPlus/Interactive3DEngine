#pragma once
#include "core.h"
#include "Renderer.h"
#include "SceneGraph.h"
#include "ResourcesManager.h"
#include "GamePadController.h"
#include "Camera.h"
#include "TerrainNode.h"

using namespace std;

class Framework
{
public:
	Framework(void);
	Framework(wstring windowCaption);
	virtual ~Framework(void);

	static Framework * This(void);

    // The following methods can be overridden
    //
    // Initialise is called once the window has been created and Direct3D initialised, but before the scene graph is
    // initialised.  This is where you create a renderer (ensure you call SetCurrentRenderer) and 
    // add your nodes to the scene graph.
	virtual void Initialise(void) {};

    // Shutdown is called after the scenegraph has been shutdown. It is where you destroy the renderer and destroy
    // your scene graph nodes
	virtual void Shutdown(void) {};

    // Update is called once each frame before the scene graph is updated. This is where you make any calls to your scene graph
    // nodes to change their behaviour
	virtual void Update(void) {}; 

    // PreRender is called before the scene graph is rendered each frame.
	virtual void PreRender(void) {};

    // PostRender is called after the scene graph is rendered each frame
	virtual void PostRender(void) {}

    // WindowCaption is called before the window is created. It gives you a chance to specify your own 
    // caption for the window
	virtual wstring WindowCaption(void);

    // WindowDimensions is called before the window is created.  It gives you a chance to override the default 
    // window size (640 x 480)
	virtual RECT WindowDimensions(void);

    // FramesPerSecond determines how often the Update/Render cycle is done. The default is 25 times per second.
	virtual unsigned int FramesPerSecond(void);

	virtual bool HandleKeyboardMessage(WPARAM wParam, LPARAM lParam) {return false;};

	// Accessors and mutators

    // Retrieves the Direct311 Device interface being used
	ID3D11Device	    * Device(void);

    // Retrieves the Direct311 DeviceContext interface being used
    ID3D11DeviceContext * DeviceContext(void);

    // TODO  Once you have implemented the SceneGraph and Renderer classes, you can uncomment out these lines
    SceneGraph *			GetSceneGraph(void);
	Renderer            *	GetCurrentRenderer(void);
	void					SetCurrentRenderer(Renderer * renderer);
	ResourcesManager *		GetResourcesManager(void);
	Camera *				GetCamera(void);

    // Public method that is called from the main Windows entry point. It is not intended to be
    // called directly
	int Run(HINSTANCE hInstance);

    // The class's message handler. Not intended to be called directly
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	

private:
	// Windows variables
	HINSTANCE				_hInstance;
	HWND					_hWindow;

	// Used in timing loop
	double					_timeSpan;

	// Direct3D 11 interfaces
	ID3D11Device *			_device;
	ID3D11DeviceContext *	_deviceContext;
	IDXGISwapChain *		_swapChain;
	ID3D11RenderTargetView *_renderTarget;
	ID3D11DepthStencilView *_zBuffer;

	SceneGraph *			_sceneGraph;
	Renderer *				_renderer;
	ResourcesManager *		_resourcesManager;
	GamePadController *		 _gamePadController;
	Camera *				_camera;

	// Private methods
	bool Startup(HINSTANCE hInstance);
	int MainLoop(void);

	bool InitialiseMainWindow(void);
	bool InitialiseDirect3D(void);
	bool SetRenderTargetsAndViewport(unsigned int width, unsigned int height);
	void UpdateAll(void);
	void RenderAll(void);
	void ShutdownAll(void);
};

