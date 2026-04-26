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

void FirstApp::loadGameObjects() {
  std::vector<SspModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
  };

  auto sspModel = std::make_shared<SspModel>(sspDevice, vertices);

  auto triangle = SspGameObject::createGameObject();
  triangle.model = sspModel;
  triangle.color = {0.1f, 0.8f, 0.1f};
  triangle.transform2d.translation.x = 0.2f;
  triangle.transform2d.scale = {2.f,.5f};
  triangle.transform2d.rotation =  .25f * glm::two_pi<float>();

  gameObjects.push_back(std::move(triangle));
}

} // namespace ssp