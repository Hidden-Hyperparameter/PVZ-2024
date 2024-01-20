#pragma once
#include "device.h"
#include "image.h"
#include <unordered_map>
#include <vulkan/vulkan.h>
#include <cassert>
#include <vector>
namespace Vulkan
{
struct UniformBuffer{

};
class Device;
// class Command;
class Image;
class Descriptor{
public:
    Device* device_;
    Image* image_;
    uint32_t max_set_;
    std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings_;
    std::vector<VkDescriptorPoolSize> pool_size_;
    VkDescriptorSetLayout set_layout_;
    std::vector<VkDescriptorSet> descriptor_set_;
    VkDescriptorPool descriptor_pool_;
    Descriptor(){;}
    const VkDescriptorSetLayout& CreateSetLayout(Image* image,const std::vector<std::tuple<uint32_t, VkDescriptorType, VkShaderStageFlags> >& binding_list);
    // Descriptor(Command* command,const std::vector<std::tuple<uint32_t, VkDescriptorType, VkShaderStageFlags> >& binding_list);
    void ContinueConstruct(
    const std::vector<std::pair<VkDescriptorType,uint32_t> >& pool_size);
    // Descriptor(Command* command,
    // uint32_t binding,
    // const std::vector<std::pair<VkDescriptorType,uint32_t> >& pool_size,
    // const std::vector<std::tuple<uint32_t , VkDescriptorType ,VkShaderStageFlags> >&binding_list);
    

    ~Descriptor();
};  

} // namespace Vulkan

