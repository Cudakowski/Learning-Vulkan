#include "first_app.hpp"

// libs
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

// std
#include <array>
#include <stdexcept>

namespace ssp {

struct SimplePushConstantData {
  glm::vec2 offset;
  alignas(16) glm::vec3 color;
};

FirstApp::FirstApp() {
  loadModels();
  createPipelineLayout();
  recreateSwapChain();
  createCommandBuffers();
}

FirstApp::~FirstApp() {
  vkDestroyPipelineLayout(sspDevice.device(), pipelineLayout, nullptr);
}

void FirstApp::run() {
  while (!sspWindow.shouldClose()) {
    glfwPollEvents();
    drawFrame();
  }

  vkDeviceWaitIdle(sspDevice.device());
}

void FirstApp::loadModels() {
  std::vector<SspModel::Vertex> vertices{
      {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
      {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
      {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
  };

  sspModel = std::make_unique<SspModel>(sspDevice, vertices);
}

void FirstApp::createPipelineLayout() {
  VkPushConstantRange pushConstantRange{};
  pushConstantRange.stageFlags =
      VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
  pushConstantRange.offset = 0;
  pushConstantRange.size = sizeof(SimplePushConstantData);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;
  pipelineLayoutInfo.pSetLayouts = nullptr;
  pipelineLayoutInfo.pushConstantRangeCount = 1;
  pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

  if (vkCreatePipelineLayout(
          sspDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout
      ) != VK_SUCCESS) {
    throw std::runtime_error("failed to create pipeline layout!");
  }
}

void FirstApp::createPipeline() {
  assert(sspSwapChain != nullptr && "Cannot create pipeline before swap chain");
  assert(
      pipelineLayout != nullptr &&
      "Cannot create pipeline before pipeline layout"
  );

  PipelineConfigInfo pipelineConfig{};
  SspPipeline::defaultPipelineConfigInfo(pipelineConfig);
  pipelineConfig.renderPass = sspSwapChain->getRenderPass();
  pipelineConfig.pipelineLayout = pipelineLayout;
  sspPipeline = std::make_unique<SspPipeline>(
      sspDevice,
      "shaders/simple_shader.vert.spv",
      "shaders/simple_shader.frag.spv",
      pipelineConfig
  );
}

void FirstApp::recreateSwapChain() {
  auto extent = sspWindow.getExtent();
  while (extent.width == 0 || extent.height == 0) {
    extent = sspWindow.getExtent();
    glfwWaitEvents();
  }
  vkDeviceWaitIdle(sspDevice.device());

  if (sspSwapChain == nullptr) {
    sspSwapChain = std::make_unique<SspSwapChain>(sspDevice, extent);
  } else {
    sspSwapChain = std::make_unique<SspSwapChain>(
        sspDevice, extent, std::move(sspSwapChain)
    );
    if (sspSwapChain->imageCount() != commandBuffers.size()) {
      freeCommandBuffers();
      createCommandBuffers();
    }
  }

  // if render pass compatible do nothing else
  createPipeline();
}

void FirstApp::createCommandBuffers() {
  commandBuffers.resize(sspSwapChain->imageCount());

  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = sspDevice.getCommandPool();
  allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

  if (vkAllocateCommandBuffers(
          sspDevice.device(), &allocInfo, commandBuffers.data()
      ) != VK_SUCCESS) {
    throw std::runtime_error("failed to allocate command buffers!");
  }
}

void FirstApp::freeCommandBuffers() {
  vkFreeCommandBuffers(
      sspDevice.device(),
      sspDevice.getCommandPool(),
      static_cast<uint32_t>(commandBuffers.size()),
      commandBuffers.data()
  );
  commandBuffers.clear();
}

void FirstApp::recordCommandBuffer(int imageIndex) {
  static int frame =0;
  frame=(frame +1)%1000;

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

  if (vkBeginCommandBuffer(commandBuffers[imageIndex], &beginInfo) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to begin recording command buffer!");
  }

  VkRenderPassBeginInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  renderPassInfo.renderPass = sspSwapChain->getRenderPass();
  renderPassInfo.framebuffer = sspSwapChain->getFrameBuffer(imageIndex);

  renderPassInfo.renderArea.offset = {0, 0};
  renderPassInfo.renderArea.extent = sspSwapChain->getSwapChainExtent();

  std::array<VkClearValue, 2> clearValues{};
  clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
  clearValues[1].depthStencil = {1.0f, 0};
  renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
  renderPassInfo.pClearValues = clearValues.data();

  vkCmdBeginRenderPass(
      commandBuffers[imageIndex], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE
  );

  VkViewport viewport{};
  viewport.x = 0;
  viewport.y = 0;
  viewport.width = static_cast<float>(sspSwapChain->getSwapChainExtent().width);
  viewport.height =
      static_cast<float>(sspSwapChain->getSwapChainExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{{0, 0}, sspSwapChain->getSwapChainExtent()};
  vkCmdSetViewport(commandBuffers[imageIndex], 0, 1, &viewport);
  vkCmdSetScissor(commandBuffers[imageIndex], 0, 1, &scissor);

  sspPipeline->bind(commandBuffers[imageIndex]);
  // vkCmdDraw(commandBuffers[i],3,1,0,0);
  sspModel->bind(commandBuffers[imageIndex]);

  for (int j = 0; j < 4; j++) {
    SimplePushConstantData push{};
    push.offset = {-0.5f + frame*0.002f, -0.4f + j * 0.25f};
    push.color = {0.0f, 0.0f, 0.2f + 0.2f * j};

    vkCmdPushConstants(
        commandBuffers[imageIndex],
        pipelineLayout,
        VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
        0,
        sizeof(SimplePushConstantData),
        &push
    );
    sspModel->draw(commandBuffers[imageIndex]);
  }


  vkCmdEndRenderPass(commandBuffers[imageIndex]);
  if (vkEndCommandBuffer(commandBuffers[imageIndex]) != VK_SUCCESS) {
    throw std::runtime_error("failed to record command buffer!");
  }
}

void FirstApp::drawFrame() {
  uint32_t imageIndex;
  auto result = sspSwapChain->acquireNextImage(&imageIndex);

  if (result == VK_ERROR_OUT_OF_DATE_KHR) {
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
    throw std::runtime_error("failed to acquire swap chain image!");
  }

  recordCommandBuffer(imageIndex);
  result = sspSwapChain->submitCommandBuffers(
      &commandBuffers[imageIndex], &imageIndex
  );

  if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      sspWindow.wasWindowResized()) {
    sspWindow.resetWindowResizedFlag();
    recreateSwapChain();
    return;
  }

  if (result != VK_SUCCESS) {
    throw std::runtime_error("failed to present swap chain image!");
  }
}

} // namespace ssp