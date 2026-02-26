#pragma once
#include "entity/player/player.h"
#include <glm/glm.hpp>
#include <imgui.h>
#include <string>

class EditorGui {

public:
  EditorGui();
  ~EditorGui();

  void showVec(std::string vecName, glm::vec3 vec, bool *open = nullptr);
  void showVec(std::string vecName, glm::vec4 vec, bool *open = nullptr);
  void showTwoValues(std::string title, float val1, float val2,
                     std::string nameVal1 = "val1",
                     std::string nameVal2 = "val2", bool *open = nullptr);
  void useSliderForVec2(std::string vecName, glm::vec2 &vec,
                        bool *open = nullptr);
  void useSliderForFloat(std::string valName, float &val, bool *open = nullptr);
  void showSingleVal(std::string valName, float &val, bool *open = nullptr);
  void enableBool(std::string name, bool &value, bool *open = nullptr);
  void enableDisable(std::string name, bool &enable, bool &disable,
                     bool *open = nullptr);
  void enableTileMapEditing(bool &enable, bool &enableGrid, bool &spawnMobs,
                            bool *open = nullptr);
  void showBoolVar(std::string valName, bool &val, bool *open = nullptr);
  void m_DebugPlayer(Player *player);

  void m_spawnMobs(glm::vec3 pos);
};
