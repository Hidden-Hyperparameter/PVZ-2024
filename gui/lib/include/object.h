#pragma once
#include "image.h"
#include "device.h"
#include "descriptor.h"
#include "app.h"
#include <vulkan/vulkan.h>
#include <array>
#include <glm/glm.hpp>
namespace Vulkan
{
struct PushConst{
    glm::vec2 offset;
    alignas(16)glm::vec3 color;
};
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
class Device;
class Descriptor;
class Image;
class App;
class Object{
public:
    App* app_;
    VkBuffer vertex_buff_;
    VkBuffer index_buff_;
    VkDeviceMemory vertex_buff_memory_;
    VkDeviceMemory index_buff_memory_;
    Descriptor* descriptor_;
    VkPipelineLayout* pipe_lay_;
    Object(App* app,Image* im);
    // Object(App* app,App::image_t id);
    std::vector<Vertex> vertexs_;
    const std::vector<Vertex> init_vertexs={
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f}}
    };
    const std::vector<uint16_t> index={
        0,1,2,2,3,0
    };
    void Update(int move_x,int move_y);
    const int WINDOW_Width;
    const int WINDOW_Height;
    int x_pos_=0,y_pos_=0;
    int x_size_=0,y_size_=0;
    void Draw(const VkCommandBuffer& command_buff,int curr_frame_);

};
} // namespace Vulkan
