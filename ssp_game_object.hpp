#pragma once

#include "ssp_model.hpp"

// std
#include <memory>

namespace ssp
{
struct Transform2dComponent{
    glm::vec2 translation{}; // {position offset}
    glm::vec2 scale{1.f,1.f};
    float rotation;

    glm::mat2 mat2() { 
        const float s =glm::sin(rotation);
        const float c =glm::cos(rotation);
        glm::mat2 rotMatrix{{c,s},{-s,c}};

        glm::mat2 scaleMat{{scale.x, 0.0f},{0.0f, scale.y}};
        return rotMatrix * scaleMat;
    }
};

  class SspGameObject{
    public:
    using id_t = unsigned int;

    static SspGameObject createGameObject(){
        static id_t currentId = 0;
        return SspGameObject{currentId++};
    }

    SspGameObject(const SspGameObject &)=delete;
    SspGameObject &operator=(const SspGameObject &) = delete;
    SspGameObject(SspGameObject &&) =default;
    SspGameObject &operator=(SspGameObject &&) = default;

    id_t getId() const {return id;}

    std::shared_ptr<SspModel> model{};
    glm::vec3 color{};
    Transform2dComponent transform2d{};

    private:
    id_t id;

    SspGameObject(id_t objId): id{objId} {}
  };
} // namespace ssp
