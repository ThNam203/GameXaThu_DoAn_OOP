#include "NonBulletUnit.h"

class Sword : public NonBulletUnit
{
public:
	sf::Texture& getTexture();
	Sword();
	~Sword() {};
};

