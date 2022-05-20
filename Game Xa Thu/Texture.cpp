#include <SFML/Graphics.hpp>
#include "GameProperties.h"
#include <iostream>
#include "Texture.h"

Texture::Texture() {
	// TEXTURE
	if (!swordman_texture.loadFromFile("image/swordman.png")) std::cout << "Loaded swordman texture failed" << std::endl;
	if (!gunner_texture.loadFromFile("image/gunner.png")) std::cout << "Loaded gunner texture failed" << std::endl;
	if (!hop_texture.loadFromFile("image/hop.png")) std::cout << "Loaded hop texture failed" << std::endl;
	if (!spear_texture.loadFromFile("image/spear.png")) std::cout << "Loaded spear texture failed" << std::endl;
	if (!background_texture.loadFromFile("image/background.png")) std::cout << "Loaded background texture failed" << std::endl;
	if (!game_icon_image.loadFromFile("image/game_icon.png")) std::cout << "Loaded game icon image failed" << std::endl;

	// SPRITE
	background_sprite.setTexture(background_texture);

	// INIT SOME PROPERTIES
	background_sprite.setScale(sf::Vector2f(2 * WINDOW_WIDTH / background_sprite.getLocalBounds().width, 2 * WINDOW_HEIGHT / background_sprite.getLocalBounds().height));
	background_sprite.setPosition(sf::Vector2f(-WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2));
}
