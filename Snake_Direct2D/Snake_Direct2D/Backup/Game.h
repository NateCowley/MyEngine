#pragma once

class Graphics;

// Game interface
class Game
{
public:
	virtual void Init() = 0;
	virtual void Tick(long deltaTime) = 0;
	virtual void Draw(Graphics* g) = 0;
	virtual void Shutdown() = 0;
};