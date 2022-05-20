#include "StartGameScreen.h"
#include <iostream>

StartGameScreen::StartGameScreen() {
	if (!start_game_background_texture.loadFromFile("image/start_game_background.png")) std::cout << "Loaded start game background failed" << std::endl;
	if (!start_button_texture.loadFromFile("image/start_btn.png")) std::cout << "Loaded start button texture failed" << std::endl;

	this->start_button.setSize(sf::Vector2f(WINDOW_WIDTH / 4, WINDOW_HEIGHT / 3));
	this->start_button.setPosition(sf::Vector2f(WINDOW_WIDTH / 2 - WINDOW_WIDTH / 8, WINDOW_HEIGHT / 2 - WINDOW_HEIGHT / 6));
	this->start_button.setTexture(&this->start_button_texture);

	start_button_sprite.setTexture(start_button_texture);
	start_game_background_sprite.setTexture(start_game_background_texture);

	start_game_background_sprite.setScale(sf::Vector2f(WINDOW_WIDTH / start_game_background_sprite.getLocalBounds().width, WINDOW_HEIGHT / start_game_background_sprite.getLocalBounds().height));
	start_game_background_sprite.setPosition(sf::Vector2f(0, 0));
}

bool StartGameScreen::isGameStartButtonPressed(sf::RenderWindow& window) {
	if (this->start_button.getGlobalBounds().contains(sf::Vector2f(sf::Mouse::getPosition(window)))) return true;
	else return false;
}