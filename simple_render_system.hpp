#pragma once

#include "ssp_camera.hpp"
#include "ssp_device.hpp"
#include "ssp_game_object.hpp"
#include "ssp_pipeline.hpp"

#include <memory>
#include <vector>

namespace ssp {
class SimpleRenderSystem {
public:
  SimpleRenderSystem(SspDevice &device, VkRenderPass renderPass);
  ~SimpleRenderSystem();

  SimpleRenderSystem(const SimpleRenderSystem &) = delete;
  SimpleRenderSystem &operator=(const SimpleRenderSystem &) = delete;

  void renderGameObjects(
      VkCommandBuffer commandBuffer,
      std::vector<SspGameObject> &gameObjects,
      const SspCamera &camera);

private:
  void createPipelineLayout();
  void createPipeline(VkRenderPass renderPass);

  SspDevice &sspDevice;
  std::unique_ptr<SspPipeline> sspPipeline;
  VkPipelineLayout pipelineLayout;
};

} // namespace ssp