#include "Interactive3D.h"


static const int BOUNDING_BOX = 1;
static const int BOUNDING_SPHERE = 2;

Interactive3D app;

// Sets the window caption
wstring Interactive3D::WindowCaption(void)
{
	return L"Interactive3D Example";
}

// Sets the size of the window
RECT Interactive3D::WindowDimensions(void)
{
	RECT rectangle = {0, 0, 800, 600};
	return rectangle;
}

// Initialises the program
void Interactive3D::Initialise(void)
{
	// Creates a new renderer
	Renderer * renderer = new Renderer();
	// Initalises the renderer
	renderer->Initialise();
	// Set up the camera of the renderer
	//renderer->SetEyePosition(0, 4000, 1);
	//renderer->SetEyePosition(0, 300, -2000);
	//renderer->SetFocalPointPosition(0,0,0);
	//renderer->SetUpVector(0,1,0);
	//renderer->SetViewTransformation();
	// Set the lighting of the scene
	//renderer->SetAmbientLight(0.44f, 0.57f, 0.74f, 1.0f);
	renderer->SetAmbientLight(0.75f, 0.75f, 0.75f, 1.0f);
	//renderer->SetAmbientLight(0.25f, 0.25f, 0.25f, 1.0f);
	renderer->SetDirectionalLightColour(0.75f, 0.75f, 0.75f, 1.0f);
	//renderer->SetDirectionalLightColour(1.0f, 1.0f, 1.0f, 1.0f);
	//renderer->SetDirectionalLightVector(0.0f, 1.0f, 0.0f);
	renderer->SetDirectionalLightVector(1.0f, 0.0f, 1.0f);
	// Set the projection transformation of the renderer
	renderer->SetProjectionTransformation(XM_PIDIV4, 1.0f, 1.0f, 10000.0f);
	// Sets the frameworks renderer to the renderer just initalised
	Framework::SetCurrentRenderer(renderer);

	////// Add objects to the Scene Graph ///////////

	//this->GetSceneGraph()->Add(new TexturedCube(L"cube"));
	//this->GetSceneGraph()->Add(new BiPlaneNode(L"BiPlane", "airplane.x"));
	this->GetSceneGraph()->Add(new TerrainNode(L"Terrain", "terrain1.raw", L"Terrain1.bmp", 256, 256, 5));
	this->GetSceneGraph()->Add(new SkyBox(L"SkyBox"));
	this->GetSceneGraph()->Add(new WoodenCube(L"Cube1", 0.0f, 200.0f, 0.0f, true));	
	this->GetSceneGraph()->Add(new BiPlaneNode(L"BiPlane1", "airplane.x", BOUNDING_BOX, 0.0f, 300.0f, 0.0f, 0.5f));
	this->GetSceneGraph()->Add(new BiPlaneNode(L"BiPlane2", "airplane.x", BOUNDING_BOX, 100.0f, 300.0f, 0.0f, 0.0f));
	this->GetSceneGraph()->Add(new BiPlaneNode(L"BiPlane3", "airplane.x", BOUNDING_SPHERE, 200.0f, 300.0f, 0.0f, 0.0f));
	this->GetSceneGraph()->Add(new BiPlaneNode(L"BiPlane4", "airplane.x", BOUNDING_SPHERE, 300.0f, 300.0f, 0.0f, 0.0f));
	//this->GetSceneGraph()->Add(new BiPlaneNode(L"BiPlane", "airplane.x", 300.0f, 300.0f, 0.0f, 0.0f, Framework::BOUNDING_SPHERE));
}

bool Interactive3D::HandleKeyboardMessage(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
		case 0x57:
			this->GetCamera()->SetForawrdBack(50);
			break;

		case 0x53:
			this->GetCamera()->SetForawrdBack(-50);
			break;

			case 0x41:
			this->GetCamera()->SetLeftRight(-50);
			break;

		case 0x44:
			this->GetCamera()->SetLeftRight(50);
			break;

		case VK_UP:
			this->GetCamera()->SetPitch(-0.1f);
			break;

		case VK_DOWN:
			this->GetCamera()->SetPitch(0.1f);
			break;

		case VK_LEFT:
			this->GetCamera()->SetYaw(-0.1f);
			break;

		case VK_RIGHT:
			this->GetCamera()->SetYaw(0.1f);
			break;

		default:
			break;
	}
	return true;
}
