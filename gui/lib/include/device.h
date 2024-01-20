#pragma once
#include <vulkan/vulkan.h>
#include <map>
#include <cassert>
#include <vector>
#include <set>
#include "app.h"

namespace Vulkan{
class App;
struct SwapChainSupportDetail{
    VkSurfaceCapabilitiesKHR capab_;
    std::vector<VkSurfaceFormatKHR> formats_;
    std::vector<VkPresentModeKHR> present_mode_;
};
struct QueueFamilyIndex{
        std::pair<bool,uint32_t> graphics_family_=std::make_pair(false,0);
        std::pair<bool,uint32_t> present_family_=std::make_pair(false,0);
        bool IsGood(){return graphics_family_.first&&present_family_.first;}
};
class Device{
public:
    App* app_;
    VkInstance instance_;
    VkSurfaceKHR surface_;
    VkPhysicalDevice physic_device_;
    VkDevice device_;
    VkQueue graphics_queue_;
    VkQueue present_queue_;
    VkCommandPool command_pool_;
    bool enable_validation_lyers_;
    SwapChainSupportDetail detail_;
    QueueFamilyIndex family_;
    const std::vector<const char*> device_extensions_={
            VK_KHR_SWAPCHAIN_EXTENSION_NAME
        };
            std::vector<VkSemaphore> image_available_;
        std::vector<VkSemaphore> render_finish_;
        std::vector<VkFence> in_flight_;
        const int MAX_FRAMES_IN_FLIGHT;

public:
    bool IsDeviceSuitable(VkPhysicalDevice device);
    bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
    
public:
    Device(App* app=nullptr);
    ~Device();
    void CreateGeneralBuffer(VkDeviceSize size_,VkBufferUsageFlags usage, VkMemoryPropertyFlags props,VkBuffer& buf,VkDeviceMemory& buf_mem);
    template<typename T>
    void CreateBuffer(VkBuffer& buf,VkDeviceMemory& buf_mem,VkBufferUsageFlagBits flag,const std::vector<T>& datas);
    void CopyBuffer(VkBuffer src,VkBuffer dest,VkDeviceSize sz);
    uint32_t Device::FindMemoryType(uint32_t type_filter,VkMemoryPropertyFlags properties);
    SwapChainSupportDetail Device::QuerySwapChain(VkPhysicalDevice device);
    QueueFamilyIndex Device::FindQueueFamilies(VkPhysicalDevice devide);
public:
    void KillBuffer(VkBuffer& buf,VkDeviceMemory& buf_mem);
};
}