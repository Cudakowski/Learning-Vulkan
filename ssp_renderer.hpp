#pragma once

#include "ssp_device.hpp"
#include "ssp_swap_chain.hpp"
#include "ssp_window.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace ssp {
class SspRenderer {
public:
  SspRenderer(SspWindow &window, SspDevice &device);
  ~SspRenderer();

  SspRenderer(const SspRenderer &) = delete;
  SspRenderer &operator=(const SspRenderer &) = delete;

  VkRenderPass getSwapChainRenderPass() const {return sspSwapChain->getRenderPass();}
  bool isFrameInProgress() const { return isFrameStarted; }

  VkCommandBuffer getCurrentCommandBuffer() const {
    assert(isFrameStarted && "Cannot get buffer when frame not in progress");
    return commandBuffers[currentFrameIndex];
  }

  int getFrameIndex() const {
    assert(isFrameStarted && "Cannot get frame index when frame not in progress");
    return currentFrameIndex;
  }

  VkCommandBuffer beginFrame();
  void endFrame();
  void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
  void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

private:
  void createCommandBuffers();
  void freeCommandBuffers();
  void recreateSwapChain();

  SspWindow &sspWindow;
  SspDevice &sspDevice;
  std::unique_ptr<SspSwapChain> sspSwapChain;
  std::vector<VkCommandBuffer> commandBuffers;

  uint32_t currentImageIndex;
  int currentFrameIndex{0};
  bool isFrameStarted{false};
};

} // namespace ssp