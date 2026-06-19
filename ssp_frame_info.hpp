#pragma once

#include "ssp_camera.hpp"
#include "ssp_game_object.hpp"

//lib
#include <vulkan/vulkan.h>


namespace ssp
{

struct FrameInfo {
    int frmaeIndex;
    float frameTime;
    VkCommandBuffer commandBuffer;
    SspCamera &camera;
    VkDescriptorSet globalDescriptorSet;
    SspGameObject::Map &gameObjects;
};

} // namespace ssp
