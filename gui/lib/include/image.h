#pragma once
#include "command.h"
#include "device.h"
#include "descriptor.h"
#include "file_helper.h"
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <string>
#include <cassert>
#include <glm/glm.hpp>

namespace Vulkan
{
struct Vertex{
    // glm::vec2 pos;
    // glm::vec3 color;
    glm::vec2 pos;
    glm::vec3 color;
    glm::vec2 texCoord;
    typedef VkVertexInputBindingDescription Binding;
    static Binding GetBinding(){//load the vertex into GLSL code
        Binding bind_descript{};
        bind_descript.binding=0;
        bind_descript.stride=sizeof(Vertex);
        bind_descript.inputRate=VK_VERTEX_INPUT_RATE_VERTEX;
        return bind_descript;
    }
    typedef VkVertexInputAttributeDescription Attribute;
    static std::array<Attribute,3> GetAttribute(){
        std::array<Attribute,3> attributeDescriptions{};

        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, pos);

        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        attributeDescriptions[2].binding = 0;
        attributeDescriptions[2].location = 2;
        attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
        attributeDescriptions[2].offset = offsetof(Vertex, texCoord);
        return attributeDescriptions;
    }
};
class Command;
class Device;
class Descriptor;
class Image{
public:
    Device* device_;
    Command* commander_;
    VkPipelineLayout* pipe_lay_=nullptr;
    Descriptor* descriptot_;
    std::vector<Vertex> vertexs_;
    VkBuffer vertex_buff_;
    VkBuffer index_buff_;
    VkDeviceMemory vertex_buff_memory_;
    VkDeviceMemory index_buff_memory_;
    VkBuffer staging_buff_;
    VkDeviceMemory staging_buff_mem_;
    VkImage image_;
    VkDeviceMemory image_mem_;
    VkImageView image_view_;
  
    Image(Command* commander,int window_x,int window_y,int x_size_,int y_size_,int x_pos=0,int y_pos=0);
    const int WINDOW_Width;
    const int WINDOW_Height;
    int x_pos_=0,y_pos_=0;
    int x_size_=0,y_size_=0;

    const std::vector<Vertex> init_vertexs={
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
};
    const std::vector<uint16_t> index={
        0,1,2,2,3,0
    };
    /**
     * @brief bind vertex buffer and then draw
     * 
     */
    void Draw(const VkCommandBuffer& command_buff,int curr_frame_);
    /**
     * @brief let the image move
     * 
     * @param move_x move in pixels
     * @param move_y move in pixels
     */
    void Update(int move_x,int move_y);
    VkDescriptorImageInfo GetImageDescriptotInfo(int i);
    bool Load(const std::string& image_name_);
    void Transition(VkImage image, VkFormat format, VkImageLayout old,VkImageLayout new_);
    ~Image();
};
} // namespace Vulkan

/**
 * TODO: add the image view after it .
 * reference https://vulkan-tutorial.com/en/Texture_mapping/Image_view_and_sampler
 * 
 */