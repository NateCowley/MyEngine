#pragma once

#include <Windows.h>
#include <string>

#include "Graphics.h"
#include "Input.h"
#include "Audio.h"
#include "EngineTools.h"
#include "Game.h"
#include "WindowListener.h"
#include "AudioPlayer.h"

/// <summary>
/// The engine. Handles window creation, input capture, and graphical output.
/// Requires the separate creation (and loading) of a Game object in order to 
/// do anything.
/// 
/// It functions primarily as a singleton-type object.
/// </summary>
class Engine
{
public:
	/// <summary>
	/// Retrieves the current Engine instance. If the instance has not yet 
	/// been created, it will create the instance before retrieval.
	/// </summary>
	static Engine* getInstance();
	~Engine();

	/// <summary>
	/// Creates the window, graphics, and input objects. Must be called before 
	/// prior to anything else. Cannot be called multiple times.
	/// </summary>
	bool Init(HINSTANCE hInstance, int nCmdShow, int width, int height);

	/// <summary>
	/// Shuts down and cleans up the window, graphics, and input objects. 
	/// The function Stop() must be called before Shutdown() in order to avoid 
	/// accidental NullPointerExceptions.
	/// </summary>
	void ShutDown();

	/// <summary>
	/// Retrieves the HWND of the current window.
	/// </summary>
	HWND getHandle();

	/// <summary>
	/// Retrieves the HINSTANCE of the current window.
	/// </summary>
	HINSTANCE getHINSTANCE();

	/// <summary>
	/// Retrieves the WNDCLASSEX of the current window.
	/// </summary>
	WNDCLASSEX getWindowClass();

	/// <summary>
	/// Retrieves the current dimensions of the client RECT
	/// </summary>
	RECT getClientDimensions();

	/// <summary>
	/// Retrieves the width of the client.
	/// </summary>
	int getClientWidth();

	/// <summary>
	/// Retrieves the height of the client.
	/// </summary>
	int getClientHeight();

	/// <summary>
	/// Retrieves the width of the window.
	/// </summary>
	int getWindowWidth();

	/// <summary>
	/// Retrieves the height of the window.
	/// </summary>
	int getWindowHeight();

	/// <summary>
	/// If a Game has been loaded, begin the game loop. 
	/// This is used to begin playing a Game.
	/// </summary>
	void Start();

	/// <summary>
	/// Used to exit out of the current game loop.
	/// </summary>
	void Stop();

	/// <summary>
	/// Retrieves the currently loaded game.
	/// </summary>
	Game* getCurrentGame();

	/// <summary>
	/// Load a game into the engine. Must be called prior to Start().
	/// </summary>
	void loadGame(Game* game);

private:
	static Engine* instance;
	Input* input;
	Graphics* graphics;
	Audio* audio;
	Game* currentGame;

	HWND windowHandle;
	HINSTANCE hInstance;
	WNDCLASSEX windowClass;
	int nCmdShow;
	LPCSTR windowTitle;

	const char* lpszClassName = "MainWindow";

	RECT screenRect;
	int clientWidth, clientHeight;
	bool beenInitialized = false;
	bool shuttingDown;

	/// <summary>
	/// Create the engine object if it hasn't already been created.
	/// </summary>
	Engine();

	/// <summary>
	/// Create the win32 window that the engine will run in.
	/// </summary>
	bool InitWindow();

	/// <summary>
	/// Create the Graphics object to be used for drawing to the screen.
	/// </summary>
	bool InitGraphics();

	/// <summary>
	/// Create the Input object that will capture input.
	/// </summary>
	bool InitInput();

	/// <summary>
	/// Create the Audio object that will handle audio input/output.
	/// </summary>
	bool InitAudio();
};