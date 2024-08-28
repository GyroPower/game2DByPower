#pragma once
#include"entity/player/player.h"
#include<glm/glm.hpp>
#include<ImGui/imgui.h>
#include<string>

class GUI {

public:
	GUI();

	void showVec(std::string vecName, glm::vec3 vec, bool* open = NULL);
	void showTwoValues(std::string title, float val1, float val2, std::string nameVal1 = "val1",
		std::string nameVal2 = "val2", bool* open = NULL);
	void useSliderForVec2(std::string vecName, glm::vec2& vec,bool* open = NULL);
	void useSliderForFloat(std::string valName, float& val, bool* open = NULL);
	void showSingleVal(std::string valName, float& val, bool* open = NULL);
	void enableBool(std::string name, bool& value, bool* open = NULL);
	void enableTileMapEditing(bool& enable, bool& enableGrid, bool* open = NULL);
	void showBoolVar(std::string valName, bool& val, bool* open = NULL);
	void m_DebugPlayer(Player& player);

};