#pragma once
#define GLFW_INCLUDE_VULKAN 0
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstdlib>
#include <map>
#include <vector>
#include <iostream>
#include "window.h"
#include "file_helper.h"
#include "command.h"
#include "image.h"
#include <glm/glm.hpp>
#include <array>
#include <chrono>
#define TIMER std::chrono::steady_clock::now()
#define DUR(a,b) std::chrono::duration_cast<std::chrono::milliseconds>(b-a).count()
typedef decltype(TIMER) TIME;
namespace Vulkan{
class PipeLine;
class Command;
class Image;
struct Vertex;
struct PushConst{
    glm::vec2 offset;
    alignas(16)glm::vec3 color;
};
class App{
    
    public:
        GLFWwindow* GetGLFWindow()const;

        std::vector<Image*> images_;
        /**
         * @brief the total number of general-meaning "images" that is able to be shown up simontaneously on screen.
         * 
         */
        std::vector<std::vector<Vertex> >all_vertex;
        std::vector<uint16_t> indicies={
            0,1,2,2,3,0,4,5,6,6,7,4
        };
        lve::Window window_;
        Command* commander_;

        const int MAX_FRAMES_IN_FLIGHT=2;
        uint32_t curr_frame_=0;
       
        // void CreateLogicDevice();
        // bool IsDeviceSuitable(VkPhysicalDevice device);
        void DrawFrame();
        // void CreateBuffer(VkDeviceSize size_,VkBufferUsageFlags usage, VkMemoryPropertyFlags props,VkBuffer& buf,VkDeviceMemory& buf_mem);
        // void CopyBuffer(VkBuffer src,VkBuffer dest,VkDeviceSize sz);
        // QueueFamilyIndex FindQueueFamilies(VkPhysicalDevice devide);
        // SwapChainSupportDetail QuerySwapChain(VkPhysicalDevice device);
        VkImage tex_image;
        VkDeviceMemory tex_image_mem;
        
        //Validation Layers
        const std::vector<const char*> validation_layers_={
            "VK_LAYER_KHRONOS_validation"
        };

        const bool enable_validation_lyers=
        #ifdef VULKAN_DEBUG
        true
        #else
        false
        #endif
        ;
        bool CheckValidationLayer();
        TIME time_cnt;
    public:
        App();
        void run();
        void Update();
};

}
