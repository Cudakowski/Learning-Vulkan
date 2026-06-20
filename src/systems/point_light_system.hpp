#pragma once

#include "ssp_camera.hpp"
#include "ssp_device.hpp"
#include "ssp_game_object.hpp"
#include "ssp_pipeline.hpp"
#include "ssp_frame_info.hpp"

#include <memory>
#include <vector>

namespace ssp {
class PointLightSystem {
public:
  PointLightSystem(SspDevice &device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
  ~PointLightSystem();

  PointLightSystem(const PointLightSystem &) = delete;
  PointLightSystem &operator=(const PointLightSystem &) = delete;

  void update(FrameInfo& frameInfo, GlobalUbo &ubo);
  void render(FrameInfo& frameInfo);

private:
  void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
  void createPipeline(VkRenderPass renderPass);

  SspDevice &sspDevice;
  std::unique_ptr<SspPipeline> sspPipeline;
  VkPipelineLayout pipelineLayout;
};

} // namespace ssp