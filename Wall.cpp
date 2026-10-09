#include "Wall.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

using namespace sf;

Wall::Wall(int screenHeight, int screenWidth)
	: height(300),
	  width(50),
	  maxPosition(100),
	  minPosition(screenHeight - 300 - 100),
	  startLocation(screenWidth + 300),
	  speed(-1000.0f) {
	createNewWall(screenHeight, screenWidth);
}

Wall::Wall(int Height, int Width, int Bound, float Speed, int screenHeight, int screenWidth)
	: height(Height),
	  width(Width),
	  maxPosition(screenHeight - Height - Bound),
	  minPosition(Bound),
	  startLocation(screenWidth + 300),
	  speed(-Speed) {

	createNewWall(screenHeight, screenWidth);
	
}

void Wall::createNewWall(int screenHeight, int screenWidth) {
	(void)screenHeight;
	(void)screenWidth;

	RectangleShape Wall(Vector2f(static_cast<float>(width), static_cast<float>(height)));

	wallObject = Wall;

	wallObject.setOutlineColor(Color::White);
	wallObject.setOutlineThickness(2);
	wallObject.setFillColor(Color::Black);

	reset();
}

int Wall::getHeight() const {
	return height;
}

int Wall::getWidth() const {
	return width;
}

int Wall::getLocation() const {
	return static_cast<int>(wallObject.getPosition().x);
}

float Wall::getSpeed() const {
	return speed;
}

void Wall::move(float DeltaTime) {
	wallObject.move((speed * DeltaTime), 0);
}

const RectangleShape& Wall::getWall() const {
	return wallObject;
}

void Wall::reset() {
	static std::random_device rd;
	static std::mt19937 generator(rd());

	std::uniform_int_distribution<int> distribution(maxPosition / 50, minPosition/50);

	int random_number = distribution(generator);

	wallObject.setPosition(static_cast<float>(startLocation), static_cast<float>(random_number * 50));
}
