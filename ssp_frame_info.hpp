#pragma once

#include "ssp_camera.hpp"

//lib
#include <vulkan/vulkan.h>

namespace ssp
{

struct FrameInfo {
    int frmaeIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    SspCamera &camera;
};

} // namespace ssp
