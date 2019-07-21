#pragma once

#include <time.h>
#include <stdlib.h>

#include "Game.h"
#include "DrawableObject.h"
#include "Snake.h"
#include "EngineTools.h"

class SnakeGame : public Game
{
public:
	int clientWidth, clientHeight;
	int gridWidth, gridHeight;
	int ratio = 20;

	SnakeGame();
	~SnakeGame();

	virtual void Init();
	virtual void Tick(long deltaTime);
	virtual void Draw(Graphics* g);
	virtual void Shutdown();

private:
	Snake* snake;
	RECT food;
	long ticks = 0;
	long frameRate = 125;

	void newFoodPoint();
};