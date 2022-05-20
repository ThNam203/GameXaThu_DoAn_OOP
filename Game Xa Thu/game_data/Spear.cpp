#include "Spear.h"

Spear::Spear()
{
	damage = 75;
	blood = 85;
	armor = 85;
	show = "--->";
}

sf::Texture& Spear::getTexture() {
	return this->texture.spear_texture;
}