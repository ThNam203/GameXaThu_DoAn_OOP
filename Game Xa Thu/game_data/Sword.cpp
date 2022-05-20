#include "Sword.h"

Sword::Sword()
{
	damage = 85;
	blood = 80;
	armor = 80;
	show = "=|--";
}

sf::Texture& Sword::getTexture() {
	return texture.swordman_texture;
}
