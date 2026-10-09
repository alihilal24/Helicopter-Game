#pragma once
#include <SFML/Graphics.hpp>
class Wall
{
public:
	Wall(int screenHeight, int screenWidth);
	Wall(int Height, int Width, int Bound, float Speed, int screenHeight, int screenWidth);
	int getHeight();
	int getWidth();
	int getLocation();
	int getSpeed();
	void move(float DeltaTime);
	sf::RectangleShape getWall();
	void reset();

private:
	int height;
	int width;
	int maxPosition;
	int minPosition;
	int startLocation;
	float speed;
	sf::RectangleShape wallObject;
	void createNewWall(int screenHeight, int screenWidth);
};

