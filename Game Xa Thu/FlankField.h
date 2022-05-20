#pragma once

#include <SFML/Graphics.hpp>
#include "GameProperties.h"
#include "MainField.h"
#include "Texture.h"

class FlankField {
private:
	int max_unit = MAX_FLANK_SLOT * MAX_FLANK_UNIT_PER_SLOT;
	double unit_width;
	Texture texture;
	sf::RectangleShape rectangle_unit_shape[MAX_FLANK_SLOT][MAX_FLANK_UNIT_PER_SLOT];
	sf::RectangleShape interact_box[MAX_FLANK_SLOT];
	double start_x_axis;
	double start_y_axis = MainField::start_y_axis + Y_DISTANCE_BETWEEN_FIELD + LINES_PER_SLOT_FLANK * UNIT_HEIGHT + Y_DISTANCE_BETWEEN_UNIT * (LINES_PER_SLOT_FLANK - 1);

public:
	int swordDamage = 85;
	int swordBlood = 80;
	int swordArmor = 80;
	int spearDamage = 75;
	int spearBlood = 85;
	int spearArmor = 85;
	int typeOfUnit[MAX_FLANK_SLOT]; // 1 is swordman, 2 is spear
	int unitNumberInEachSlot[MAX_FLANK_SLOT];

	FlankField(double start_x_axis, double unit_width, sf::Texture& texture);
	void setFieldTexture();
	void drawFlankField(sf::RenderWindow& window);
	void reset(); // start game again
	int getTypeOfUnitAtIndex(int index);
	int getNumberOfUnitAtIndex(int index);
	void setupInteractBox();
	sf::Vector2f getFirstMiddleUnitPos();
	sf::Vector2f getLastMiddleUnitPos();
	void handleSlotInteraction(bool& showInfoBox,int& clickedIndex, sf::Event& event, sf::RenderWindow& window);

	int* getSlotInfor(int index);
	void addSword(int number);
	void addSpear(int number);
};