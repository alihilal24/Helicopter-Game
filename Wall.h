#pragma once
#include <SFML/Graphics.hpp>
class Wall
{
public:
	Wall(int screenHeight, int screenWidth);
	Wall(int Height, int Width, int Bound, float Speed, int screenHeight, int screenWidth);
	int getHeight() const;
	int getWidth() const;
	int getLocation() const;
	float getSpeed() const;
	void move(float DeltaTime);
	const sf::RectangleShape& getWall() const;
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

