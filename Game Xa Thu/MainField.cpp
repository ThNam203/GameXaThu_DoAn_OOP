#include "MainField.h"
#include <iostream>

double MainField::start_x_axis = Y_DISTANCE_BETWEEN_FIELD_AND_SCREEN;
double MainField::start_y_axis = WINDOW_HEIGHT / 2 + UNIT_HEIGHT + Y_DISTANCE_BETWEEN_UNIT;

MainField::MainField(double unit_width) {
	this->unit_width = unit_width;
	std::fill_n(typeOfUnit, MAX_MAIN_SLOT, 1);
	std::fill_n(unitNumberInEachSlot, MAX_MAIN_SLOT, 0);


	double current_y_axis = this->start_y_axis;
	double current_x_axis = this->start_x_axis;
	double current_unit_number = 0;

	double all_gap_between_units_in_line = (MAX_MAIN_UNIT_PER_LINE - 1) * X_DISTANCE_BETWEEN_UNIT;
	double main_slot_width = unit_width * MAX_MAIN_UNIT_PER_LINE + all_gap_between_units_in_line;

	for (int i = 0; i < MAX_MAIN_SLOT; i++) {
		double current_x_util = current_x_axis;

		for (int j = 0; j < LINES_PER_SLOT; j++) {
			for (int k = 0; k < MAX_MAIN_UNIT_PER_LINE; k++) {
				this->rectangle_unit_shape[i][j * MAX_MAIN_UNIT_PER_LINE + k].setPosition(sf::Vector2f(current_x_util, current_y_axis));
				this->rectangle_unit_shape[i][j * MAX_MAIN_UNIT_PER_LINE + k].setSize(sf::Vector2f(unit_width, UNIT_HEIGHT));

				current_x_util += unit_width + X_DISTANCE_BETWEEN_UNIT;
			}

			current_x_util = current_x_axis;

			current_y_axis -= Y_DISTANCE_BETWEEN_UNIT;
			current_y_axis -= UNIT_HEIGHT;
		}

		current_y_axis = this->start_y_axis;
		current_x_axis += main_slot_width + DISTANCE_BETWEEN_SLOT;
	}

	this->setupInteractBox();
}

void MainField::setFieldTexture() {
	for (int i = 0; i < MAX_MAIN_SLOT; i++) {
		if (this->typeOfUnit[i] == 1) {
			for (int j = 0; j < MAX_MAIN_UNIT_PER_SLOT; j++) {
				this->rectangle_unit_shape[i][j].setTexture(&texture.swordman_texture);
			}
		}
		else if (this->typeOfUnit[i] == 2) {
			for (int j = 0; j < MAX_MAIN_UNIT_PER_SLOT; j++) {
				this->rectangle_unit_shape[i][j].setTexture(&texture.spear_texture);
			}
		}
		else for (int j = 0; j < MAX_MAIN_UNIT_PER_SLOT; j++) {
			this->rectangle_unit_shape[i][j].setTexture(&texture.hop_texture);
		}
	}
}

void MainField::drawMainField(sf::RenderWindow& window) {
	for (int i = 0; i < MAX_MAIN_SLOT; i++) {
		for (int j = 0; j < this->unitNumberInEachSlot[i]; j++) {
			window.draw(this->rectangle_unit_shape[i][j]);
		}
	}
}

void MainField::setupInteractBox() {
	sf::Vector2f* allStartSlotCoords = new sf::Vector2f[MAX_MAIN_SLOT];

	// TOP LEFT CORNER FOR THE RIGHTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
	for (int i = 0; i < MAX_MAIN_SLOT; i++) {
		allStartSlotCoords[i] = this->rectangle_unit_shape[i][(LINES_PER_SLOT - 1) * MAX_MAIN_UNIT_PER_LINE].getGlobalBounds().getPosition();
	}

	double all_gap_between_units_in_line = (MAX_MAIN_UNIT_PER_LINE - 1) * X_DISTANCE_BETWEEN_UNIT;
	double slot_width = this->unit_width * MAX_MAIN_UNIT_PER_LINE + all_gap_between_units_in_line;
	double y_gap_between_main_lines = (LINES_PER_SLOT - 1) * Y_DISTANCE_BETWEEN_UNIT;
	double slot_height = UNIT_HEIGHT * LINES_PER_SLOT + y_gap_between_main_lines;

	for (int i = 0; i < MAX_MAIN_SLOT; i++) {
		this->interact_box[i].setPosition(allStartSlotCoords[i]);
		this->interact_box[i].setSize(sf::Vector2f(slot_width, slot_height));
	}
}

void MainField::handleSlotInteraction(bool& showInfoBox, int& clickedIndex, sf::Event& event, sf::RenderWindow& window) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == 0) {
			int flag = 0;

			for (int i = 0; i < MAX_MAIN_SLOT; i++) {
				if (this->interact_box[i].getGlobalBounds().contains(sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(sf::Mouse::getPosition(window)))))) {
					showInfoBox = true;
					flag = 1;
					clickedIndex = i;
					break;
				}
			}
			
			if (flag == 0) {
				showInfoBox = false;
			}
		}
	}
}

void MainField::reset() {
	std::fill_n(typeOfUnit, MAX_MAIN_SLOT, 1);
	std::fill_n(unitNumberInEachSlot, MAX_MAIN_SLOT, 0);
}

int MainField::getTypeOfUnitAtIndex(int index) {
	return this->typeOfUnit[index];
}

int MainField::getNumberOfUnitAtIndex(int index) {
	return this->unitNumberInEachSlot[index];
}

int* MainField::getSlotInfor(int index) {
	int* damageBloodArmor = new int[3];
	int number_of_unit = getNumberOfUnitAtIndex(index);
	int type = getTypeOfUnitAtIndex(index); // 1 sword, 2 spear, 3 hop

	if (type == 1) {
		damageBloodArmor[0] = number_of_unit * swordDamage;
		damageBloodArmor[1] = number_of_unit * swordBlood;
		damageBloodArmor[2] = number_of_unit * swordArmor;
	}
	else if (type == 2) {
		damageBloodArmor[0] = number_of_unit * spearDamage;
		damageBloodArmor[1] = number_of_unit * spearBlood;
		damageBloodArmor[2] = number_of_unit * spearArmor;
	}
	else {
		damageBloodArmor[0] = number_of_unit * hopDamage;
		damageBloodArmor[1] = number_of_unit * hopBlood;
		damageBloodArmor[2] = number_of_unit * hopArmor;
	}

	return damageBloodArmor;
}