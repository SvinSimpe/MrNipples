#include "Application.h"

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow )
{
	int retVal = 0;

	Application* project = new Application();

	if( FAILED( retVal = project->InitializeWindow( hInstance, nCmdShow ) ) )
		return retVal;
	
	if( FAILED( retVal = project->InitializeDirectX11() ) )
		return retVal;


	retVal = project->Run();

	project->Release();

	delete project;

	return retVal;
}