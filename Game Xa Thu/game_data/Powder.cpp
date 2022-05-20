#include "Powder.h"

Powder::Powder()
{
	damage = 100;
	blood = 80;
	armor = 80;
	munition = 100;
	show = "oooo";
}

sf::Texture& Powder::getTexture() {
	return this->texture.gunner_texture;
}