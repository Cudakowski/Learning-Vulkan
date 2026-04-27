#include "first_app.hpp"

#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <stdexcept>

namespace ssp {

FirstApp::FirstApp() {
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{sspDevice, sspRenderer.getSwapChainRenderPass()};

  while (!sspWindow.shouldClose()) {
    glfwPollEvents();
    
    if(auto commandBuffer = sspRenderer.beginFrame()){
      sspRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
      sspRenderer.endSwapChainRenderPass(commandBuffer);
      sspRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(sspDevice.device());
}




    // temporary helper function, creates a 1x1x1 cube centered at offset
    std::unique_ptr<SspModel> createCubeModel(SspDevice& device, glm::vec3 offset) {
      std::vector<SspModel::Vertex> vertices{
     
          // left face (white)
          {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
          {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
          {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
          {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
          {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
          {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
     
          // right face (yellow)
          {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
          {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
          {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
          {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
          {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
          {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
     
          // top face (orange, remember y axis points down)
          {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
          {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
          {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
          {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
          {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
          {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
     
          // bottom face (red)
          {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
          {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
          {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
          {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
          {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
          {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
     
          // nose face (blue)
          {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
          {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
          {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
          {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
          {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
          {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
     
          // tail face (green)
          {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
          {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
          {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
          {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
          {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
          {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
     
      };
      for (auto& v : vertices) {
        v.position += offset;
      }
      return std::make_unique<SspModel>(device, vertices);
    }




void FirstApp::loadGameObjects() {
  std::shared_ptr<SspModel> sspModel = createCubeModel(sspDevice, {0.0f, 0.0f, 0.0f });

  auto cube = SspGameObject::createGameObject();
  cube.model = sspModel;
  cube.transform.translation = {0.0f, 0.0f, 0.5f};
  cube.transform.scale = {0.5f, 0.5f, 0.5f};
  gameObjects.push_back(std::move(cube));
}

} // namespace ssp