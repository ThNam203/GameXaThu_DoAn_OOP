#include "FlankField.h"

FlankField::FlankField(double start_x_axis, double unit_width, sf::Texture& texture) {
	this->unit_width = unit_width;
	std::fill_n(typeOfUnit, MAX_FLANK_SLOT, 1);
	std::fill_n(unitNumberInEachSlot, MAX_FLANK_SLOT, 0);

	this->start_x_axis = start_x_axis;

	double current_y_axis = this->start_y_axis;
	double current_x_axis = this->start_x_axis;

	double all_gap_between_units_in_line = (MAX_FLANK_UNIT_PER_LINE - 1) * X_DISTANCE_BETWEEN_UNIT;
	double main_slot_width = unit_width * MAX_FLANK_UNIT_PER_LINE + all_gap_between_units_in_line + DISTANCE_BETWEEN_SLOT;

	for (int i = 0; i < MAX_FLANK_SLOT; i++) {
		double current_x_util = current_x_axis;

		for (int j = 0; j < LINES_PER_SLOT_FLANK; j++) {
			for (int k = 0; k < MAX_FLANK_UNIT_PER_LINE; k++) {
				this->rectangle_unit_shape[i][j * MAX_FLANK_UNIT_PER_LINE + k].setPosition(sf::Vector2f(current_x_util, current_y_axis));
				this->rectangle_unit_shape[i][j * MAX_FLANK_UNIT_PER_LINE + k].setSize(sf::Vector2f(unit_width, UNIT_HEIGHT));

				current_x_util += unit_width + X_DISTANCE_BETWEEN_UNIT;
			}

			current_x_util = current_x_axis;

			current_y_axis -= Y_DISTANCE_BETWEEN_UNIT;
			current_y_axis -= UNIT_HEIGHT;
		}

		current_y_axis = start_y_axis;
		current_x_axis += main_slot_width;
	}

	this->setupInteractBox();
}

void FlankField::setFieldTexture() {
	for (int i = 0; i < MAX_FLANK_SLOT; i++) {
		if (this->typeOfUnit[i] == 1) {
			for (int j = 0; j < MAX_FLANK_UNIT_PER_SLOT; j++) {
				this->rectangle_unit_shape[i][j].setTexture(&texture.swordman_texture);
			}
		}
		else if (this->typeOfUnit[i] == 2) {
			for (int j = 0; j < MAX_FLANK_UNIT_PER_SLOT; j++) {
				this->rectangle_unit_shape[i][j].setTexture(&texture.spear_texture);
			}
		}
	}
}

void FlankField::drawFlankField(sf::RenderWindow& window) {
	for (int i = 0; i < MAX_FLANK_SLOT; i++) {
		for (int j = 0; j < this->unitNumberInEachSlot[i]; j++) {
			window.draw(this->rectangle_unit_shape[i][j]);
		}
	}
}

void FlankField::setupInteractBox() {
	sf::Vector2f* allStartSlotCoords = new sf::Vector2f[MAX_FLANK_SLOT];

	// TOP LEFT CORNER FOR THE RIGHTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT
	for (int i = 0; i < MAX_FLANK_SLOT; i++) {
		allStartSlotCoords[i] = this->rectangle_unit_shape[i][(LINES_PER_SLOT_FLANK - 1) * MAX_FLANK_UNIT_PER_LINE].getGlobalBounds().getPosition();
	}

	double all_gap_between_units_in_line = (MAX_FLANK_UNIT_PER_LINE - 1) * X_DISTANCE_BETWEEN_UNIT;
	double slot_width = unit_width * MAX_FLANK_UNIT_PER_LINE + all_gap_between_units_in_line;
	double y_gap_between_lines = (LINES_PER_SLOT_FLANK - 1) * Y_DISTANCE_BETWEEN_UNIT;
	double slot_height = UNIT_HEIGHT * LINES_PER_SLOT_FLANK + y_gap_between_lines;

	for (int i = 0; i < MAX_FLANK_SLOT; i++) {
		this->interact_box[i].setPosition(allStartSlotCoords[i]);
		this->interact_box[i].setSize(sf::Vector2f(slot_width, slot_height));
	}
}

void FlankField::handleSlotInteraction(bool& showInfoBox, int& clickedIndex, sf::Event& event, sf::RenderWindow& window) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == 0) {
			int flag = 0;

			for (int i = 0; i < MAX_FLANK_SLOT; i++) {
				if (this->interact_box[i].getGlobalBounds().contains(sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(sf::Mouse::getPosition(window)))))) {
					showInfoBox = true;
					clickedIndex = i;
					flag = 1;
					break;
				}
			}

			if (flag == 0) {
				showInfoBox = false;
			}
		}
	}
}

sf::Vector2f FlankField::getFirstMiddleUnitPos() {
	return sf::Vector2f(this->rectangle_unit_shape[0][0].getGlobalBounds().getPosition().x, this->rectangle_unit_shape[0][0].getGlobalBounds().getPosition().y - Y_DISTANCE_BETWEEN_UNIT);
}

sf::Vector2f FlankField::getLastMiddleUnitPos() {
	return sf::Vector2f(this->rectangle_unit_shape[2][9].getGlobalBounds().getPosition().x, this->rectangle_unit_shape[2][9].getGlobalBounds().getPosition().y - Y_DISTANCE_BETWEEN_UNIT);
}

void FlankField::reset() {
	std::fill_n(typeOfUnit, MAX_FLANK_SLOT, 1);
	std::fill_n(unitNumberInEachSlot, MAX_FLANK_SLOT, 0);
}

int FlankField::getTypeOfUnitAtIndex(int index) {
	return this->typeOfUnit[index];
}

int FlankField::getNumberOfUnitAtIndex(int index) {
	return this->unitNumberInEachSlot[index];
}

int* FlankField::getSlotInfor(int index) {
	int* damageBloodArmor = new int[3];
	int number_of_unit = getNumberOfUnitAtIndex(index);
	int type = getTypeOfUnitAtIndex(index); // 1 sword, 2 spear

	if (type == 1) {
		damageBloodArmor[0] = number_of_unit * swordDamage;
		damageBloodArmor[1] = number_of_unit * swordBlood;
		damageBloodArmor[2] = number_of_unit * swordArmor;
	} else {
		damageBloodArmor[0] = number_of_unit * spearDamage;
		damageBloodArmor[1] = number_of_unit * spearBlood;
		damageBloodArmor[2] = number_of_unit * spearArmor;
	}

	return damageBloodArmor;
}


void FlankField::addSword(int number) {
	for (int i = 0; i < 3; i++) {
		if (number <= 0) return;
		std::cout << number << std::endl;
		if (this->unitNumberInEachSlot[i] == 0) {
			this->typeOfUnit[i] = 1;
			this->setFieldTexture();
			if (number > 20) {
				this->unitNumberInEachSlot[i] = 20;
				number -= 20;
			}
			else {
				this->unitNumberInEachSlot[i] = number;
				return;
			}
		}
		else if (this->typeOfUnit[i] == 1) {
			if (this->unitNumberInEachSlot[i] + number > 20) {
				this->unitNumberInEachSlot[i] = 20;
				int mod = 20 - this->unitNumberInEachSlot[i];
				number = number - mod;
			}
			else {
				this->unitNumberInEachSlot[i] += number;
				return;
			}
		}
	}
}

void FlankField::addSpear(int number) {
	for (int i = 0; i < 3; i++) {
		if (number <= 0) return;

		if (this->unitNumberInEachSlot[i] == 0) {
			this->typeOfUnit[i] = 2;
			this->setFieldTexture();
			if (number > 20) {
				this->unitNumberInEachSlot[i] = 20;
				number -= 20;
			}
			else {
				this->unitNumberInEachSlot[i] = number;
				return;
			}
		}
		else if (this->typeOfUnit[i] == 2) {
			if (this->unitNumberInEachSlot[i] + number > 20) {
				this->unitNumberInEachSlot[i] = 20;
				int mod = 20 - this->unitNumberInEachSlot[i];
				number = number - mod;
			}
			else {
				this->unitNumberInEachSlot[i] += number;
				return;
			}
		}
	}
}