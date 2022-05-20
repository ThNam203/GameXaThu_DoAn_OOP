#ifndef UNIT_HEADER
#define UNIT_HEADER

#include <iostream>
#include <SFML/Graphics.hpp>
#include "../Texture.h"

using namespace std;

class Unit
{
protected:
	int damage;
	int blood;
	int armor;
	string show;
	Texture texture;
public:
	Unit();
	~Unit() {};
};
#endif