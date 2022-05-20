#pragma once

#include "imgui.h"
#include "imconfig-SFML.h"

#include <SFML/Graphics.hpp>
#include "GameProperties.h"
#include "game_data/NonBulletUnit.h"
#include "Texture.h"

class MainField {
private:
	int max_unit = 240;
	double unit_width;
	Texture texture;
	sf::RectangleShape rectangle_unit_shape[8][30];
	sf::RectangleShape interact_box[MAX_MAIN_SLOT];

public:
	int swordDamage = 85;
	int swordBlood = 80;
	int swordArmor = 80;
	int hopDamage = 70;
	int hopBlood = 80;
	int hopArmor = 80;
	int spearDamage = 75;
	int spearBlood = 85;
	int spearArmor = 85;
	static double start_x_axis;
	static double start_y_axis;
	int typeOfUnit[MAX_MAIN_SLOT];
	int unitNumberInEachSlot[MAX_MAIN_SLOT];

	MainField(double unit_width);
	void setFieldTexture();
	void drawMainField(sf::RenderWindow& window);
	void reset();
	int getTypeOfUnitAtIndex(int index);
	int getNumberOfUnitAtIndex(int index);
	void setupInteractBox(); // call in constructor
	void handleSlotInteraction(bool& showInfoBox, int& clickedIndex, sf::Event& event, sf::RenderWindow& window);

	int* getSlotInfor(int index);
};