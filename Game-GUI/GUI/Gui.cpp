#include"Gui.h"

GUI::GUI() {}

// all the methods except debugPlayer can be used for debug variables and vectors
// instead of glm vecs i should use the vecs given with ImGui to use in every wherer this class with 
// ImGui, i will change this 

void GUI::showVec(std::string vecName, glm::vec3 vec, bool* open) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(vecName.c_str(), open, flags)) {
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

void GUI::showTwoValues(std::string title, float val1, float val2, std::string nameVal1,
	std::string nameVal2, bool* open) {

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

void GUI::showSingleVal(std::string valName, float& val, bool* open) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;

	if (ImGui::Begin(valName.c_str(), NULL, flags)) {
		valName += ":";
		ImGui::Text(valName.c_str());
		ImGui::SameLine(0, 5);
		ImGui::Text(std::to_string(val).c_str());
	}
	ImGui::End();
}
void GUI::useSliderForVec2(std::string name, glm::vec2& vec, bool* open) {

	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(name.c_str(), NULL, flags)) {
		ImGui::SliderFloat("x = ", &vec.x, 0.0f, 1.0f);
		ImGui::SliderFloat("y = ", &vec.y, 0.0f, 1.0f);
	}
	ImGui::End();

}

void GUI::useSliderForFloat(std::string valName, float& val, bool* open) {
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(valName.c_str(), NULL, flags)) {
		ImGui::SliderFloat(valName.c_str(), &val, 1.0f, 10.0f);
	}
	ImGui::End();
}

void GUI::enableBool(std::string name, bool& value, bool* open)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin(name.c_str(), NULL, flags))
	{
		ImGui::Checkbox(name.c_str(), &value);
	}
	ImGui::End();
}

void GUI::enableTileMapEditing(bool& enable, bool& enableGrid, bool* open)
{
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse;

	if (ImGui::Begin("TileMap options", NULL, flags))
	{
		ImGui::Checkbox("Show TileMap", &enableGrid);
		ImGui::Checkbox("Enable TileMap editing", &enable);
	}
	ImGui::End();
}

void GUI::showBoolVar(std::string valName, bool& val, bool* open)
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

void GUI::m_DebugPlayer(Player& player)
{
	// this method is to show all the attributes of the player to show for debugging
	// to see if something is off or not working
	ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar;

	static bool showPlayerHitboxPos = false;
	static bool showPlayerHitboxSize = false;
	static bool showPlayerHitboxPosPlusSize = false;
	static bool showPlayerTouchWall = false;
	static bool showPlayerGrounded = false;
	static bool showHitbox = false;

	ImGui::Begin("Player debug", NULL, flags);

	ImGui::Checkbox("show Player Hitbox position", &showPlayerHitboxPos);
	ImGui::Checkbox("show Player Hitbox size", &showPlayerHitboxSize);
	ImGui::Checkbox("show Player Hitbox position plus Size", &showPlayerHitboxPosPlusSize);
	ImGui::Checkbox("show Player is touching a wall", &showPlayerTouchWall);
	ImGui::Checkbox("show Player is grounded", &showPlayerGrounded);
	ImGui::Checkbox("show Player Hitbox", &showHitbox);

	if (player.m_returnVisibilityHitbox() != showHitbox)
		player.m_setVisibilityHitbox(showHitbox);


	if (showPlayerHitboxPos) this->showVec("Player hitbox position", glm::vec3(player.m_getEntityRect().pos, 0.0f), &showPlayerHitboxPos);
	if (showPlayerHitboxSize) this->showVec("Player hitbox size", glm::vec3(player.m_getEntityRect().size, 0.0f), &showPlayerHitboxSize);
	showHitbox ? player.m_setVisibilityHitbox(true) : player.m_setVisibilityHitbox(false);
	if (showPlayerHitboxPosPlusSize) this->showVec("Player hitbox plus size", glm::vec3(player.m_getEntityRect().pos + player.m_getEntityRect().size, 0.0), &showPlayerHitboxPosPlusSize);
	if (showPlayerTouchWall) this->showBoolVar("Player touching a wall", player.m_wallTouch, &showPlayerTouchWall);
	if (showPlayerGrounded) this->showBoolVar("Player is grounded", player.m_grounded, &showPlayerGrounded);


	ImGui::End();

	
}