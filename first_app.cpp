#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "ssp_camera.hpp"
#include "simple_render_system.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// std
#include <array>
#include <chrono>
#include <stdexcept>

namespace ssp {

FirstApp::FirstApp() {
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {
  SimpleRenderSystem simpleRenderSystem{sspDevice, sspRenderer.getSwapChainRenderPass()};
  SspCamera camera{};
  camera.setViewTarget(glm::vec3(-1.0f,-2.0f,2.0f),glm::vec3(0.0f,0.0f,2.5f));
  
  auto viewerObject = SspGameObject::createGameObject();
  KeyboardMovementController cameraController{};

  auto currentTime = std::chrono::high_resolution_clock::now();

  while (!sspWindow.shouldClose()) {
    glfwPollEvents();

    auto newTime = std::chrono::high_resolution_clock::now();
    float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime-currentTime).count();
    currentTime = newTime;

    cameraController.moveInPlaneXZ(sspWindow.getGLFWwindow(), frameTime, viewerObject);
    camera.setViewYXZ(viewerObject.transform.translation,viewerObject.transform.rotation);
    float aspect = sspRenderer.getAspectRatio();
    camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

    if(auto commandBuffer = sspRenderer.beginFrame()){
      sspRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects,camera);
      sspRenderer.endSwapChainRenderPass(commandBuffer);
      sspRenderer.endFrame();
    }
  }

  vkDeviceWaitIdle(sspDevice.device());
}



void FirstApp::loadGameObjects() {
  std::shared_ptr<SspModel> sspModel = SspModel::createModelFromFile(sspDevice, "models/smooth_vase.obj");

  auto gameObject = SspGameObject::createGameObject();
  gameObject.model = sspModel;
  gameObject.transform.translation = {0.0f, 0.0f, 2.5f};
  gameObject.transform.scale = glm::vec3{3.f};
  gameObjects.push_back(std::move(gameObject));
}

} // namespace ssp