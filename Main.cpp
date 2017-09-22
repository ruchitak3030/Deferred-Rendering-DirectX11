
#include <Windows.h>
#include "Game.h"




int WINAPI WinMain(
	HINSTANCE hInstance,		// The handle to this app's instance
	HINSTANCE hPrevInstance,	// A handle to the previous instance of the app (always NULL)
	LPSTR lpCmdLine,			// Command line params
	int nCmdShow)				// How the window should be shown (we ignore this)
{
#if defined(DEBUG) | defined(_DEBUG)

	
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	
	Game dxGame(hInstance);

	HRESULT hr = S_OK;

	hr = dxGame.InitWindow();
	if(FAILED(hr)) return hr;

	hr = dxGame.InitDirectX();
	if(FAILED(hr)) return hr;

	
	return dxGame.Run();
}
