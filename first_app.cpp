#include "first_app.hpp"

#include <stdexcept>
#include <array>

namespace ssp{


FirstApp::FirstApp()
{
    loadModels();
    createPipelineLayout();
    createPipeline();
    createCommandBuffers();
}


FirstApp::~FirstApp()
{
    vkDestroyPipelineLayout(sspDevice.device(),pipelineLayout, nullptr);
}


void FirstApp::run()
{
    while(!sspWindow.shouldClose())
    {
        glfwPollEvents();
        drawFrame();
    }

    vkDeviceWaitIdle(sspDevice.device());
}

void FirstApp::loadModels()
{
    std::vector<SspModel::Vertex> vertices {
        {{ 0.0f, -0.5f},{1.0f,0.0f,0.0f}},
        {{ 0.5f,  0.5f},{0.0f,1.0f,0.0f}},
        {{-0.5f,  0.5f},{0.0f,0.0f,1.0f}}
    };

    sspModel = std::make_unique<SspModel>(sspDevice,vertices);
}


void FirstApp::createPipelineLayout()
{
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType =  VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 0;
    pipelineLayoutInfo.pSetLayouts = nullptr;
    pipelineLayoutInfo.pushConstantRangeCount = 0;
    pipelineLayoutInfo.pPushConstantRanges = nullptr;

    if(vkCreatePipelineLayout(sspDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create pipeline layout!");
    }
}


void FirstApp::createPipeline()
{
    auto pipelineConfig = 
        SspPipeline::defaultPipelineConfigInfo(sspSwapChain.width(), sspSwapChain.height());
    pipelineConfig.renderPass = sspSwapChain.getRenderPass();
    pipelineConfig.pipelineLayout = pipelineLayout;
    sspPipeline = std::make_unique<SspPipeline>(
        sspDevice,
        "shaders/simple_shader.vert.spv",
        "shaders/simple_shader.frag.spv",
        pipelineConfig
    );
}


void FirstApp::createCommandBuffers() 
{
    commandBuffers.resize(sspSwapChain.imageCount());

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = sspDevice.getCommandPool();
    allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

    if(vkAllocateCommandBuffers(sspDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    for(int i{};i<commandBuffers.size(); ++i)
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if(vkBeginCommandBuffer(commandBuffers[i],&beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;  
        renderPassInfo.renderPass = sspSwapChain.getRenderPass();
        renderPassInfo.framebuffer = sspSwapChain.getFrameBuffer(i);

        renderPassInfo.renderArea.offset = {0,0};
        renderPassInfo.renderArea.extent = sspSwapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        sspPipeline->bind(commandBuffers[i]);
        //vkCmdDraw(commandBuffers[i],3,1,0,0);
        sspModel->bind(commandBuffers[i]);
        sspModel->draw(commandBuffers[i]);

        vkCmdEndRenderPass(commandBuffers[i]);
        if(vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}


void FirstApp::drawFrame() {
    uint32_t imageIndex;
    auto result = sspSwapChain.acquireNextImage(&imageIndex);

    if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
    {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    result = sspSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);

    if(result != VK_SUCCESS)
    {
        throw std::runtime_error("failed to present swap chain image!");
    }
}


} // namespace ssp