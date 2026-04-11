#pragma once

#include "ssp_device.hpp"

#include <string>
#include <vector>

namespace ssp
{

struct PipelineConfigInfo {
  PipelineConfigInfo(const PipelineConfigInfo&)=delete;
  PipelineConfigInfo& operator=(const PipelineConfigInfo&)=delete;

  VkPipelineViewportStateCreateInfo viewportInfo;
  VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
  VkPipelineRasterizationStateCreateInfo rasterizationInfo;
  VkPipelineMultisampleStateCreateInfo multisampleInfo;
  VkPipelineColorBlendAttachmentState colorBlendAttachment;
  VkPipelineColorBlendStateCreateInfo colorBlendInfo;
  VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
  std::vector<VkDynamicState> dynamicStateEnables;
  VkPipelineDynamicStateCreateInfo dynamicStateInfo;
  VkPipelineLayout pipelineLayout = nullptr;
  VkRenderPass renderPass = nullptr;
  uint32_t subpass = 0;
};



class SspPipeline
{
private:
    static std::vector<char> readFile(const std::string& filepath);

    void createGraphicsPipeline(
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo &configInfo);

    void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

    SspDevice& sspDevice;
    VkPipeline graphicsPipeline;
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
public:
    SspPipeline(
        SspDevice &device,
        const std::string& vertFilepath,
        const std::string& fragFilepath, 
        const PipelineConfigInfo &configInfo);

    ~SspPipeline();

    SspPipeline(const SspPipeline&) = delete;
    SspPipeline& operator=(const SspPipeline&) = delete;

    void bind(VkCommandBuffer commandBuffer);
    static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);
};





} // namespace ssp
