#pragma once

#include "ssp_device.hpp"

// std
#include <memory>
#include <unordered_map>
#include <vector>

namespace ssp {

class SspDescriptorSetLayout {
 public:
  class Builder {
   public:
    Builder(SspDevice &sspDevice) : sspDevice{sspDevice} {}

    Builder &addBinding(
        uint32_t binding,
        VkDescriptorType descriptorType,
        VkShaderStageFlags stageFlags,
        uint32_t count = 1);
    std::unique_ptr<SspDescriptorSetLayout> build() const;

   private:
    SspDevice &sspDevice;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
  };

  SspDescriptorSetLayout(
      SspDevice &sspDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
  ~SspDescriptorSetLayout();
  SspDescriptorSetLayout(const SspDescriptorSetLayout &) = delete;
  SspDescriptorSetLayout &operator=(const SspDescriptorSetLayout &) = delete;

  VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

 private:
  SspDevice &sspDevice;
  VkDescriptorSetLayout descriptorSetLayout;
  std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

  friend class SspDescriptorWriter;
};

class SspDescriptorPool {
 public:
  class Builder {
   public:
    Builder(SspDevice &sspDevice) : sspDevice{sspDevice} {}

    Builder &addPoolSize(VkDescriptorType descriptorType, uint32_t count);
    Builder &setPoolFlags(VkDescriptorPoolCreateFlags flags);
    Builder &setMaxSets(uint32_t count);
    std::unique_ptr<SspDescriptorPool> build() const;

   private:
    SspDevice &sspDevice;
    std::vector<VkDescriptorPoolSize> poolSizes{};
    uint32_t maxSets = 1000;
    VkDescriptorPoolCreateFlags poolFlags = 0;
  };

  SspDescriptorPool(
      SspDevice &sspDevice,
      uint32_t maxSets,
      VkDescriptorPoolCreateFlags poolFlags,
      const std::vector<VkDescriptorPoolSize> &poolSizes);
  ~SspDescriptorPool();
  SspDescriptorPool(const SspDescriptorPool &) = delete;
  SspDescriptorPool &operator=(const SspDescriptorPool &) = delete;

  bool allocateDescriptor(
      const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet &descriptor) const;

  void freeDescriptors(std::vector<VkDescriptorSet> &descriptors) const;

  void resetPool();

 private:
  SspDevice &sspDevice;
  VkDescriptorPool descriptorPool;

  friend class SspDescriptorWriter;
};

class SspDescriptorWriter {
 public:
  SspDescriptorWriter(SspDescriptorSetLayout &setLayout, SspDescriptorPool &pool);

  SspDescriptorWriter &writeBuffer(uint32_t binding, VkDescriptorBufferInfo *bufferInfo);
  SspDescriptorWriter &writeImage(uint32_t binding, VkDescriptorImageInfo *imageInfo);

  bool build(VkDescriptorSet &set);
  void overwrite(VkDescriptorSet &set);

 private:
  SspDescriptorSetLayout &setLayout;
  SspDescriptorPool &pool;
  std::vector<VkWriteDescriptorSet> writes;
};

}  // namespace ssp