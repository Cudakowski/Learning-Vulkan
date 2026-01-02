#pragma once

#include "ssp_window.hpp"
#include "ssp_pipeline.hpp"
#include "ssp_device.hpp"
#include "ssp_swap_chain.hpp"
#include "ssp_model.hpp"

#include <memory>
#include <vector>

namespace ssp{
class FirstApp
{
public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT =600;
    

    FirstApp();
    ~FirstApp();

    FirstApp(const FirstApp &)=delete;
    FirstApp &operator=(const FirstApp &)=delete;

    void run();
private:
    void loadModels();
    void createPipelineLayout();
    void createPipeline();
    void createCommandBuffers();
    void drawFrame();

    SspWindow sspWindow{WIDTH,HEIGHT, "Hello!"};
    SspDevice sspDevice{sspWindow};
    SspSwapChain sspSwapChain{sspDevice, sspWindow.getExtent()};

    std::unique_ptr<SspPipeline> sspPipeline;
    VkPipelineLayout pipelineLayout;
    std::vector<VkCommandBuffer> commandBuffers;
    std::unique_ptr<SspModel> sspModel;

};



} // namespace ssp