#include "first_app.hpp"

#include "keyboard_movement_controller.hpp"
#include "ssp_camera.hpp"
#include "simple_render_system.hpp"
#include "ssp_buffer.hpp"

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

struct GlobalUbo {
  alignas(16) glm::mat4 projectionView{1.f};
  alignas(16) glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f,-3.f,-1.f});
};

FirstApp::FirstApp() {
  globalPool = SspDescriptorPool::Builder(sspDevice)
    .setMaxSets(SspSwapChain::MAX_FRAMES_IN_FLIGHT)
    .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,SspSwapChain::MAX_FRAMES_IN_FLIGHT)
    .build();
  loadGameObjects();
}

FirstApp::~FirstApp() {}

void FirstApp::run() {

  std::vector<std::unique_ptr<SspBuffer>> uboBuffers(SspSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i = 0; i < uboBuffers.size(); i++)
  {
    uboBuffers[i] = std::make_unique<SspBuffer>(
      sspDevice,
      sizeof(GlobalUbo),
      1,
      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
      //VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    ); 

    uboBuffers[i]->map();
  }

  auto globalSetLayout = SspDescriptorSetLayout::Builder(sspDevice)
    .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
    .build();

  std::vector<VkDescriptorSet> globalDescriptorSets(SspSwapChain::MAX_FRAMES_IN_FLIGHT);
  for (int i{0}; i < globalDescriptorSets.size(); ++i ){
    auto bufferInfo = uboBuffers[i]->descriptorInfo();
    SspDescriptorWriter(*globalSetLayout, *globalPool)
      .writeBuffer(0, &bufferInfo)
      .build(globalDescriptorSets[i]);
  }

  SimpleRenderSystem simpleRenderSystem{sspDevice, sspRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
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
      int frameIndex = sspRenderer.getFrameIndex();
      FrameInfo frameInfo{frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex]};

      // update
      GlobalUbo ubo{};
      ubo.projectionView = camera.getProjection() * camera.getView();
      uboBuffers[frameIndex]->writeToBuffer(&ubo);
      uboBuffers[frameIndex]->flush();

      // render
      sspRenderer.beginSwapChainRenderPass(commandBuffer);
      simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
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