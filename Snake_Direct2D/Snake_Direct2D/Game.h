#pragma once

class Graphics;

// Game interface

/// <summary>
/// Provides the basic controls for a game object. Should only be created and loaded into the Engine.
/// </summary>
class Game
{
public:
	virtual void Init() = 0;
	virtual void Tick(long deltaTime) = 0;
	virtual void Draw(Graphics* g) = 0;
	virtual void Shutdown() = 0;
};