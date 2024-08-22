#include"Ui.h"

UI::UI() {}

// for debug porpuse
void UI::showVec(std::string vecName, glm::vec3 vec) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(vecName.c_str(), NULL, flags)) {
		ImGui::Text("X = ");
		ImGui::SameLine(0, 0);
		ImGui::Text(std::to_string(vec.x).c_str());

		ImGui::SameLine(0, 5);
		
		ImGui::Text("Y = ");
		ImGui::SameLine(0, 0);
		ImGui::Text(std::to_string(vec.y).c_str());

		ImGui::SameLine(0, 5);

		ImGui::Text("Z = ");
		ImGui::SameLine(0, 0);
		ImGui::Text(std::to_string(vec.z).c_str());

	}
	ImGui::End();


}

void UI::showTwoValues(std::string title, float val1, float val2, std::string nameVal1,
	std::string nameVal2) {

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;
	nameVal1 += " = ";
	nameVal2 += " = ";
	if (ImGui::Begin(title.c_str(), NULL, flags)) {
		ImGui::Text(nameVal1.c_str());
		ImGui::SameLine(0, 0);
		ImGui::Text(std::to_string(val1).c_str());

		ImGui::Text(nameVal2.c_str());
		ImGui::SameLine(0, 0);
		ImGui::Text(std::to_string(val2).c_str());
	}
	ImGui::End();
}

void UI::showSingleVal(std::string valName, float& val) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;

	if (ImGui::Begin(valName.c_str(), NULL, flags)) {
		valName += ":";
		ImGui::Text(valName.c_str());
		ImGui::SameLine(0, 5);
		ImGui::Text(std::to_string(val).c_str());
	}
	ImGui::End();
}
void UI::useSliderForVec2(std::string name, glm::vec2& vec) {

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(name.c_str(), NULL, flags)) {
		ImGui::SliderFloat("x = ", &vec.x, 0.0f, 1.0f);
		ImGui::SliderFloat("y = ", &vec.y, 0.0f, 1.0f);
	}
	ImGui::End();

}

void UI::useSliderForFloat(std::string valName, float& val) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(valName.c_str(), NULL, flags)) {
		ImGui::SliderFloat(valName.c_str(), &val, 1.0f, 10.0f);
	}
	ImGui::End();
}

void UI::enableBool(std::string name, bool& value)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(name.c_str(), NULL, flags))
	{
		ImGui::Checkbox(name.c_str(), &value);
	}
	ImGui::End();
}

void UI::enableTileMapEditing(bool& enable, bool& enableGrid)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("TileMap options", NULL, flags))
	{
		ImGui::Checkbox("Show TileMap", &enableGrid);
		ImGui::Checkbox("Enable TileMap editing", &enable);
	}
	ImGui::End();
}

void UI::showBoolVar(std::string valName, bool& val)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(valName.c_str(), NULL, flags))
	{
		ImGui::Text(valName.c_str());
		ImGui::SameLine(0, 0);
		ImGui::Text(": ");
		ImGui::SameLine(0, 2);
		ImGui::Text(val ? "True" : "False");
	}
	ImGui::End();
}