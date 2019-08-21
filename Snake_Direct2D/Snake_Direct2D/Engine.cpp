#include "Engine.h"

Engine* Engine::instance = 0;

Engine::Engine()
{
	currentGame = NULL;
}

Engine::~Engine()
{
	ShutDown();
}

Engine* Engine::getInstance()
{
	if (!instance)
	{
		instance = new Engine();
	}

	return instance;
}

bool Engine::Init(HINSTANCE hInstance, int nCmdShow, Game* game, int width, int height)
{
	if (beenInitialized)
	{
		return true;
	}

	loadGame(game);

	if (hInstance == NULL || width <= 0 || height <= 0)
	{
		return false;
	}

	if (instance == 0)
	{
		instance = new Engine();
	}

	if (!InitWindow(hInstance, nCmdShow, width, height))
	{
		return false;
	}

	if (!InitGraphics())
	{
		return false;
	}

	ShowWindow(window->getHandle(), nCmdShow);

	if (!InitInput())
	{
		return false;
	}

	if (!InitAudio())
	{
		return false;
	}

	beenInitialized = true;

	return true;
}

bool Engine::InitWindow(HINSTANCE hInstance, int nCmdShow, int clientWidth, int clientHeight, std::string title)
{
	if (!window)
	{
		window = new ME_Window();
	}

	window->addWindowListener(this);

	return window->Init(hInstance, nCmdShow, clientWidth, clientHeight, title);
}

bool Engine::InitGraphics()
{
	graphics = Graphics::getInstance();

	if (!graphics->Init(window->getHandle()))
	{
		return false;
	}

	return true;
}

bool Engine::InitInput()
{
	input = Input::getInstance();

	if (!input->Init(window->getHINSTANCE(), window->getHandle(), window->getClientWidth(), window->getClientHeight()))
	{
		return false;
	}

	return true;
}

bool Engine::InitAudio()
{
	audio = Audio::getInstance();

	if (!audio->Init())
	{
		return false;
	}

	return true;
}

void Engine::ShutDown()
{
	shuttingDown = true;

	if (currentGame)
	{
		currentGame->Shutdown();
		delete currentGame;
	}

	if (window)
	{
		window->ShutDown();
		delete window;
	}

	if (graphics)
	{
		graphics->Shutdown();
		delete graphics;
	}

	if (input)
	{
		input->Shutdown();
		delete input;
	}

	if (audio)
	{
		audio->Shutdown();
		delete audio;
	}
}

void Engine::Start()
{
	if (currentGame == NULL)
	{
		return;
	}

	currentGame->Init();

	shuttingDown = false;

	MSG message;
	// set the message to anything other than quit, so we can go into the while loop
	message.message = WM_NULL;

	SYSTEMTIME stime;

	GetSystemTime(&stime);

	long current = 0;
	long elapsed = 0;
	long lastTick = stime.wMilliseconds;

	while (message.message != WM_QUIT && !shuttingDown)
	{
		// check if there's a message
		if (PeekMessage(&message, window->getHandle(), 0, 0, PM_REMOVE))
		{
			// if there's a message, dispatch to WindowProc
			DispatchMessage(&message);
		}
		else
		{
			GetSystemTime(&stime);

			current = stime.wMilliseconds;

			elapsed = abs(current - lastTick);

			// UPDATE
			input->Frame();

			currentGame->Tick(elapsed);

			// START RENDER
			graphics->BeginDraw();
			
			currentGame->Draw(graphics);
			// END RENDER
			graphics->EndDraw();

			lastTick = current;
		}
	}

	MessageBox(NULL, "", "", MB_OK);
}

void Engine::Stop()
{
	shuttingDown = true;
}

Game* Engine::getCurrentGame()
{
	return currentGame;
}

void Engine::loadGame(Game* game)
{
	currentGame = game;
}

void Engine::Resized()
{

}

void Engine::Minimized()
{

}

void Engine::Maximized()
{

}

void Engine::Closing()
{
	shuttingDown = true;
}