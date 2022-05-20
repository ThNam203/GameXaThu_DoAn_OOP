#pragma once

#include <SFML/Graphics.hpp>
#include "GameProperties.h"
#include "MainField.h"

class BackField {
private:
	int damage = 100;
	int blood = 80;
	int armor = 80;
	int munition = 100;
	int max_unit = 96;
	double unit_width;
	sf::RectangleShape rectangle_unit_shape[MAX_BACK_SLOT][MAX_BACK_UNIT_PER_SLOT];
	sf::RectangleShape interact_box[MAX_BACK_SLOT];
	
	double start_x_axis = Y_DISTANCE_BETWEEN_FIELD_AND_SCREEN;
	double y_gap_between_main_lines = (LINES_PER_SLOT - 1) * Y_DISTANCE_BETWEEN_UNIT;
	double start_y_axis = MainField::start_y_axis - UNIT_HEIGHT * (LINES_PER_SLOT - 1) - y_gap_between_main_lines - Y_DISTANCE_BETWEEN_FIELD;

public:
	int unitNumberInEachSlot[MAX_BACK_SLOT];

	BackField(double unit_width, sf::Texture& texture);
	void drawBackField(sf::RenderWindow& window);
	void reset();
	int getNumberOfUnitAtIndex(int index);
	void setupInteractBox(); // call in constructor, also set up for slot_info_box
	void handleSlotInteraction(bool& showInfoBox, int& clickedIndex, sf::Event& event, sf::RenderWindow& window);

	int getBlood();
	int getDamage();
	int getMunition();
	int getArmor();

	int* getFieldInfor();
};