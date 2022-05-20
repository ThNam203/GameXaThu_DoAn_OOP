#pragma once

class Texture {
public:
	sf::Texture swordman_texture;
	sf::Texture gunner_texture;
	sf::Texture hop_texture;
	sf::Texture spear_texture;
	sf::Texture background_texture;

	sf::Sprite background_sprite;
	sf::Sprite exit_game_sprite;

	sf::Image game_icon_image;

	Texture();
};