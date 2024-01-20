#pragma once
#include "device.h"
#include "swap_chain.h"
#include "descriptor.h"
#include <cassert>
#include <vulkan/vulkan.h>
namespace Vulkan
{
class Device;
class SwapChain;
class Descriptor;
class PipeLine{
public:
    VkPipelineLayout pipeline_layout_;
    VkPipeline graphics_pipeline_;
    Device* device_;
    SwapChain* swap_chain_;
    // Descriptor* descriptot_;
public:
    PipeLine(SwapChain* swap_chain,const VkDescriptorSetLayout& descriptor_set_layout);
    ~PipeLine();
    VkShaderModule CreateShaderModule(const std::vector<char>& code);
};
} // namespace Vulkan
