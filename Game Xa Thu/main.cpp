#include "imgui.h"
#include "imgui_internal.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <Windows.h>

#include "GameData.h"

double getUnitWidth();
void gameReset();
double getFlankFieldWidth(double unit_width);
void menuMusicStart();
void gameMusicStart();
void eventHandleFunc(sf::Event event, sf::View& view, sf::RenderWindow& window);

// ImGui style config
void ImGuiStyleConfig();
// ImGui draw
void ImGuiMenuDraw();
void ImGuiPauseDraw();
void InGameMenuDraw();
void GameInformationDraw();
void AreYouSureToQuit();
void AddFlankUnitDraw();
void AddBackUnitAtStartDraw();
void AddMainUnitAtStartDraw();
void AddFlankUnitAtStartDraw();
void SlotInforDraw(std::string slot_name, int slot_index);

// MENUUUUUUUUUUUUUUUUUUUUUU
std::vector<const char*> gameMenuItemList = {
	{ "START GAME" },
	{ "SETTING" },
	{ "EXIT GAME" }
};

struct Setting {
	int musicVolumn = 50;
	int minMusic = 0;
	int maxMusic = 100;
};

Setting setting; // music setting
sf::Music menuMusic;
sf::Music gameMusic;

bool isGameStarted = false;
bool isGameClose = false;
bool moving = false; // for dragging game view screen
sf::Vector2f oldPos; // for dragging game view screen

// Texture load and set up some sprite properties and start game screen
Texture texture;
StartGameScreen startGameScreen;

bool gameInformationShow = false;
bool showAreYouSureToQuit = false;
bool isGameMenuShow = true;
bool isAddBackUnitAtStartDrawShow = false;
bool isAddMainUnitAtStartDrawShow = false;
bool isAddFlankUnitAtStartDrawShow = false;
bool isLeftAddButtonClicked = false;
bool isRightAddButtonClicked = false;
bool isAddFlankUnitShow = false;
bool activeSlotInteractHandle = false;
bool isGamePauseShow = false;
bool isInforBoxShow = false;
bool isMainInforBoxShow = false;
bool isBackInforBoxShow = false;
bool isLeftFlankInforBoxShow = false;
bool isRightFlankInforBoxShow = false;

int backFieldInfoBoxIndex = -1;
int mainFieldInfoBoxIndex = -1;
int leftFlankFieldInfoBoxIndex = -1;
int rightFlankFieldInfoBoxIndex = -1;

// MAIN FIELD
double all_unit_width = getUnitWidth(); // set up global unit width
MainField mainField(all_unit_width);

// BACK FIELD
double back_unit_width = all_unit_width * 2; // make it twice as big as other :))
BackField backField(back_unit_width, texture.gunner_texture);

// FLANK FIELD
double start_left_flank_field_x_axis = Y_DISTANCE_BETWEEN_FIELD_AND_SCREEN;
double left_flank_field_width = getFlankFieldWidth(all_unit_width);
double start_right_flank_field_x_axis = WINDOW_WIDTH - Y_DISTANCE_BETWEEN_FIELD_AND_SCREEN - left_flank_field_width;

FlankField leftFlankField(start_left_flank_field_x_axis, all_unit_width, texture.hop_texture);
FlankField rightFlankField(start_right_flank_field_x_axis, all_unit_width, texture.spear_texture);
// END FLANK FIELD

sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE, sf::Style::Titlebar | sf::Style::Close);
sf::View view = window.getDefaultView();

int number_of_add_more_sword = 0;
int number_of_add_more_spear = 0;

int main() {
	ShowWindow(GetConsoleWindow(), SW_HIDE); // hide console window
	//ShowWindow(GetConsoleWindow(), SW_SHOW);
	window.setView(view);
	window.setIcon(texture.game_icon_image.getSize().x, texture.game_icon_image.getSize().y, texture.game_icon_image.getPixelsPtr());

	ImGui::SFML::Init(window);
	ImGuiStyleConfig(); // config imgui
	sf::Clock deltaClock; // imgui update function

	if (!gameMusic.openFromFile("music/game_music.ogg")) std::cout << "Cannot load game music" << std::endl;
	if (!menuMusic.openFromFile("music/menu_music.ogg")) std::cout << "Cannot load menu music" << std::endl;
	menuMusicStart();

	while (window.isOpen() && !isGameClose) {
		sf::Event event;
		while (window.pollEvent(event))	eventHandleFunc(event, view, window);
		
		isInforBoxShow = isMainInforBoxShow || isBackInforBoxShow || isLeftFlankInforBoxShow || isRightFlankInforBoxShow;
		ImGui::SFML::Update(window, deltaClock.restart());
		
		window.clear(sf::Color(133, 222, 157));

		if (!isGameStarted) {
			window.draw(startGameScreen.start_game_background_sprite);
			if (isGameMenuShow) ImGuiMenuDraw();
			else {
				if (isAddBackUnitAtStartDrawShow) AddBackUnitAtStartDraw();
				else if (isAddMainUnitAtStartDrawShow) AddMainUnitAtStartDraw();
				else if (isAddFlankUnitAtStartDrawShow) AddFlankUnitAtStartDraw();
			}
			if (showAreYouSureToQuit) AreYouSureToQuit();
		}
		else {
			mainField.drawMainField(window);
			backField.drawBackField(window);
			leftFlankField.drawFlankField(window);
			rightFlankField.drawFlankField(window);

			InGameMenuDraw();
			if (gameInformationShow) GameInformationDraw();

			if (isGamePauseShow) ImGuiPauseDraw();
			if (isAddFlankUnitShow) AddFlankUnitDraw();

			if (isInforBoxShow) {
				if (isMainInforBoxShow) SlotInforDraw("MAIN", mainFieldInfoBoxIndex);
				else if (isBackInforBoxShow) SlotInforDraw("BACK", backFieldInfoBoxIndex);
				else if (isLeftFlankInforBoxShow) SlotInforDraw("LEFT", leftFlankFieldInfoBoxIndex);
				else if (isRightFlankInforBoxShow) SlotInforDraw("RIGHT", rightFlankFieldInfoBoxIndex);
			}
		}

		ImGui::SFML::Render(window);

		window.display();
	}

	ImGui::SFML::Shutdown();
	return 0;
}

void eventHandleFunc(sf::Event event, sf::View& view, sf::RenderWindow& window) {
	ImGui::SFML::ProcessEvent(window, event);
	if (event.type == sf::Event::Closed) window.close();

	if (isGameStarted) {
		if (activeSlotInteractHandle) {
			backField.handleSlotInteraction(isBackInforBoxShow, backFieldInfoBoxIndex, event, window);
			mainField.handleSlotInteraction(isMainInforBoxShow, mainFieldInfoBoxIndex, event, window);
			leftFlankField.handleSlotInteraction(isLeftFlankInforBoxShow, leftFlankFieldInfoBoxIndex, event, window);
			rightFlankField.handleSlotInteraction(isRightFlankInforBoxShow, rightFlankFieldInfoBoxIndex, event, window);
		}

		switch (event.type) {
		/*case sf::Event::MouseWheelScrolled:
			if (event.mouseWheelScroll.delta == 1) {
				view.zoom(0.9f);
			}
			else view.zoom(1.1f);

			window.setView(view);
			break;*/
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == 0) {
				moving = true;
				oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

				// infor box
				if (isInforBoxShow) {
					isInforBoxShow = false;
					isMainInforBoxShow = false;
					isBackInforBoxShow = false;
					isLeftFlankInforBoxShow = false;
					isRightFlankInforBoxShow = false;
				}
			}
			break;
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape) {
				if (gameInformationShow) {
					activeSlotInteractHandle = true;
					gameInformationShow = false;
				} 
				else if (isAddFlankUnitShow) {
					activeSlotInteractHandle = true;
					isAddFlankUnitShow = false;
				}
				else if (isInforBoxShow) {
					isInforBoxShow = false;
					isMainInforBoxShow = false;
					isBackInforBoxShow = false;
					isLeftFlankInforBoxShow = false;
					isRightFlankInforBoxShow = false;
				}
				else if (isGamePauseShow) {
					activeSlotInteractHandle = true;
					isGamePauseShow = false;
				}
				else {
					activeSlotInteractHandle = false;
					isGamePauseShow = true;
					isInforBoxShow = false;
					isMainInforBoxShow = false;
					isBackInforBoxShow = false;
					isLeftFlankInforBoxShow = false;
					isRightFlankInforBoxShow = false;
				}
			}
			break;
		/*case  sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == 0) {
				moving = false;
			}
			break;
		case sf::Event::MouseMoved:
		{
			if (!moving) break;
			const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			const sf::Vector2f deltaPos = oldPos - newPos;

			view.setCenter(view.getCenter() + deltaPos);
			window.setView(view);
			oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
			break;
		}*/
		}
	}
}

double getUnitWidth() {
	double all_gap_between_units_in_field = MAX_MAIN_SLOT * (MAX_MAIN_UNIT_PER_LINE - 1) * X_DISTANCE_BETWEEN_UNIT;
	double unit_width = (WINDOW_WIDTH - Y_DISTANCE_BETWEEN_FIELD_AND_SCREEN * 2 - DISTANCE_BETWEEN_SLOT * (MAX_MAIN_SLOT - 1) - all_gap_between_units_in_field) / (MAX_MAIN_SLOT * MAX_MAIN_UNIT_PER_LINE);
	return unit_width;
}

void gameReset() {
	mainField.reset();
	backField.reset();
	leftFlankField.reset();
	rightFlankField.reset();
}

double getFlankFieldWidth(double unit_width) {
	double all_gap_between_units_in_line = (MAX_FLANK_UNIT_PER_LINE - 1) * X_DISTANCE_BETWEEN_UNIT;
	double flank_width = (unit_width * MAX_FLANK_UNIT_PER_LINE + all_gap_between_units_in_line) * MAX_FLANK_SLOT + (MAX_FLANK_SLOT - 1) * DISTANCE_BETWEEN_SLOT;
	return flank_width;
}

void menuMusicStart() {
	gameMusic.stop();
	menuMusic.play();
	menuMusic.setLoop(true);
	menuMusic.setVolume(setting.musicVolumn);
}

void gameMusicStart() {
	menuMusic.stop();
	gameMusic.play();
	gameMusic.setLoop(true);
	gameMusic.setVolume(setting.musicVolumn);
}

void ImGuiStyleConfig() {
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	// io
	
	// io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Tahoma", 16.0f);

	// style
	// widh, height

	style.Colors[ImGuiCol_WindowBg] = ImColor(16, 16, 16);
	style.Colors[ImGuiCol_ChildBg] = ImColor(24, 24, 24);
	style.Colors[ImGuiCol_Text] = ImColor(255, 255, 255);
	style.Colors[ImGuiCol_CheckMark] = ImColor(255, 255, 255);
	style.Colors[ImGuiCol_Header] = ImColor(130, 140, 81);
	style.Colors[ImGuiCol_HeaderActive] = ImColor(194, 184, 178);
	style.Colors[ImGuiCol_HeaderHovered] = ImColor(194, 184, 178);
	style.Colors[ImGuiCol_Button] = ImColor(24, 24, 24);
	style.Colors[ImGuiCol_ButtonActive] = ImColor(24, 24, 24);
	style.Colors[ImGuiCol_ButtonHovered] = ImColor(24, 24, 24);
}

void ImGuiMenuDraw() {
	static int selectedItem = 0;
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	ImGui::Begin("Game menu", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	
	ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0, 0, 0, 255).Value);
	ImGui::BeginChild("##Menu items", ImVec2(120, ImGui::GetContentRegionAvail().y), true);
	{
		float height = ImGui::GetContentRegionAvail().y - 10;
		for (int i = 0; i < gameMenuItemList.size(); i++) {
			bool isSelected = (selectedItem == i);
			if (ImGui::Selectable(gameMenuItemList[i], &isSelected, 0, ImVec2(ImGui::GetContentRegionAvail().x, height / 3))) selectedItem = i;
			if (isSelected) ImGui::SetItemDefaultFocus();
		}
	}
	ImGui::EndChild();

	{
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
	}

	ImGui::BeginChild("##ItemContainer", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
	{
		auto center = [](float avai_width, float element_width, float padding = 0) {
			ImGui::SameLine((avai_width / 2) - (element_width / 2) + padding);
		};

		auto center_text = [&](const char* format, float spacing = 15, ImColor color = ImColor(255, 255, 255)) {
			center(ImGui::GetContentRegionAvail().x, ImGui::CalcTextSize(format).x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + spacing);
			ImGui::TextColored(color, format);
		};

		// 0 is start game
		if (selectedItem == 0) {
			center(ImGui::GetContentRegionAvail().x, startGameScreen.start_button_sprite.getGlobalBounds().width);
			ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - startGameScreen.start_button_sprite.getGlobalBounds().height) * 0.5f);
			if (ImGui::ImageButton(startGameScreen.start_button_sprite)) {
				isAddBackUnitAtStartDrawShow = true;
				isGameMenuShow = false;
			}
		} // 1 is setting
		else if (selectedItem == 1) {
			ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize("SETTING").y - ImGui::CalcTextSize("MUSIC").y * 2) * 0.5f - 20);
			ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("SETTING").x) * 0.5f);
			ImGui::Text("SETTING");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::CalcTextSize("MUSIC").y);
			if (ImGui::SliderInt("  MUSIC VOLUMN", &setting.musicVolumn, setting.minMusic, setting.maxMusic, std::to_string(setting.musicVolumn).c_str())) {
				gameMusic.setVolume(setting.musicVolumn);
				menuMusic.setVolume(setting.musicVolumn);
			}
		} // 2 is exit game
		else if (selectedItem == 2) {
			showAreYouSureToQuit = true;
			isGamePauseShow = false;
		}
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::End();
}

void SlotInforDraw(std::string slot_name, int slot_index) {
	int boxHeight = 230;
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(300, boxHeight));

	std::string slot_window_name;
	if (slot_name == "MAIN" || slot_name == "BACK") slot_window_name = "    " + slot_name + " FIELD - SLOT " + to_string(slot_index + 1) + "th'S PROPERTIES";
	else slot_window_name = slot_name + " FLANK FIELD - SLOT " + to_string(slot_index + 1) + "th's PROPERTIES";
	
	if (slot_name == "BACK") {
		int number_of_unit = backField.getNumberOfUnitAtIndex(slot_index);

		if (number_of_unit > 0) {
			ImGui::Begin(slot_window_name.c_str(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 25);
			ImGui::Image(texture.gunner_texture, sf::Vector2f(50, 50));

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 25);
			ImGui::Text("Powder");

			ImGui::Text("\n          Slot's unit number: %i", number_of_unit);
			ImGui::Text("            Total damage: %i", number_of_unit * backField.getDamage());
			ImGui::Text("            Total heatlh: %i", number_of_unit * backField.getBlood());
			ImGui::Text("            Total armor: %i", number_of_unit * backField.getArmor());
			ImGui::Text("\n        ALL UNITS READY FOR BATTLE");
			ImGui::End();
		}
	}
	else if (slot_name == "MAIN") {
		int type = mainField.getTypeOfUnitAtIndex(slot_index); // 1 is sword, 2 is spear, 3 is hopper
		int number_of_unit = mainField.getNumberOfUnitAtIndex(slot_index);
		int* damageBloodArmorInfo = mainField.getSlotInfor(slot_index);

		if (number_of_unit > 0) {
			ImGui::Begin(slot_window_name.c_str(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 25);
			if (type == 1) ImGui::Image(texture.swordman_texture, sf::Vector2f(50, 50));
			else if (type == 2) ImGui::Image(texture.spear_texture, sf::Vector2f(50, 50));
			else if (type == 3) ImGui::Image(texture.hop_texture, sf::Vector2f(50, 50));

			std::string unitName;
			if (type == 1) unitName = "Swordman";
			else if (type == 2) unitName = "Spearman";
			else if (type == 3) unitName = "Hopper";
			if (type == 3) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 20);
			else ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 25);
			ImGui::Text(unitName.c_str());

			ImGui::Text("\n          Slot's unit number: %i", number_of_unit);
			ImGui::Text("            Total damage: %i", damageBloodArmorInfo[0]);
			ImGui::Text("            Total heatlh: %i", damageBloodArmorInfo[1]);
			ImGui::Text("            Total armor: %i", damageBloodArmorInfo[2]);
			ImGui::Text("\n        ALL UNITS READY FOR BATTLE");
			ImGui::End();
		}
	}
	else if (slot_name == "LEFT") {
		int type = leftFlankField.getTypeOfUnitAtIndex(slot_index); // 1 is sword, 2 is spear, 3 is hopper
		int number_of_unit = leftFlankField.getNumberOfUnitAtIndex(slot_index);
		int* damageBloodArmorInfo = leftFlankField.getSlotInfor(slot_index);

		if (number_of_unit > 0) {
			ImGui::Begin(slot_window_name.c_str(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 25);
			if (type == 1) ImGui::Image(texture.swordman_texture, sf::Vector2f(50, 50));
			else if (type == 2) ImGui::Image(texture.spear_texture, sf::Vector2f(50, 50));

			std::string unitName;
			if (type == 1) unitName = "Swordman";
			else if (type == 2) unitName = "Spear";
			if (type == 2) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 20);
			else ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 25);
			ImGui::Text(unitName.c_str());

			ImGui::Text("\n          Slot's unit number: %i", number_of_unit);
			ImGui::Text("            Total damage: %i", damageBloodArmorInfo[0]);
			ImGui::Text("            Total heatlh: %i", damageBloodArmorInfo[1]);
			ImGui::Text("            Total armor: %i", damageBloodArmorInfo[2]);
			ImGui::Text("\n        ALL UNITS READY FOR BATTLE");
			ImGui::End();
		}
	}
	else {
		int type = rightFlankField.getTypeOfUnitAtIndex(slot_index); // 1 is sword, 2 is spear, 3 is hopper
		int number_of_unit = rightFlankField.getNumberOfUnitAtIndex(slot_index);
		int* damageBloodArmorInfo = rightFlankField.getSlotInfor(slot_index);

		if (number_of_unit > 0) {
			ImGui::Begin(slot_window_name.c_str(), 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 25);
			if (type == 1) ImGui::Image(texture.swordman_texture, sf::Vector2f(50, 50));
			else if (type == 2) ImGui::Image(texture.spear_texture, sf::Vector2f(50, 50));

			std::string unitName;
			if (type == 1) unitName = "Swordman";
			else if (type == 2) unitName = "Spear";
			if (type == 2) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 20);
			else ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 25);
			ImGui::Text(unitName.c_str());

			ImGui::Text("\n          Slot's unit number: %i", number_of_unit);
			ImGui::Text("            Total damage: %i", damageBloodArmorInfo[0]);
			ImGui::Text("            Total heatlh: %i", damageBloodArmorInfo[1]);
			ImGui::Text("            Total armor: %i", damageBloodArmorInfo[2]);
			ImGui::Text("\n        ALL UNITS READY FOR BATTLE");
			ImGui::End();
		}
	}
}

void ImGuiPauseDraw() {
	static int selectedItem = 0;
	static std::string pauseItemList[2] = { "SETTING", "MAIN MENU" };
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	ImGui::Begin("Game menu", 0, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	ImGui::PushStyleColor(ImGuiCol_Border, ImColor(0, 0, 0, 255).Value);
	ImGui::BeginChild("##Menu items", ImVec2(120, ImGui::GetContentRegionAvail().y), true);
	{
		float height = ImGui::GetContentRegionAvail().y - 10;
		for (int i = 0; i < 2; i++) { // there are 2 pause items
			bool isSelected = (selectedItem == i);
			if (ImGui::Selectable(pauseItemList[i].c_str(), &isSelected, 0, ImVec2(ImGui::GetContentRegionAvail().x, height / 2))) selectedItem = i;
			if (isSelected) ImGui::SetItemDefaultFocus();
		}
	}
	ImGui::EndChild();

	{
		ImGui::SameLine();
		ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
		ImGui::SameLine();
	}

	ImGui::BeginChild("##ItemContainer", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true);
	{
		auto center = [](float avai_width, float element_width, float padding = 0) {
			ImGui::SameLine((avai_width / 2) - (element_width / 2) + padding);
		};

		auto center_text = [&](const char* format, float spacing = 15, ImColor color = ImColor(255, 255, 255)) {
			center(ImGui::GetContentRegionAvail().x, ImGui::CalcTextSize(format).x);
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + spacing);
			ImGui::TextColored(color, format);
		};
		 // 0 is setting
		if (selectedItem == 0) {
			ImGui::SetCursorPosY((ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize("SETTING").y - ImGui::CalcTextSize("MUSIC").y * 2) * 0.5f - 20);
			ImGui::SetCursorPosX((ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("SETTING").x) * 0.5f);
			ImGui::Text("SETTING");
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + ImGui::CalcTextSize("MUSIC").y);
			if (ImGui::SliderInt("  MUSIC VOLUMN", &setting.musicVolumn, setting.minMusic, setting.maxMusic, std::to_string(setting.musicVolumn).c_str())) {
				gameMusic.setVolume(setting.musicVolumn);
				menuMusic.setVolume(setting.musicVolumn);
			}
		} // 1 is exit to main menu
		else if (selectedItem == 1) {
			selectedItem = 0; // reset pause item
			isGameStarted = false;
			isGamePauseShow = false;
			isGameMenuShow = true;
			gameReset();
			menuMusicStart();
		}
	}
	ImGui::EndChild();
	ImGui::PopStyleColor();

	ImGui::End();
}

void AddFlankUnitDraw() {
	double y_pos = ImGui::GetIO().DisplaySize.y;
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(200, 200));
	ImGui::Begin("Add unit", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

	ImGui::BeginChild("##item", ImVec2(200, 180), false, ImGuiWindowFlags_NoScrollbar);
	ImGui::PushItemWidth(50);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	ImGui::InputInt(" Number of sword", &number_of_add_more_sword, 0, 0);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 8);
	ImGui::InputInt(" Number of spear", &number_of_add_more_spear, 0, 0);
	ImGui::PopItemWidth();

	static int padding = 30;
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding);
	ImGui::Image(texture.swordman_texture, sf::Vector2f(50, 50));
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50 - padding);
	ImGui::Image(texture.spear_texture, sf::Vector2f(50, 50));

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + padding + 15);
	ImGui::Text("%i", number_of_add_more_sword);
	ImGui::SameLine(ImGui::GetContentRegionAvail().x - 50 + 15 - padding);
	ImGui::Text("%i", number_of_add_more_spear);

	if (ImGui::Button("ADD TO \nLEFT", ImVec2(ImGui::GetContentRegionAvail().x / 2 - 5, ImGui::GetContentRegionAvail().y))) {
		isAddFlankUnitShow = false;
		activeSlotInteractHandle = true;
		leftFlankField.addSword(number_of_add_more_sword);
		leftFlankField.addSpear(number_of_add_more_spear);
		number_of_add_more_sword = 0;
		number_of_add_more_spear = 0;
	}
	ImGui::SameLine();
	if (ImGui::Button("ADD TO \nRIGHT", ImVec2(ImGui::GetContentRegionAvail().x / 2, ImGui::GetContentRegionAvail().y))) {
		isAddFlankUnitShow = false;
		activeSlotInteractHandle = true;
		rightFlankField.addSword(number_of_add_more_sword);
		rightFlankField.addSpear(number_of_add_more_spear);
		number_of_add_more_sword = 0;
		number_of_add_more_spear = 0;
	}
	ImGui::EndChild();

	ImGui::End();
}

void AddBackUnitAtStartDraw() {
	int boxWidth = 100;
	int margin = 20;
	double startPos = 160;
	double nextOffset = boxWidth + margin;

	ImGui::SetNextWindowPos(ImVec2(startPos, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(960, 200));
	ImGui::Begin("##Add back unit", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

	for (int i = 0; i < 8; i++) {
		std::string childName = "##backitem" + to_string(i);
		int* unitCursor = &backField.unitNumberInEachSlot[i];
		ImGui::BeginChild(childName.c_str(), ImVec2(100, 140), false, ImGuiWindowFlags_NoScrollbar);
		ImGui::PushItemWidth(30);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 3);
		ImGui::InputInt(" Powder", unitCursor, 0, 0);
		if (*unitCursor > 12) *unitCursor = 12;
		else if (*unitCursor < 0) *unitCursor = 0;
		ImGui::PopItemWidth();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
		ImGui::Image(texture.gunner_texture, sf::Vector2f(50, 50));

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
		ImGui::Text("%i", *unitCursor);
		if (*unitCursor == 12) {
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 32);
			ImGui::Text("(MAX)");
		}

		ImGui::EndChild();

		if (i != 7) {
			ImGui::SameLine(nextOffset * (i + 1) + 5);
		}
	}
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 105);
	if (ImGui::Button("NEXT TO MAIN FIELD", ImVec2(200, ImGui::GetContentRegionAvail().y))) {
		isAddBackUnitAtStartDrawShow = false;
		isAddMainUnitAtStartDrawShow = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("MAX", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 8; k++) {
			backField.unitNumberInEachSlot[k] = 12;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("RESET", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 8; k++) {
			backField.unitNumberInEachSlot[k] = 0;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("RANDOM", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 8; k++) {
			backField.unitNumberInEachSlot[k] = rand() % 21;
		}
	}
	ImGui::End();
}

void AddMainUnitAtStartDraw() {
	int boxWidth = 100;
	int margin = 20;
	double startPos = 160;
	double nextOffset = boxWidth + margin;

	ImGui::SetNextWindowPos(ImVec2(startPos, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(960, 250));
	ImGui::Begin("##Add main unit", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

	for (int i = 0; i < 8; i++) {
		std::string childName = "##mainitem" + to_string(i);
		int* unitCursor = &mainField.unitNumberInEachSlot[i];
		ImGui::BeginChild(childName.c_str(), ImVec2(100, 190), false, ImGuiWindowFlags_NoScrollbar);

		double width = ImGui::GetContentRegionAvail().x / 3 - 5;
		if (ImGui::ImageButton(texture.swordman_texture, sf::Vector2f(width, width))) mainField.typeOfUnit[i] = 1;
		ImGui::SameLine(0, 0);
		if (ImGui::ImageButton(texture.spear_texture, sf::Vector2f(width, width))) mainField.typeOfUnit[i] = 2;
		ImGui::SameLine(0, 0);
		if (ImGui::ImageButton(texture.hop_texture, sf::Vector2f(width, width))) mainField.typeOfUnit[i] = 3;

		ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 30);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
		ImGui::InputInt("##", unitCursor, 0, 0);
		if (*unitCursor > 30) *unitCursor = 30;
		else if (*unitCursor < 0) *unitCursor = 0;
		ImGui::PopItemWidth();

		std::string unitName;
		if (mainField.typeOfUnit[i] == 1) unitName = "Swordman";
		else if (mainField.typeOfUnit[i] == 2) unitName = "Spearman";
		else if (mainField.typeOfUnit[i] == 3) unitName = "Hopper";
		if (mainField.typeOfUnit[i] == 3) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
		else ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
		ImGui::Text(unitName.c_str());

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

		if (mainField.typeOfUnit[i] == 1) ImGui::Image(texture.swordman_texture, sf::Vector2f(50, 50));
		else if (mainField.typeOfUnit[i] == 2) ImGui::Image(texture.spear_texture, sf::Vector2f(50, 50));
		else if (mainField.typeOfUnit[i] == 3) ImGui::Image(texture.hop_texture, sf::Vector2f(50, 50));

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
		ImGui::Text("%i", *unitCursor);
		if (*unitCursor == 30) {
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 32);
			ImGui::Text("(MAX)");
		}

		ImGui::EndChild();

		if (i != 7) {
			ImGui::SameLine(nextOffset * (i + 1) + 5);
		}
	}
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 105);
	if (ImGui::Button("NEXT TO FLANK FIELD", ImVec2(200, ImGui::GetContentRegionAvail().y))) {
		mainField.setFieldTexture();
		isAddMainUnitAtStartDrawShow = false;
		isAddFlankUnitAtStartDrawShow = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("MAX", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 8; k++) {
			mainField.unitNumberInEachSlot[k] = 30;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("RESET", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 8; k++) {
			mainField.unitNumberInEachSlot[k] = 0;
			mainField.typeOfUnit[k] = 1;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("RANDOM", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 8; k++) {
			mainField.unitNumberInEachSlot[k] = rand() % 31;
			mainField.typeOfUnit[k] = rand() % 3 + 1;
		}
	}
	ImGui::End();
}

void AddFlankUnitAtStartDraw() {
	int boxWidth = 100;
	int margin = 20;
	double startPos = 160;
	double nextOffset = boxWidth + margin;

	ImGui::SetNextWindowPos(ImVec2(startPos, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(960, 250));
	ImGui::Begin("##Add flank unit", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove);

	for (int i = 0; i < 8; i++) {
		if (i < 3) {
			std::string childName = "##flankitem" + to_string(i);
			int* unitCursor = &leftFlankField.unitNumberInEachSlot[i];
			ImGui::BeginChild(childName.c_str(), ImVec2(100, 190), false, ImGuiWindowFlags_NoScrollbar);

			double width = ImGui::GetContentRegionAvail().x / 2 - 5;
			if (ImGui::ImageButton(texture.swordman_texture, sf::Vector2f(width, width))) leftFlankField.typeOfUnit[i] = 1;
			ImGui::SameLine(0, 0);
			if (ImGui::ImageButton(texture.spear_texture, sf::Vector2f(width, width))) leftFlankField.typeOfUnit[i] = 2;

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 30);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
			ImGui::InputInt("##", unitCursor, 0, 0);
			if (*unitCursor > 20) *unitCursor = 20;
			else if (*unitCursor < 0) *unitCursor = 0;
			ImGui::PopItemWidth();

			std::string unitName;
			if (leftFlankField.typeOfUnit[i] == 1) unitName = "Swordman";
			else if (leftFlankField.typeOfUnit[i] == 2) unitName = "Spear";
			if (leftFlankField.typeOfUnit[i] == 2) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
			else ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
			ImGui::Text(unitName.c_str());

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

			if (leftFlankField.typeOfUnit[i] == 1) ImGui::Image(texture.swordman_texture, sf::Vector2f(50, 50));
			else if (leftFlankField.typeOfUnit[i] == 2) ImGui::Image(texture.spear_texture, sf::Vector2f(50, 50));

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
			ImGui::Text("%i", *unitCursor);
			if (*unitCursor == 20) {
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 32);
				ImGui::Text("(MAX)");
			}

			ImGui::EndChild();
		}
		else if (i > 4) {
			std::string childName = "##flankitem" + to_string(i);
			int index = i - 5;
			int* unitCursor = &rightFlankField.unitNumberInEachSlot[index];
			ImGui::BeginChild(childName.c_str(), ImVec2(100, 190), false, ImGuiWindowFlags_NoScrollbar);

			double width = ImGui::GetContentRegionAvail().x / 2 - 5;
			if (ImGui::ImageButton(texture.swordman_texture, sf::Vector2f(width, width))) rightFlankField.typeOfUnit[index] = 1;
			ImGui::SameLine(0, 0);
			if (ImGui::ImageButton(texture.spear_texture, sf::Vector2f(width, width))) rightFlankField.typeOfUnit[index] = 2;

			ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - 30);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 15);
			ImGui::InputInt("##", unitCursor, 0, 0);
			if (*unitCursor > 20) *unitCursor = 20;
			else if (*unitCursor < 0) *unitCursor = 0;
			ImGui::PopItemWidth();

			std::string unitName;
			if (rightFlankField.typeOfUnit[index] == 1) unitName = "Swordman";
			else if (rightFlankField.typeOfUnit[index] == 2) unitName = "Spear";
			if (rightFlankField.typeOfUnit[index] == 2) ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 25);
			else ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);
			ImGui::Text(unitName.c_str());

			ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 20);

			if (rightFlankField.typeOfUnit[index] == 1) ImGui::Image(texture.swordman_texture, sf::Vector2f(50, 50));
			else if (rightFlankField.typeOfUnit[index] == 2) ImGui::Image(texture.spear_texture, sf::Vector2f(50, 50));

			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 40);
			ImGui::Text("%i", *unitCursor);
			if (*unitCursor == 20) {
				ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 32);
				ImGui::Text("(MAX)");
			}

			ImGui::EndChild();
		}

		if (i != 7) {
			ImGui::SameLine(nextOffset * (i + 1) + 5);
		}
	}
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 105);
	if (ImGui::Button("ENTER GAME", ImVec2(200, ImGui::GetContentRegionAvail().y))) {
		leftFlankField.setFieldTexture();
		rightFlankField.setFieldTexture();
		isAddFlankUnitAtStartDrawShow = false;
		isGameStarted = true;
		activeSlotInteractHandle = true;
		gameMusicStart();
	}
	ImGui::SameLine();
	if (ImGui::Button("MAX", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 3; k++) {
			leftFlankField.unitNumberInEachSlot[k] = 20;
			rightFlankField.unitNumberInEachSlot[k] = 20;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("RESET", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 3; k++) {
			leftFlankField.unitNumberInEachSlot[k] = 0;
			leftFlankField.typeOfUnit[k] = 1;
			rightFlankField.unitNumberInEachSlot[k] = 0;
			rightFlankField.typeOfUnit[k] = 1;
		}
	}
	ImGui::SameLine();
	if (ImGui::Button("RANDOM", ImVec2(50, ImGui::GetContentRegionAvail().y))) {
		for (int k = 0; k < 3; k++) {
			leftFlankField.unitNumberInEachSlot[k] = rand() % 21;
			leftFlankField.typeOfUnit[k] = rand() % 2 + 1;
			rightFlankField.unitNumberInEachSlot[k] = rand() % 21;
			rightFlankField.typeOfUnit[k] = rand() % 2 + 1;
		}
	}

	ImGui::End();
}

void InGameMenuDraw() {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(300, 40));
	ImGui::SetNextWindowBgAlpha(0);
	ImGui::Begin("##InGameMenu", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);

	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 8, ImGui::GetCursorPosY() - 8));
	if (ImGui::Button("MENU", ImVec2(100, 24))) {
		if (isGamePauseShow) {
			activeSlotInteractHandle = true;
			isGamePauseShow = false;
		}
		else {
			activeSlotInteractHandle = false;
			isGamePauseShow = true;
			isInforBoxShow = false;
			isMainInforBoxShow = false;
			isBackInforBoxShow = false;
			isLeftFlankInforBoxShow = false;
			isRightFlankInforBoxShow = false;
		}

		if (isAddFlankUnitShow) isAddFlankUnitShow = false;
		if (gameInformationShow) gameInformationShow = false;
	}
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 6, ImGui::GetCursorPosY() - 6));
	if (ImGui::Button("INFORMATION", ImVec2(100, 30))) {
		if (gameInformationShow) {
			activeSlotInteractHandle = true;
			gameInformationShow = false;
		}
		else {
			activeSlotInteractHandle = false;
			gameInformationShow = true;
			isInforBoxShow = false;
			isMainInforBoxShow = false;
			isBackInforBoxShow = false;
			isLeftFlankInforBoxShow = false;
			isRightFlankInforBoxShow = false;
		}

		if (isAddFlankUnitShow) isAddFlankUnitShow = false;
	}
	ImGui::SameLine();
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() - 6, ImGui::GetCursorPosY() - 6));
	if (ImGui::Button("ADD", ImVec2(100, 30))) {
		if (isAddFlankUnitShow) {
			activeSlotInteractHandle = true;
			isAddFlankUnitShow = false;
		}
		else {
			activeSlotInteractHandle = false;
			isAddFlankUnitShow = true;
			isInforBoxShow = false;
			isMainInforBoxShow = false;
			isBackInforBoxShow = false;
			isLeftFlankInforBoxShow = false;
			isRightFlankInforBoxShow = false;
		}
	}

	ImGui::End();
}

void GameInformationDraw() {
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2));
	ImGui::Begin("                                      GAME INFORMATION", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

	double column_width = ImGui::GetContentRegionAvail().x / 4;
	double height = ImGui::GetContentRegionAvail().y;

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);
	ImGui::Columns(4);
	// POWDER COL -  POWDER COL -  POWDER COL -  POWDER COL -  POWDER COL -  POWDER COL -  POWDER COL
	int powder_damage = backField.getDamage();
	int powder_blood = backField.getBlood();
	int powder_armor = backField.getArmor();

	int* powderNumberDamageBloodArmorMunition = backField.getFieldInfor();

	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + column_width / 2 - column_width / 6);
	ImGui::Image(texture.gunner_texture, sf::Vector2f(column_width / 3, height / 4));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
	ImGui::Text("    POWDER STATS");
	ImGui::Text("\n    DAMAGE - %i", powder_damage);
	ImGui::Text("     BLOOD - %i", powder_blood);
	ImGui::Text("     ARMOR - %i", powder_armor);

	ImGui::Text("\n NUMBER OF UNIT - %i", powderNumberDamageBloodArmorMunition[0]);
	ImGui::Text("  ALL DAMAGE - %i", powderNumberDamageBloodArmorMunition[1]);
	ImGui::Text("  ALL HEALTH - %i", powderNumberDamageBloodArmorMunition[2]);
	ImGui::Text("  ALL ARMOR - %i", powderNumberDamageBloodArmorMunition[3]);
	ImGui::Text(" ALL MUNITION - %i", powderNumberDamageBloodArmorMunition[4]);
	// SWORD COL - SWORD COL - SWORD COL - SWORD COL - SWORD COL - SWORD COL - SWORD COL - SWORD COL - SWORD COL
	ImGui::NextColumn();
	int sword_damage = mainField.swordDamage;
	int sword_blood = mainField.swordBlood;
	int sword_armor = mainField.swordArmor;

	int all_sword_number_unit = 0;
	int all_sword_damage = 0;
	int all_sword_blood = 0;
	int all_sword_armor = 0;

	for (int i = 0; i < 8; i++) {
		if (mainField.getTypeOfUnitAtIndex(i) == 1) {
			all_sword_number_unit += mainField.getNumberOfUnitAtIndex(i);
			all_sword_damage += mainField.getNumberOfUnitAtIndex(i) * sword_damage;
			all_sword_blood += mainField.getNumberOfUnitAtIndex(i) * sword_blood;
			all_sword_armor += mainField.getNumberOfUnitAtIndex(i) * sword_armor;
		}
	}

	for (int i = 0; i < 3; i++) {
		if (leftFlankField.getTypeOfUnitAtIndex(i) == 1) {
			all_sword_number_unit += leftFlankField.getNumberOfUnitAtIndex(i);
			all_sword_damage += leftFlankField.getNumberOfUnitAtIndex(i) * sword_damage;
			all_sword_blood += leftFlankField.getNumberOfUnitAtIndex(i) * sword_blood;
			all_sword_armor += leftFlankField.getNumberOfUnitAtIndex(i) * sword_armor;
		}

		if (rightFlankField.getTypeOfUnitAtIndex(i) == 1) {
			all_sword_number_unit += rightFlankField.getNumberOfUnitAtIndex(i);
			all_sword_damage += rightFlankField.getNumberOfUnitAtIndex(i) * sword_damage;
			all_sword_blood += rightFlankField.getNumberOfUnitAtIndex(i) * sword_blood;
			all_sword_armor += rightFlankField.getNumberOfUnitAtIndex(i) * sword_armor;
		}
	}
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + column_width / 2 - column_width / 6);
	ImGui::Image(texture.swordman_texture, sf::Vector2f(column_width / 3, height / 4));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
	ImGui::Text("    SWORD STATS");
	ImGui::Text("\n    DAMAGE - %i", sword_damage);
	ImGui::Text("     BLOOD - %i", sword_blood);
	ImGui::Text("     ARMOR - %i", sword_armor);

	ImGui::Text("\n NUMBER OF UNIT - %i", all_sword_number_unit);
	ImGui::Text("  ALL DAMAGE - %i", all_sword_damage);
	ImGui::Text("  ALL HEALTH - %i", all_sword_blood);
	ImGui::Text("  ALL ARMOR - %i", all_sword_armor);
	// SPEAR COL - SPEAR COL - SPEAR COL - SPEAR COL - SPEAR COL - SPEAR COL - SPEAR COL - SPEAR COL
	ImGui::NextColumn();
	int spear_damage = mainField.spearDamage;
	int spear_blood = mainField.spearBlood;
	int spear_armor = mainField.spearArmor;

	int all_spear_number_unit = 0;
	int all_spear_damage = 0;
	int all_spear_blood = 0;
	int all_spear_armor = 0;

	for (int i = 0; i < 8; i++) {
		if (mainField.getTypeOfUnitAtIndex(i) == 2) {
			all_spear_number_unit += mainField.getNumberOfUnitAtIndex(i);
			all_spear_damage += mainField.getNumberOfUnitAtIndex(i) * spear_damage;
			all_spear_blood += mainField.getNumberOfUnitAtIndex(i) * spear_blood;
			all_spear_armor += mainField.getNumberOfUnitAtIndex(i) * spear_armor;
		}
	}

	for (int i = 0; i < 3; i++) {
		if (leftFlankField.getTypeOfUnitAtIndex(i) == 2) {
			all_spear_number_unit += leftFlankField.getNumberOfUnitAtIndex(i);
			all_spear_damage += leftFlankField.getNumberOfUnitAtIndex(i) * spear_damage;
			all_spear_blood += leftFlankField.getNumberOfUnitAtIndex(i) * spear_blood;
			all_spear_armor += leftFlankField.getNumberOfUnitAtIndex(i) * spear_armor;
		}

		if (rightFlankField.getTypeOfUnitAtIndex(i) == 2) {
			all_spear_number_unit += rightFlankField.getNumberOfUnitAtIndex(i);
			all_spear_damage += rightFlankField.getNumberOfUnitAtIndex(i) * spear_damage;
			all_spear_blood += rightFlankField.getNumberOfUnitAtIndex(i) * spear_blood;
			all_spear_armor += rightFlankField.getNumberOfUnitAtIndex(i) * spear_armor;
		}
	}
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + column_width / 2 - column_width / 6);
	ImGui::Image(texture.spear_texture, sf::Vector2f(column_width / 3, height / 4));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
	ImGui::Text("    SPEAR STATS");
	ImGui::Text("\n    DAMAGE - %i", spear_damage);
	ImGui::Text("     BLOOD - %i", spear_blood);
	ImGui::Text("     ARMOR - %i", spear_armor);

	ImGui::Text("\n NUMBER OF UNIT - %i", all_spear_number_unit);
	ImGui::Text("  ALL DAMAGE - %i", all_spear_damage);
	ImGui::Text("  ALL HEALTH - %i", all_spear_blood);
	ImGui::Text("  ALL ARMOR - %i", all_spear_armor);
	// HOP COL - HOP COL - HOP COL - HOP COL - HOP COL - HOP COL - HOP COL - HOP COL - HOP COL
	ImGui::NextColumn();
	int hop_damage = mainField.hopDamage;
	int hop_blood = mainField.hopBlood;
	int hop_armor = mainField.hopArmor;

	int all_hop_number_unit = 0;
	int all_hop_damage = 0;
	int all_hop_blood = 0;
	int all_hop_armor = 0;

	for (int i = 0; i < 8; i++) {
		if (mainField.getTypeOfUnitAtIndex(i) == 3) {
			all_hop_number_unit += mainField.getNumberOfUnitAtIndex(i);
			all_hop_damage += mainField.getNumberOfUnitAtIndex(i) * hop_damage;
			all_hop_blood += mainField.getNumberOfUnitAtIndex(i) * hop_blood;
			all_hop_armor += mainField.getNumberOfUnitAtIndex(i) * hop_armor;
		}
	}
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + column_width / 2 - column_width / 6);
	ImGui::Image(texture.hop_texture, sf::Vector2f(column_width / 3, height / 4));
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
	ImGui::Text("    HOP STATS");
	ImGui::Text("\n    DAMAGE - %i", hop_damage);
	ImGui::Text("     BLOOD - %i", hop_blood);
	ImGui::Text("     ARMOR - %i", hop_armor);

	ImGui::Text("\n NUMBER OF UNIT - %i", all_hop_number_unit);
	ImGui::Text("  ALL DAMAGE - %i", all_hop_damage);
	ImGui::Text("  ALL HEALTH - %i", all_hop_blood);
	ImGui::Text("  ALL ARMOR - %i", all_hop_armor);
	/// <summary>
	/// ENDD INFORMATION TABLE
	/// </summary>
	ImGui::End();
}

void AreYouSureToQuit() {
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f + 70, ImGui::GetIO().DisplaySize.y * 0.5f), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
	ImGui::SetNextWindowSize(ImVec2(300, 120));
	ImGui::Begin("##AreYouSureToQuit", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoTitleBar);
	ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 2 - 70, ImGui::GetCursorPosY() + ImGui::GetContentRegionAvail().y / 2 - 40));
	ImGui::Text("ARE YOU SURE TO QUIT");
	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 10);
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() + ImGui::GetContentRegionAvail().x / 4 + 25);
	if (ImGui::Button("YES", ImVec2(ImGui::GetContentRegionAvail().x / 2, 40))) {
		isGameClose = true;
	};
	ImGui::End();
}