#include "NonBulletUnit.h"

class Spear : public NonBulletUnit
{
public:
	sf::Texture& getTexture();
	Spear();
	~Spear() {};
};

