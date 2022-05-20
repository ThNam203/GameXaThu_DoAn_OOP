#include "NonBulletUnit.h"

class Hop : public NonBulletUnit
{
public:
	sf::Texture& getTexture();
	Hop();
	~Hop() {};
};