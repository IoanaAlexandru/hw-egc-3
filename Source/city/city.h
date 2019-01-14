#pragma once
#include <Component/SimpleScene.h>
#include <Component/Transform/Transform.h>
#include <Core/GPU/Mesh.h>
#include "object.h"

#include <map>

namespace newvegas {
enum AreaType { GREEN_AREA, BUILDING_AREA, PARKING_LOT };

class City : public SimpleScene {
 public:
  City();
  ~City();

  void Init() override;

  void CreateBuilding(Texture2D* texture, int pos_x, int pos_y);

 private:
  void FrameStart() override;
  void Update(float deltaTimeSeconds) override;
  void FrameEnd() override;

  void RenderSimpleMesh(Mesh* mesh, Shader* shader,
                        const glm::mat4& modelMatrix,
                        Texture2D* texture = NULL);
  std::vector<std::vector<int>> CreateMap();
  Texture2D* CreateRandomTexture(unsigned int width, unsigned int height);

  void OnInputUpdate(float deltaTime, int mods) override;
  void OnKeyPress(int key, int mods) override;
  void OnKeyRelease(int key, int mods) override;
  void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
  void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
  void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
  void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
  void OnWindowResize(int width, int height) override;

  std::vector<std::vector<int>> map;  // 0 -> street; 1 -> building; 2-> processed
  const int kMapSize = 50, kMaxStreets = 150, kMaxStreetLength = 200;
  std::vector<Texture2D*> textures;
  std::vector<Object*> objects;
};
}  // namespace newvegas
