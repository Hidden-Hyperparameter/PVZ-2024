#pragma once
#include "command.h"
#include "device.h"
#include "descriptor.h"
#include "file_helper.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <string>
#include <cassert>
#include <glm/glm.hpp>
/**
 * 
 * 
 * TODO:
 * Separate the image class into two distinct classes. One "Object" class only manages the vertex and index buffer. The descriptots can be managed by the App class.
 * One Object is binded with one game object(Unit). For each frame, the object choose different image descriptots.
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 *
 */

namespace Vulkan
{

class Command;
class Device;
class Descriptor;
class Image{
public:
    Device* device_;
    Command* commander_;
    VkPipelineLayout* pipe_lay_=nullptr;
    Descriptor* descriptot_;
    
    VkBuffer staging_buff_;
    VkDeviceMemory staging_buff_mem_;
    VkImage image_;
    VkDeviceMemory image_mem_;
    VkImageView image_view_;
    /**
     * @brief is_master means that whether this object can descruct the image view etc.
     * 
     */
    bool is_master_;
    Image(Command* commander,int window_x,int window_y,int x_size_,int y_size_,int x_pos=0,int y_pos=0);
    Image(const Image &other);
    int x_size_,y_size_;


    /**
     * @brief bind vertex buffer and then draw
     * 
     */
    /**
     * @brief let the image move
     * 
     * @param move_x move in pixels
     * @param move_y move in pixels
     */
    VkDescriptorImageInfo GetImageDescriptotInfo(int i);
    bool Load(const std::string& image_name_);
    void Transition(VkImage image, VkFormat format, VkImageLayout old,VkImageLayout new_);
    ~Image();
};
} // namespace Vulkan
