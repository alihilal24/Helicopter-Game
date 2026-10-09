#include "Wall.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>

using namespace sf;

Wall::Wall(int screenHeight, int screenWidth) {
	height = 300;
	width = 50;
	maxPosition = 100;
	minPosition = screenHeight - height - 100;
	startLocation = screenWidth + 300;
	speed = -1000;
	
	createNewWall(screenHeight, screenWidth);
}

Wall::Wall(int Height, int Width, int Bound, float Speed, int screenHeight, int screenWidth) {
	height = Height;
	width = Width;
	maxPosition = screenHeight - height - Bound;
	minPosition = Bound;
	startLocation = screenWidth + 300;
	speed = Speed * -1;

	createNewWall(screenHeight, screenWidth);
	
}

void Wall::createNewWall(int screenHeight, int screenWidth) {

	RectangleShape Wall(Vector2f(width, height));

	wallObject = Wall;
	//wallObject.setOrigin(Vector2f(height / 2, width / 2));

	//Hitbox Debugging
	wallObject.setOutlineColor(Color::White);
	wallObject.setOutlineThickness(2);
	wallObject.setFillColor(Color::Black);

	reset();
}

int Wall::getSpeed() {
	return speed;
}

void Wall::move(float DeltaTime) {
	wallObject.move((speed * DeltaTime), 0);
	//std::cout << (int)wallObject.getPosition().x << " " << speed * DeltaTime << std::endl;
}

RectangleShape Wall::getWall() {
	return wallObject;
}

void Wall::reset() {
	std::random_device rd;
	std::mt19937 generator(rd());

	std::uniform_int_distribution<int> distribution(maxPosition / 50, minPosition/50);

	int random_number = distribution(generator);


	wallObject.setPosition(startLocation, random_number*50);
}
