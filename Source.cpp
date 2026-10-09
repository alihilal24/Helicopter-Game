#include "Collision.h"
#include "Wall.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <iostream>
#include <cmath>
#include <string>
using namespace sf;

const int SCREEN_HEIGHT = 1000;
const int SCREEN_WIDTH = 1500;
const float MAX_VERTICAL_SPEED = -475.0f;
const float MIN_VERTICAL_SPEED = 475.0f;
const String HELICOPTER_IMAGE_FILENAME = "Helicopter.png";
const float HELICOPTER_START_X = 200.0f;
const float HELICOPTER_START_Y = 500.0f;
const float PHYSICS_REFERENCE_FPS = 1000.0f;
const IntRect HELICOPTER_COLLISION_RECT(150, 90, 470, 170);
float currentVerticalSpeed = 0;
Clock heliGameClock;
Clock wallGameClock;

enum class GameState {
	StartMenu,
	Playing,
	GameOver
};

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

bool loadUIFont(Font& font) {
	const std::array<std::string, 2> fontPaths = {
		"C:\\Windows\\Fonts\\segoeui.ttf",
		"C:\\Windows\\Fonts\\arial.ttf"
	};

	for (const auto& fontPath : fontPaths) {
		if (font.loadFromFile(fontPath)) {
			return true;
		}
	}

	return false;
}

void centerText(Text& text, float x, float y) {
	const FloatRect bounds = text.getLocalBounds();
	text.setOrigin(bounds.left + bounds.width / 2.0f, bounds.top + bounds.height / 2.0f);
	text.setPosition(x, y);
}

void resetRun(Sprite& helicopter, Wall& wall) {
	helicopter.setPosition(HELICOPTER_START_X, HELICOPTER_START_Y);
	helicopter.setRotation(0.0f);
	currentVerticalSpeed = 0.0f;
	wall.reset();
	HeliDeltaTime();
	WallDeltaTime();
}

/*
	When the helicopter moves up or down, slowly increment or decrement the current speed until
	max or min vertspeed is reached, then let helicopter continue to move at that speed until bound is reached or
	W / UP button is pressed / released.	
*/
void gravityEffect(bool falling, float deltaTime) {
	const float frameScale = deltaTime * PHYSICS_REFERENCE_FPS;

	if (falling && currentVerticalSpeed < MIN_VERTICAL_SPEED) {
		currentVerticalSpeed += (((1.0015f * std::abs(currentVerticalSpeed)) + 1.0f) - std::abs(currentVerticalSpeed)) * frameScale;
	}
	else if (!falling && currentVerticalSpeed > MAX_VERTICAL_SPEED){
		currentVerticalSpeed += (((-1.0015f * std::abs(currentVerticalSpeed)) - 1.0f) + std::abs(currentVerticalSpeed)) * frameScale;
	}
}


int main() {
	
	RenderWindow window(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Helicopter Game");
	Event e;
	window.setFramerateLimit(144);

	/*
		Dimensions of Helicopter.png is 800:332
		I divided it by 4 to get an appropriate size --> 200:83
		
		Creating helicpoter sprite with rectangle shape and Helicopter.png texture
	*/
	//Vector2f(200,83)
	Texture heliTexture;
	if (!Collision::createTextureAndBitmask(heliTexture, HELICOPTER_IMAGE_FILENAME)) {
		std::cerr << "Failed to load " << HELICOPTER_IMAGE_FILENAME.toAnsiString() << std::endl;
		return 1;
	}
	heliTexture.setSmooth(true);
	Sprite heli(heliTexture);
	Sprite heliCollision(heliTexture, HELICOPTER_COLLISION_RECT);

	heli.setPosition(HELICOPTER_START_X, HELICOPTER_START_Y);
	auto heliTextureSize = heliTexture.getSize();
	heli.setOrigin(heliTextureSize.x / 2.0f, heliTextureSize.y / 2.0f);
	heli.setScale(200.0f / heliTextureSize.x, 83.0f / heliTextureSize.y);
	heliCollision.setOrigin(
		(heliTextureSize.x / 2.0f) - HELICOPTER_COLLISION_RECT.left,
		(heliTextureSize.y / 2.0f) - HELICOPTER_COLLISION_RECT.top);
	heliCollision.setScale(heli.getScale());

	Font uiFont;
	if (!loadUIFont(uiFont)) {
		std::cerr << "Failed to load a UI font from the Windows fonts directory." << std::endl;
		return 1;
	}

	Text titleText("Helicopter Game", uiFont, 76);
	titleText.setFillColor(Color::White);
	titleText.setStyle(Text::Bold);
	centerText(titleText, SCREEN_WIDTH / 2.0f, 180.0f);

	Text menuText("Fly as long as you can without hitting the wall, ceiling, or floor.", uiFont, 30);
	menuText.setFillColor(Color(230, 230, 230));
	centerText(menuText, SCREEN_WIDTH / 2.0f, 310.0f);

	Text controlsText("Controls: W or Up Arrow to thrust upward", uiFont, 30);
	controlsText.setFillColor(Color(230, 230, 230));
	centerText(controlsText, SCREEN_WIDTH / 2.0f, 360.0f);

	Text promptText("Press Space to start", uiFont, 34);
	promptText.setFillColor(Color(255, 215, 120));
	promptText.setStyle(Text::Bold);
	centerText(promptText, SCREEN_WIDTH / 2.0f, 470.0f);

	Text scoreText("Score: 0", uiFont, 32);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(30.0f, 20.0f);

	RectangleShape overlay(Vector2f(static_cast<float>(SCREEN_WIDTH), static_cast<float>(SCREEN_HEIGHT)));
	overlay.setFillColor(Color(0, 0, 0, 180));

	Text gameOverText("You crashed", uiFont, 68);
	gameOverText.setFillColor(Color::White);
	gameOverText.setStyle(Text::Bold);
	centerText(gameOverText, SCREEN_WIDTH / 2.0f, 250.0f);

	Text finalScoreText("Final Score: 0", uiFont, 36);
	finalScoreText.setFillColor(Color(240, 240, 240));
	centerText(finalScoreText, SCREEN_WIDTH / 2.0f, 360.0f);

	RectangleShape playAgainButton(Vector2f(320.0f, 90.0f));
	playAgainButton.setOrigin(playAgainButton.getSize().x / 2.0f, playAgainButton.getSize().y / 2.0f);
	playAgainButton.setPosition(SCREEN_WIDTH / 2.0f, 500.0f);
	playAgainButton.setFillColor(Color(245, 196, 67));
	playAgainButton.setOutlineColor(Color::White);
	playAgainButton.setOutlineThickness(3.0f);

	Text playAgainText("Play Again", uiFont, 34);
	playAgainText.setFillColor(Color::Black);
	playAgainText.setStyle(Text::Bold);
	centerText(playAgainText, SCREEN_WIDTH / 2.0f, 500.0f);

	Text replayHintText("Press Enter or click the button", uiFont, 26);
	replayHintText.setFillColor(Color(220, 220, 220));
	centerText(replayHintText, SCREEN_WIDTH / 2.0f, 600.0f);

	GameState gameState = GameState::StartMenu;
	Clock scoreClock;
	float score = 0.0f;
	float finalScore = 0.0f;

	Wall w(SCREEN_HEIGHT, SCREEN_WIDTH);
	//GAME LOOP

	while (window.isOpen()) {
		const Vector2f mousePosition = window.mapPixelToCoords(Mouse::getPosition(window));
		const bool hoverPlayAgain = playAgainButton.getGlobalBounds().contains(mousePosition);
		playAgainButton.setFillColor(hoverPlayAgain ? Color(255, 220, 110) : Color(245, 196, 67));
		
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (gameState == GameState::StartMenu && e.type == Event::KeyPressed && e.key.code == Keyboard::Space) {
				resetRun(heli, w);
				score = 0.0f;
				scoreClock.restart();
				gameState = GameState::Playing;
			}

			if (gameState == GameState::GameOver) {
				const bool playAgainRequestedByKey = e.type == Event::KeyPressed && e.key.code == Keyboard::Enter;
				const bool playAgainRequestedByMouse =
					e.type == Event::MouseButtonPressed &&
					e.mouseButton.button == Mouse::Left &&
					playAgainButton.getGlobalBounds().contains(mousePosition);

				if (playAgainRequestedByKey || playAgainRequestedByMouse) {
					resetRun(heli, w);
					score = 0.0f;
					scoreClock.restart();
					gameState = GameState::Playing;
				}
			}
		}
		
		if (gameState == GameState::Playing) {
			const float helicopterDeltaTime = HeliDeltaTime();
			heliCollision.setPosition(heli.getPosition());
			heliCollision.setRotation(heli.getRotation());

			//If W or Up Key is pressed, move helicopter up and start the game
			if (Keyboard::isKeyPressed(Keyboard::Key::W) || Keyboard::isKeyPressed(Keyboard::Key::Up))
			{
				gravityEffect(false, helicopterDeltaTime);
				heli.move(0, currentVerticalSpeed * helicopterDeltaTime);
				heli.setRotation((currentVerticalSpeed / (MIN_VERTICAL_SPEED * 2.0f)) * 30.0f);

			}
			//When W or Up Key is released, helicopter moves downwards
			else
			{
				gravityEffect(true, helicopterDeltaTime);
				heli.move(0, currentVerticalSpeed * helicopterDeltaTime);
				heli.setRotation((currentVerticalSpeed / (MIN_VERTICAL_SPEED * 2.0f)) * 30.0f);
			}

			heliCollision.setPosition(heli.getPosition());
			heliCollision.setRotation(heli.getRotation());

			w.move(WallDeltaTime());
			if (w.getWall().getPosition().x + w.getWall().getSize().x < 0.0f)
			{
				w.reset();
			}

			if (heliCollision.getGlobalBounds().intersects(w.getWall().getGlobalBounds())) {
				finalScore = score;
				gameState = GameState::GameOver;
			}

			score = scoreClock.getElapsedTime().asSeconds() * 10.0f;
		}
		
		//When ceiling / floor is hit, helicopter has crashed and game ends
		if (gameState == GameState::Playing &&
			(heli.getPosition().y < 50.0f || heli.getPosition().y > SCREEN_HEIGHT - 50.0f)) {
			finalScore = score;
			gameState = GameState::GameOver;
		}

		scoreText.setString("Score: " + std::to_string(static_cast<int>(score)));
		finalScoreText.setString("Final Score: " + std::to_string(static_cast<int>(finalScore)));
		centerText(finalScoreText, SCREEN_WIDTH / 2.0f, 360.0f);

		window.clear(Color(18, 24, 38));

		if (gameState == GameState::StartMenu) {
			window.draw(titleText);
			window.draw(menuText);
			window.draw(controlsText);
			window.draw(promptText);
		}
		else {
			window.draw(heli);
			window.draw(w.getWall());
			window.draw(scoreText);

			if (gameState == GameState::GameOver) {
				window.draw(overlay);
				window.draw(gameOverText);
				window.draw(finalScoreText);
				window.draw(playAgainButton);
				window.draw(playAgainText);
				window.draw(replayHintText);
			}
		}

		window.display();

			
	}

	return 0;
}