#include "BulletUnit.h"

class Powder : public BulletUnit
{
public:
	sf::Texture& getTexture();
	Powder();
	~Powder() {};
};

