#include "Collision.h"
#include "Wall.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
using namespace sf;

const int SCREEN_HEIGHT = 1000;
const int SCREEN_WIDTH = 1500;
const float MAX_VERTICAL_SPEED = -400;
const float MIN_VERTICAL_SPEED = 400;
const String HELICOPTER_IMAGE_FILENAME = "Helicopter.png";
const String WALL_IMAGE_FILENAME = "Wall.png";
float currentVerticalSpeed = 0;
Clock heliGameClock;
Clock wallGameClock;

/*
	Using this multiplied by currentVerticalSpeed results in consistant helicopter movement speed regardless of
	framerate changes
*/
float HeliDeltaTime() {
	return heliGameClock.restart().asSeconds();
}
float WallDeltaTime() {
	return wallGameClock.restart().asSeconds();
}

/*
	When the helicopter moves up or down, slowly increment or decrement the current speed until
	max or min vertspeed is reached, then let helicopter continue to move at that speed until bound is reached or
	W / UP button is pressed / released.	
*/
void gravityEffect(bool falling) {

	if (falling && currentVerticalSpeed < MIN_VERTICAL_SPEED) {
		currentVerticalSpeed += ((1.0015 * std::abs(currentVerticalSpeed)) + 1) - std::abs(currentVerticalSpeed);
	}
	else if (!falling && currentVerticalSpeed > MAX_VERTICAL_SPEED){
		currentVerticalSpeed += ((-1.0015 * std::abs(currentVerticalSpeed)) - 1) + std::abs(currentVerticalSpeed);
	}
}


int main() {
	
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Helicopter Game");
	Event e;
	window.setFramerateLimit(1000);

	/*
		Dimensions of Helicopter.png is 800:332
		I divided it by 4 to get an appropriate size --> 200:83
		
		Creating helicpoter sprite with rectangle shape and Helicopter.png texture
	*/
	//Vector2f(200,83)
	Texture heliTexture;
	Collision::createTextureAndBitmask(heliTexture, HELICOPTER_IMAGE_FILENAME);
	heliTexture.setSmooth(true);
	Sprite heli(heliTexture);

	heli.setPosition(200, 500);
	heli.setOrigin(100,41.5);
	
	heli.setScale(200, 83);


	bool gameStarted = false;
	bool crashed = false;

	Wall w(SCREEN_HEIGHT, SCREEN_WIDTH);
	//GAME LOOP

	while (window.isOpen()) {
		
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();
		}
		
		//If W or Up Key is pressed, move helicopter up and start the game
		if (Keyboard::isKeyPressed(Keyboard::Key::W) || Keyboard::isKeyPressed(Keyboard::Key::Up))
		{
	
			//Forces restart for gameclock when game starts for accurate DeltaTime
			if (!gameStarted) {
				HeliDeltaTime();
				WallDeltaTime();
			}

			gravityEffect(false);
			heli.move(0, (currentVerticalSpeed*HeliDeltaTime()));
			heli.setRotation((currentVerticalSpeed/(MIN_VERTICAL_SPEED*2)) * 30);
			gameStarted = true;

		}
		//When W or Up Key is released, helicopter moves downwards
		else if (gameStarted)
		{
			gravityEffect(true);
			heli.move(0, (currentVerticalSpeed * HeliDeltaTime()));
			heli.setRotation((currentVerticalSpeed / (MIN_VERTICAL_SPEED * 2)) * 30);
		}
		
		if (gameStarted)
		{
			w.move(WallDeltaTime());
			if (w.getWall().getPosition().x < -10)
			{
				w.reset();
			}
			
			

			if (Collision::pixelPerfectTest(heli, w));
				crashed = true;
			

		}
		
		
		
		
		//When ceiling / floor is hit, helicopter has crashed and game ends
		if ((int)heli.getPosition().y < 50 || (int)heli.getPosition().y > SCREEN_HEIGHT - 50)
			crashed = true;
		
		//If player crashed, game stops until play again button (Return Key) is pressed
		//Helicopter returns to start position and restarts the game 
		while (crashed) {
			if (Keyboard::isKeyPressed(Keyboard::Key::Return))
			{
				gameStarted = false;
				heli.setPosition(200, 500);
				heli.setRotation(0);
				currentVerticalSpeed = 0;
				w.reset();
				crashed = false;
			}
		}
		
		//Debugging
//		std::cout << (int)heli.getPosition().y << " " << currentVerticalSpeed << std::endl;
		std::cout << (int)w.getWall().getPosition().x << " " << heli.getSize().x << std::endl;

		window.clear();
		window.draw(heli);
		window.draw(w.getWall());
		window.display();

			
	}

	return 0;
}