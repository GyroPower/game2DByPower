#pragma once
#include<ImGui/imgui.h>
#include<string>
#include<glm/glm.hpp>

class UI {

public:
	UI();

	void showVec(std::string vecName, glm::vec3 vec);
	void showTwoValues(std::string title, float val1, float val2, std::string nameVal1 = "val1",
		std::string nameVal2 = "val2");
	void useSliderForVec2(std::string vecName, glm::vec2& vec);
	void useSliderForFloat(std::string valName, float& val);
	void showSingleVal(std::string valName, float& val);
	void enableBool(std::string name, bool& value);
	void enableTileMapEditing(bool& enable, bool& enableGrid);
	void showBoolVar(std::string valName, bool& val);

};