#include "Engine.h"

Engine* Engine::instance = 0;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
	{
		// call Stop to break out of the loop that was started in Start()
		Engine::getInstance()->Stop();
		DestroyWindow(hwnd);
		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	case WM_SIZING:
	{
		MessageBox(NULL, "resizing", "", MB_OK);
		return 0;
	}
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Engine::Engine()
{
	currentGame = NULL;
}

Engine::~Engine()
{
	ShutDown();

	UnregisterClass(windowTitle, hInstance);
}

Engine* Engine::getInstance()
{
	if (!instance)
	{
		instance = new Engine();
	}

	return instance;
}

bool Engine::Init(HINSTANCE hInstance, int nCmdShow, int width, int height)
{
	if (beenInitialized)
	{
		return true;
	}

	if (hInstance == NULL || width <= 0 || height <= 0)
	{
		return false;
	}

	if (instance == 0)
	{
		instance = new Engine();
	}

	this->hInstance = hInstance;
	this->nCmdShow = nCmdShow;
	clientWidth = width;
	clientHeight = height;
	this->windowTitle = "Snake";

	if (!InitWindow())
	{
		return false;
	}

	if (!InitGraphics())
	{
		return false;
	}

	ShowWindow(windowHandle, nCmdShow);

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

bool Engine::InitWindow()
{
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.hInstance = hInstance;
	windowClass.lpfnWndProc = WindowProc;
	windowClass.lpszClassName = lpszClassName;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;

	RegisterClassEx(&windowClass);

	int screenWidth, screenHeight;

	if (!EngineTools::GetDesktopResolution(screenWidth, screenHeight))
	{
		return false;
	}

	RECT wnd = { 0, 0, 800, 800 };
	RECT dt;

	GetClientRect(GetDesktopWindow(), &dt);

	AdjustWindowRect(&wnd, WS_OVERLAPPEDWINDOW, FALSE);

	int w = (wnd.right - wnd.left);
	int h = (wnd.bottom - wnd.top);
	int x = ((dt.right - dt.left) - w) / 2;
	int y = ((dt.bottom - dt.top) - h) / 2;

	windowHandle = CreateWindow(lpszClassName, windowTitle, WS_OVERLAPPEDWINDOW, x, y, w, h, NULL, NULL, hInstance, NULL);

	if (!windowHandle)
	{
		return false;
	}

	return true;
}

bool Engine::InitGraphics()
{
	graphics = Graphics::getInstance();

	if (!graphics->Init(windowHandle))
	{
		return false;
	}

	return true;
}

bool Engine::InitInput()
{
	input = Input::getInstance();

	if (!input->Init(hInstance, windowHandle, clientWidth, clientHeight))
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

HWND Engine::getHandle()
{
	return windowHandle;
}

HINSTANCE Engine::getHINSTANCE()
{
	return hInstance;
}

WNDCLASSEX Engine::getWindowClass()
{
	return windowClass;
}

RECT Engine::getClientDimensions()
{
	return screenRect;
}

int Engine::getClientWidth()
{
	return clientWidth;
}

int Engine::getClientHeight()
{
	return clientHeight;
}

int Engine::getWindowWidth()
{
	// TODO: get size of full window, includeing outline/toolbar
	return clientWidth;
}

int Engine::getWindowHeight()
{
	// TODO: get size of full window, includeing outline/toolbar
	return clientWidth;
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

	/*
	AudioComponent* ac = new AudioComponent((TCHAR*)"A:\\Downloads\\Sky Run.wav");
	AudioPlayer ap(ac);

	ap.setLoop(144, 10, 2);
	ap.startPlaying();
	*/
	/*
	if (FAILED(audio->OpenFile((TCHAR*)"A:\\Downloads\\Sky Run.wav")))
	{
		return;
	}
	*/

	while (message.message != WM_QUIT && !shuttingDown)
	{
		// check if there's a message
		if (PeekMessage(&message, windowHandle, 0, 0, PM_REMOVE))
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