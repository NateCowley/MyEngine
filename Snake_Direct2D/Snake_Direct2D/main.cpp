//#include <Windows.h>
//#include <d2d1.h>
//#include <d3d11.h>

#include "Engine.h"
#include "SnakeGame.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow)
{
	Engine* engine = Engine::getInstance();

	if (!engine->Init(hInstance, nCmdShow, new SnakeGame()))
	{
		delete engine;
		return -1;
	}

	engine->Start();

	delete engine;

	return 0;
}