#include "Hop.h"

Hop::Hop()
{
	damage = 70;
	blood = 80;
	armor = 80;
	show = "--)>";
}

sf::Texture& Hop::getTexture() {
	return this->texture.hop_texture;
}