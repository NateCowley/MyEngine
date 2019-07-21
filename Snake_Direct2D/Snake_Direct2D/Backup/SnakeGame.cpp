#include "SnakeGame.h"

SnakeGame::SnakeGame()
{

}

SnakeGame::~SnakeGame()
{

}

void SnakeGame::Init()
{
	srand(time(NULL));

	EngineTools::GetClientResolution(clientWidth, clientHeight);

	//MessageBox(NULL, (std::to_string(clientWidth) + " " + std::to_string(clientHeight)).c_str(), "", MB_OK);

	gridWidth = clientWidth / ratio;
	gridHeight = clientHeight / ratio;

	int x = clientWidth / 2 - gridWidth;
	int y = clientHeight / 2 - gridHeight;

	while ((x % gridWidth) != 0)
	{
		x--;
	}

	while ((y % gridHeight) != 0)
	{
		y--;
	}

	snake = new Snake(x, y, gridWidth, gridHeight);

	newFoodPoint();
}

void SnakeGame::Tick(long deltaTime)
{
	ticks += deltaTime;

	if (ticks > frameRate)
	{
		snake->tick(deltaTime);

		if (snake->isColliding(food.left, food.top))
		{
			snake->addSnakeBit();

			newFoodPoint();
		}

		ticks = 0;
	}
}

void SnakeGame::Draw(Graphics* g)
{
	g->ClearScreen(Color::BLACK);

	g->DrawFillRect(food.left, food.top, gridWidth, gridHeight, Color::WHITE);

	snake->draw(g);
}

void SnakeGame::Shutdown()
{
	if (snake)
	{
		delete snake;
	}
}

void SnakeGame::newFoodPoint()
{
	int x = rand() % clientWidth;
	int y = rand() % clientHeight;

	while ((x % gridWidth) != 0)
	{
		x--;
	}

	while ((y % gridHeight) != 0)
	{
		y--;
	}

	while (snake->isColliding(x, y))
	{
		x = rand() % clientWidth;
		y = rand() % clientHeight;

		while (x % gridWidth != 0)
		{
			x--;
		}

		while (y % gridHeight != 0)
		{
			y--;
		}
	}

	food.left = x;
	food.top = y;
	food.right = x + gridWidth;
	food.bottom = y + gridHeight;
}