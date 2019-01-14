#pragma once
#include <Core/GPU/Mesh.h>

namespace newvegas {
class Object : Mesh {
 public:
  Object(std::string name, std::string file_location, std::string file_name, Texture2D* texture, glm::mat4 model_matrix)
      : Mesh(name) {
    LoadMesh(file_location, file_name);
    texture_ = texture;
    model_matrix_ = model_matrix;
  };
  ~Object() {}

  inline Texture2D* GetTexture() { return texture_; }
  inline glm::mat4 GetModelMatrix() { return model_matrix_; }

 private:
  Texture2D* texture_;
  glm::mat4 model_matrix_;
};
}  // namespace newvegas