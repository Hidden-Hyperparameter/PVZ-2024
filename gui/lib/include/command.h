#pragma once
#include "device.h"
#include "swap_chain.h"
#include "pipeline.h"
#include "descriptor.h"
#include "app.h"
#include <vector>
#include <vulkan/vulkan.h>
namespace Vulkan
{
class Device;
class SwapChain;
class PipeLine;
class Descriptor;
class App;
struct Vertex;
class Command{
public:
    Device* device_;
    SwapChain* swap_chain_;
    PipeLine* graphics_pipeline_;
    uint32_t image_index_=0;
    const int MAX_FRAMES_IN_FLIGHT;
    // VkDeviceSize offset_=0;
    // VkBuffer index_buff_;
    App* app_;
    // VkDeviceMemory index_buff_memory_;
    std::vector<VkCommandBuffer> command_buff_;

public:
  //std::vector<VkBuffer> uniform_buff_;
    //std::vector<VkDeviceMemory> uniform_buff_mem_;
    //std::vector<void*> uniform_buff_map_;
    VkSampler sampler_;
    Command(App* app);
    void MakeDescriptotAndPipeline();
    VkCommandBuffer&& StartCommandBuffer(uint32_t frame);
    /**
     * @brief the command begun can be only used once. It is used in copy buffer or image senarios.
     * 
     */
    VkCommandBuffer&& BeginOneTimeCommand();
    /**
     * @brief used with BeginOneTimeCommand();
     * 
     * @param command_buff the command buffer that begin operation returns
     */
    void EndOneTimeCommand(VkCommandBuffer&& command_buff);
    void SubmitCommandBuffer(uint32_t frema);
    // void Command::ReloadVertex(const std::vector<Vertex>& vert,int image);
    // VkDescriptorBufferInfo GetBufferDescriptotInfo(int i);
    /**
    * @bug this have to be changed
    * @brief images_[0] is not a good idea
    */
    // VkDescriptorImageInfo GetImageDescriptotInfo(int i);
    ~Command();
};
} // namespace Vulkan
