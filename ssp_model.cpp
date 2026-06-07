#include "ssp_model.hpp"


#include <cassert>
#include <cstring>
namespace ssp
{

SspModel::SspModel(SspDevice &device, const SspModel::Builder &builder) : sspDevice{device}
{
    createVertexBuffers(builder.vertices);
    createIndexBuffers(builder.indices);
}


SspModel::~SspModel()
{
    vkDestroyBuffer(sspDevice.device(), vertexBuffer, nullptr);
    vkFreeMemory(sspDevice.device(),vertexBufferMemory, nullptr);

    if (hasIndexBuffer){
        vkDestroyBuffer(sspDevice.device(), indexBuffer, nullptr);
        vkFreeMemory(sspDevice.device(),indexBufferMemory, nullptr);
    }
}


void SspModel::createVertexBuffers(const std::vector<Vertex> &vertices)
{
    vertexCount = static_cast<uint32_t>(vertices.size());
    assert(vertexCount >=3 && "Vertex count must be at least 3");
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    sspDevice.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void *data;
    vkMapMemory(sspDevice.device(), stagingBufferMemory, 0 , bufferSize, 0 , &data);
    memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(sspDevice.device(), stagingBufferMemory);

    sspDevice.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        vertexBuffer,
        vertexBufferMemory
    );

    sspDevice.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(sspDevice.device(), stagingBuffer, nullptr);
    vkFreeMemory(sspDevice.device(),stagingBufferMemory, nullptr);

}

void SspModel::createIndexBuffers(const std::vector<uint32_t> &indeces)
{
    indexCount = static_cast<uint32_t>(indeces.size());
    hasIndexBuffer = indexCount > 0;

    if(!hasIndexBuffer){
        return;
    }

    VkDeviceSize bufferSize = sizeof(indeces[0]) * indexCount;

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    sspDevice.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        stagingBuffer,
        stagingBufferMemory
    );

    void *data;
    vkMapMemory(sspDevice.device(), stagingBufferMemory, 0 , bufferSize, 0 , &data);
    memcpy(data, indeces.data(), static_cast<size_t>(bufferSize));
    vkUnmapMemory(sspDevice.device(), stagingBufferMemory);

    sspDevice.createBuffer(
        bufferSize,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
        indexBuffer,
        indexBufferMemory
    );

    sspDevice.copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(sspDevice.device(), stagingBuffer, nullptr);
    vkFreeMemory(sspDevice.device(),stagingBufferMemory, nullptr);
}


void SspModel::draw(VkCommandBuffer commandBuffer)
{
    if ( hasIndexBuffer) {
        vkCmdDrawIndexed(commandBuffer, indexCount,1 ,0,0,0);
    } else {
        vkCmdDraw(commandBuffer, vertexCount, 1,0,0);
    }
}

void SspModel::bind(VkCommandBuffer commandBuffer)
{
    VkBuffer buffers[]={vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer,0,1,buffers,offsets);

    if ( hasIndexBuffer) {
        vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
}


std::vector<VkVertexInputBindingDescription> SspModel::Vertex::getBindingDescriptions()
{
    std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
    bindingDescriptions[0].binding = 0;
    bindingDescriptions[0].stride = sizeof(Vertex);
    bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    return bindingDescriptions;

}

std::vector<VkVertexInputAttributeDescription> SspModel::Vertex::getAttributeDescriptions()
{
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);
    attributeDescriptions[0].location = 0;
    attributeDescriptions[0].binding = 0;
    attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[0].offset =  offsetof(Vertex, position);

    attributeDescriptions[1].location = 1;
    attributeDescriptions[1].binding = 0;
    attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
    attributeDescriptions[1].offset = offsetof(Vertex, color);
    
    return attributeDescriptions;

}


} // namespace ssp
