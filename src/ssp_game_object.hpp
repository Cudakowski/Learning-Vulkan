#pragma once

#include "ssp_model.hpp"

// libs
#include <glm/gtc/matrix_transform.hpp>

// std
#include <memory>
#include <unordered_map>

namespace ssp
{
struct TransformComponent{
  glm::vec3 translation{}; // {position offset}
  glm::vec3 scale{1.f,1.f, 1.f};
  glm::vec3 rotation;



  // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
  // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
  // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
  glm::mat4 mat4();
  glm::mat3 normalMatrix();


};

struct PointLightComponent {
  float lightIntensity = 1.0f;
};

class SspGameObject{
  public:
  using id_t = unsigned int;
  using Map = std::unordered_map<id_t, SspGameObject>;

  static SspGameObject createGameObject(){
      static id_t currentId = 0;
      return SspGameObject{currentId++};
  }

  static SspGameObject makePointLight(float intensity =10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

  SspGameObject(const SspGameObject &)=delete;
  SspGameObject &operator=(const SspGameObject &) = delete;
  SspGameObject(SspGameObject &&) =default;
  SspGameObject &operator=(SspGameObject &&) = default;

  id_t getId() const {return id;}

  glm::vec3 color{};
  TransformComponent transform{};
  
  // Optional pointer components
  std::shared_ptr<SspModel> model{};
  std::unique_ptr<PointLightComponent> pointLight = nullptr;

  private:
  id_t id;

  SspGameObject(id_t objId): id{objId} {}
};

} // namespace ssp
