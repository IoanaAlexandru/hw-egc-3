#include "city.h"

#include <iostream>
#include <string>
#include <vector>

#include <Core/Engine.h>

namespace newvegas {
City::City() {}

City::~City() {}

void City::Init() {
  // Load meshes
  {
    Mesh* mesh = new Mesh("box");
    mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
    meshes[mesh->GetMeshID()] = mesh;
  }

  // Create a shader program for drawing face polygon with the color of the
  // normal
  {
    Shader* shader = new Shader("CityShader");
    shader->AddShader("Source/city/shaders/VertexShader.glsl",
                      GL_VERTEX_SHADER);
    shader->AddShader("Source/city/shaders/FragmentShader.glsl",
                      GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
  }

  // Create map and place camera in the middle
  {
    map = CreateMap();
    for (auto row : map) {
      for (auto pos : row) {
        if (pos) {
          std::cout << 1;
        } else {
          std::cout << 0;
        }
      }
      std::cout << std::endl;
    }

    GetSceneCamera()->SetPosition(glm::vec3(kMapSize / 2, 1, kMapSize / 2));
  }

  // Init textures to choose randomly from
  {
    textures.push_back(TextureManager::GetTexture("building1.jpg"));
    textures.push_back(TextureManager::GetTexture("building2.jpg"));
    textures.push_back(TextureManager::GetTexture("building3.jpg"));
    textures.push_back(TextureManager::GetTexture("building4.jpg"));
    textures.push_back(TextureManager::GetTexture("building5.jpg"));
  }

  {
    for (int i = 0; i < map.size(); i++) {
      for (int j = 0; j < map[i].size(); j++) {
        glm::mat4 model_matrix = glm::mat4(1);
        model_matrix = glm::translate(model_matrix, glm::vec3(i, 0, j));

        if (map[i][j] == 1) {
          if (rand() % 10 > 1) {
            Texture2D* texture = textures.at(rand() % textures.size());
            CreateBuilding(texture, i, j);
          } else {
            Texture2D* texture = TextureManager::GetTexture("grass.jpg");
            CreatePark(texture, i, j);
          }
        } else if (map[i][j] == 0) {
          Texture2D* texture = TextureManager::GetTexture("asphalt.jpg");
          model_matrix = glm::scale(model_matrix, glm::vec3(1, 0.1, 1));
          objects.push_back(new Object(
              "street_" + std::to_string(i) + "_" + std::to_string(j),
              RESOURCE_PATH::MODELS + "Primitives", "box.obj", texture,
              model_matrix));
          map[i][j] = 2;
        }
      }
    }
  }
}

void City::CreateBuilding(Texture2D* texture, int pos_x, int pos_y) {
  if (pos_x < 0 || pos_x >= kMapSize || pos_y < 0 || pos_y >= kMapSize) return;
  if (map[pos_x][pos_y] != 1) return;

  float random_height = rand() % 4 + 1;
  for (float i = 0.5; i < random_height; i++) {
    glm::mat4 model_matrix = glm::mat4(1);
    model_matrix = glm::translate(model_matrix, glm::vec3(pos_x, i, pos_y));

    objects.push_back(new Object(
        "building_" + std::to_string(pos_x) + "_" + std::to_string(pos_y),
        RESOURCE_PATH::MODELS + "Primitives", "box.obj", texture,
        model_matrix));
  }
  map[pos_x][pos_y] = 2;

  CreateBuilding(texture, pos_x + 1, pos_y);
  CreateBuilding(texture, pos_x - 1, pos_y);
  CreateBuilding(texture, pos_x, pos_y + 1);
  CreateBuilding(texture, pos_x, pos_y - 1);
}

void City::CreatePark(Texture2D* texture, int pos_x, int pos_y) {
  if (pos_x < 0 || pos_x >= kMapSize || pos_y < 0 || pos_y >= kMapSize) return;
  if (map[pos_x][pos_y] != 1) return;

  glm::mat4 pos_matrix = glm::mat4(1);
  pos_matrix = glm::translate(pos_matrix, glm::vec3(pos_x, 0, pos_y));
  glm::mat4 model_matrix = glm::scale(pos_matrix, glm::vec3(1, 0.05, 1));

  objects.push_back(new Object(
      "park_" + std::to_string(pos_x) + "_" + std::to_string(pos_y),
      RESOURCE_PATH::MODELS + "Primitives", "box.obj", texture, model_matrix));
  if (rand() % 10 < 3) {
    if (rand() % 10 <= 5) {
      objects.push_back(new Object(
          "tree_trunk_" + std::to_string(pos_x) + "_" + std::to_string(pos_y),
          RESOURCE_PATH::MODELS + "Vegetation", "trunk.obj",
          TextureManager::GetTexture("bark.jpg"),
          glm::scale(pos_matrix, glm::vec3(0.1, 0.1, 0.1))));
    } else if (rand() % 10 <= 5) {
      objects.push_back(new Object(
          "tree_" + std::to_string(pos_x) + "_" + std::to_string(pos_y),
          RESOURCE_PATH::MODELS + "Vegetation", "bush.obj",
          TextureManager::GetTexture("palm.png"),
          glm::scale(pos_matrix, glm::vec3(0.005, 0.005, 0.005))));
    } else {
      objects.push_back(new Object(
          "tree_" + std::to_string(pos_x) + "_" + std::to_string(pos_y),
          RESOURCE_PATH::MODELS + "Vegetation/Bamboo/", "bamboo.obj",
          TextureManager::GetTexture("bamboo.png"),
          glm::scale(pos_matrix, glm::vec3(0.05, 0.05, 0.05))));
    }
  }
  map[pos_x][pos_y] = 2;

  CreatePark(texture, pos_x + 1, pos_y);
  CreatePark(texture, pos_x - 1, pos_y);
  CreatePark(texture, pos_x, pos_y + 1);
  CreatePark(texture, pos_x, pos_y - 1);
}  // namespace newvegas

void City::FrameStart() {
  // clears the color buffer (using the previously set color) and depth buffer
  glClearColor(0, 0, 0, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::ivec2 resolution = window->GetResolution();
  // sets the screen area where to draw
  glViewport(0, 0, resolution.x, resolution.y);
}

void City::Update(float deltaTimeSeconds) {
  for (auto o : objects) {
    RenderSimpleMesh((Mesh*)o, shaders["CityShader"], o->GetModelMatrix(),
                     o->GetTexture());
  }
}

void City::FrameEnd() {}

void City::RenderSimpleMesh(Mesh* mesh, Shader* shader,
                            const glm::mat4& modelMatrix, Texture2D* texture) {
  if (!mesh || !shader || !shader->GetProgramID()) return;

  // render an object using the specified shader and the specified position
  glUseProgram(shader->program);

  // Bind model matrix
  GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
  glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE,
                     glm::value_ptr(modelMatrix));

  // Bind view matrix
  glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
  int loc_view_matrix = glGetUniformLocation(shader->program, "View");
  glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

  // Bind projection matrix
  glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
  int loc_projection_matrix =
      glGetUniformLocation(shader->program, "Projection");
  glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE,
                     glm::value_ptr(projectionMatrix));

  if (texture) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture->GetTextureID());
    glUniform1i(glGetUniformLocation(shader->program, "texture"), 0);
  }

  // Draw the object
  glBindVertexArray(mesh->GetBuffers()->VAO);
  glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()),
                 GL_UNSIGNED_SHORT, 0);
}

std::vector<std::vector<int>> City::CreateMap() {
  int dimensions = kMapSize, max_streets = kMaxStreets,
      max_length = kMaxStreetLength;
  auto map = std::vector<std::vector<int>>(dimensions,
                                           std::vector<int>(dimensions, 1));

  // Start from center
  int current_row = kMapSize / 2;
  int current_col = kMapSize / 2;

  std::vector<std::pair<int, int>> directions;
  directions.emplace_back(-1, 0);
  directions.emplace_back(1, 0);
  directions.emplace_back(0, -1);
  directions.emplace_back(0, 1);

  std::pair<int, int> last_direction, random_direction;

  while (max_streets && dimensions && max_length) {
    do {
      random_direction = directions[rand() % 4];
    } while ((random_direction.first == -last_direction.first &&
              random_direction.second == -last_direction.second) ||
             (random_direction.first == last_direction.first &&
              random_direction.second == last_direction.second));

    int random_length = rand() % max_length + 1, street_length = 0;
    while (street_length < random_length) {
      if (((current_row == 0) && (random_direction.first == -1)) ||
          ((current_col == 0) && (random_direction.second == -1)) ||
          ((current_row == dimensions - 1) && (random_direction.first == 1)) ||
          ((current_col == dimensions - 1) && (random_direction.second == 1))) {
        break;
      } else {
        map[current_row][current_col] = 0;
        current_row += random_direction.first;
        current_col += random_direction.second;
        street_length++;
      }
    }

    if (street_length) {
      last_direction = random_direction;
      max_streets--;
    }
  }

  return map;
}

Texture2D* City::CreateRandomTexture(unsigned int width, unsigned int height) {
  GLuint textureID = 0;
  unsigned int channels = 3;
  unsigned int size = width * height * channels;
  unsigned char* data = new unsigned char[size];

  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);

  glPixelStorei(GL_PACK_ALIGNMENT, 1);
  CheckOpenGLError();

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, data);

  glGenerateMipmap(GL_TEXTURE_2D);

  CheckOpenGLError();

  // Save the texture into a wrapper Texture2D class for using easier later
  // during rendering phase
  Texture2D* texture = new Texture2D();
  texture->Init(textureID, width, height, channels);

  SAFE_FREE_ARRAY(data);
  return texture;
}

void City::OnInputUpdate(float deltaTime, int mods) {
  float speed = 2;

  if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 right = GetSceneCamera()->transform->GetLocalOXVector();
    glm::vec3 forward = GetSceneCamera()->transform->GetLocalOZVector();
    forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
  }
}

void City::OnKeyPress(int key, int mods) {
  // add key press event
}

void City::OnKeyRelease(int key, int mods) {
  // add key release event
}

void City::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
  // add mouse move event
}

void City::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {
  // add mouse button press event
}

void City::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {
  // add mouse button release event
}

void City::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void City::OnWindowResize(int width, int height) {}
}  // namespace newvegas
