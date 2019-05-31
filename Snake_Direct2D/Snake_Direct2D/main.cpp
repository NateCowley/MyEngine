//#include <Windows.h>
//#include <d2d1.h>
//#include <d3d11.h>

#include "Engine.h"
#include "SnakeGame.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPWSTR cmd, int nCmdShow)
{
	Engine* engine = Engine::getInstance();

	if (!engine->Init(hInstance, nCmdShow, 800, 800))
	{
		delete engine;
		return -1;
	}

	SnakeGame* sg = new SnakeGame();

	engine->loadGame(sg);

	engine->Start();

	delete engine;
	// sg is not deleted because the engine deletes it

	return 0;
}