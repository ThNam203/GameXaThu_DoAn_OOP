#include "BackField.h"

BackField::BackField(double unit_width, sf::Texture& texture) {
	this->unit_width = unit_width;
	std::fill_n(unitNumberInEachSlot, MAX_BACK_SLOT, 0);

	double current_y_axis = this->start_y_axis;
	double current_x_axis = this->start_x_axis;
	double back_width = WINDOW_WIDTH - start_x_axis * 2;

	double all_gap_between_units_in_line = (MAX_BACK_UNIT_PER_LINE - 1) * X_DISTANCE_BETWEEN_UNIT;
	double main_slot_width = unit_width * MAX_BACK_UNIT_PER_LINE + all_gap_between_units_in_line;

	double distance_between_slots = (back_width - (main_slot_width * MAX_BACK_SLOT)) / MAX_BACK_SLOT;
	double distance_from_back_field_to_screen = (back_width - (main_slot_width * MAX_BACK_SLOT + distance_between_slots * (MAX_BACK_SLOT - 1))) / 2;
	current_x_axis += distance_from_back_field_to_screen;

	for (int i = 0; i < MAX_BACK_SLOT; i++) {
		double current_x_util = current_x_axis;

		for (int j = 0; j < LINES_PER_SLOT; j++) {
			for (int k = 0; k < MAX_BACK_UNIT_PER_LINE; k++) {
				this->rectangle_unit_shape[i][j * MAX_BACK_UNIT_PER_LINE + k].setTexture(&texture);
				this->rectangle_unit_shape[i][j * MAX_BACK_UNIT_PER_LINE + k].setPosition(sf::Vector2f(current_x_util, current_y_axis));
				this->rectangle_unit_shape[i][j * MAX_BACK_UNIT_PER_LINE + k].setSize(sf::Vector2f(unit_width, UNIT_HEIGHT));

				current_x_util += unit_width + X_DISTANCE_BETWEEN_UNIT;
			}

			current_x_util = current_x_axis;

			current_y_axis -= Y_DISTANCE_BETWEEN_UNIT;
			current_y_axis -= UNIT_HEIGHT;
		}

		current_y_axis = start_y_axis;
		current_x_axis += main_slot_width + distance_between_slots;
	}

	this->setupInteractBox();
}

void BackField::drawBackField(sf::RenderWindow& window) {
	for (int i = 0; i < MAX_BACK_SLOT; i++) {
		for (int j = 0; j < this->unitNumberInEachSlot[i]; j++) {
			window.draw(this->rectangle_unit_shape[i][j]);
		}
	}
}

void BackField::setupInteractBox() {
	sf::Vector2f* allStartSlotCoords = new sf::Vector2f[MAX_BACK_SLOT];

	for (int i = 0; i < MAX_BACK_SLOT; i++) {
		allStartSlotCoords[i] = this->rectangle_unit_shape[i][(LINES_PER_SLOT - 1) * MAX_BACK_UNIT_PER_LINE].getGlobalBounds().getPosition();
	}

	double all_gap_between_units_in_line = (MAX_BACK_UNIT_PER_LINE - 1) * X_DISTANCE_BETWEEN_UNIT;
	double slot_width = this->unit_width * MAX_BACK_UNIT_PER_LINE + all_gap_between_units_in_line;
	double y_gap_between_main_lines = (LINES_PER_SLOT - 1) * Y_DISTANCE_BETWEEN_UNIT;
	double slot_height = UNIT_HEIGHT * LINES_PER_SLOT + y_gap_between_main_lines;

	for (int i = 0; i < MAX_BACK_SLOT; i++) {
		this->interact_box[i].setPosition(allStartSlotCoords[i]);
		this->interact_box[i].setSize(sf::Vector2f(slot_width, slot_height));
	}
}

void BackField::handleSlotInteraction(bool& showInfoBox, int& clickedIndex, sf::Event& event, sf::RenderWindow& window) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == 0) {
			int flag = 0;

			for (int i = 0; i < MAX_BACK_SLOT; i++) {
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

void BackField::reset() {
	std::fill_n(unitNumberInEachSlot, MAX_BACK_SLOT, 0);
}

int BackField::getNumberOfUnitAtIndex(int index) {
	return this->unitNumberInEachSlot[index];
}

int BackField::getBlood() {
	return this->blood;
}

int BackField::getDamage() {
	return this->damage;
}

int BackField::getMunition() {
	return this->munition;
}

int BackField::getArmor() {
	return this->armor;
}

int* BackField::getFieldInfor() {
	int* numberDamageBloodArmorMunition = new int[5];
	std::fill_n(numberDamageBloodArmorMunition, 5, 0);

	for (int i = 0; i < 8; i++) {
		numberDamageBloodArmorMunition[0] += this->unitNumberInEachSlot[i];
		numberDamageBloodArmorMunition[1] += this->unitNumberInEachSlot[i] * this->damage;
		numberDamageBloodArmorMunition[2] += this->unitNumberInEachSlot[i] * this->blood;
		numberDamageBloodArmorMunition[3] += this->unitNumberInEachSlot[i] * this->armor;
		numberDamageBloodArmorMunition[4] += this->unitNumberInEachSlot[i] * this->munition;
	}

	return numberDamageBloodArmorMunition;
}