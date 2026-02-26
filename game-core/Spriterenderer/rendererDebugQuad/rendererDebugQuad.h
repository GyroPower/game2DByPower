#pragma once
#include "camera/camera.h"
#include "entity/entity.h"
#include "shaderProgram/shader.h"
#include <memory>
#include <vector>

class rendererDebugQuad {

public:
  rendererDebugQuad() {}
  rendererDebugQuad(Shader &shader, std::string Name);
  ~rendererDebugQuad();

  void reserveData(int sizeToReserve = 1000);
  void initData(std::vector<std::unique_ptr<Entity2D_Instaciaded>> &entities);
  void updateData(Entity2D_Instaciaded *entity);
  void emptyData();
  void draw(Camera &camera);

private:
  Shader m_shader;
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_VBO_pos;
  GLuint m_VBO_color;
  GLuint m_IBO;

  std::string m_rendererName;

  int m_instances = 0;
  int m_sizeReserved = 0;
  bool m_initRender = false;
  GLuint m_indices = 0;
};
