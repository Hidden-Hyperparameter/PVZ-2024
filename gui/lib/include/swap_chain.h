#pragma once
#include <vulkan/vulkan.h>
#include "device.h"
#include <vector>
namespace Vulkan
{
class Device;
class SwapChain{
public:
    Device* device_;
    VkSwapchainKHR swap_chain_;
    std::vector<VkImage> swap_chain_images_;
    VkFormat swap_chain_image_format_;
    VkExtent2D swap_chain_extent_;
    std::vector<VkImageView> swap_chain_image_views_;
    std::vector<VkFramebuffer> swap_chain_frame_buffers_;
    /**
     * @brief render pass is a variable describing the input data. Such as, the data is organized as position and color.
     * 
     */
    VkRenderPass render_pass_;
public:
    SwapChain(Device* device);
    ~SwapChain();
    VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& avaiable);
    VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available);
    VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps);
    void CreateImageViews();
    void CreateFrameBuffers();
    void CreateRenderPass();
};
    
} // namespace Vulkan
