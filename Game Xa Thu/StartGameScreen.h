#pragma once

#include "SFML/Graphics.hpp"
#include "GameProperties.h"

class StartGameScreen {
public:
	sf::RectangleShape start_button;

	sf::Texture start_button_texture;
	sf::Texture start_game_background_texture;

	sf::Sprite start_game_background_sprite;
	sf::Sprite start_button_sprite;

	StartGameScreen();
	bool isGameStartButtonPressed(sf::RenderWindow& window);
};