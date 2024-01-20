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
#include <string>
#include <unordered_set>
#include <unordered_map>
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
        App(int w,int h,const std::string& name);
        GLFWwindow* GetGLFWindow()const;
        static const int LOAD_IMAGE_FAIL=-1;
        std::unordered_map<int,Image*> images_;
        // std::vector<Image*> images_;
        /**
         * @brief the total number of general-meaning "images" that is able to be shown up simontaneously on screen.
         * 
         */
        lve::Window* window_;
        Command* commander_;
        const std::pair<uint32_t,uint32_t> GetImageSize(int id);
        const int MAX_FRAMES_IN_FLIGHT=2;
        uint32_t curr_frame_=0;
       uint32_t image_cnt_=0;
        // void CreateLogicDevice();
        // bool IsDeviceSuitable(VkPhysicalDevice device);
        void DrawFrame();
        /**
         * @brief load an image into app storage
         * 
         * @param name image name, not contain "asset" path
         * @return int id(the id in app image storage)
         */
        int LoadOneImage(const std::string& name);
        int LoadOneImageNoFailure(const std::string& name);
        static std::pair<int,int>GetImageSize(const std::string& name);
        void PollEvent();
        bool IsRunning()const;
        // void CreateBuffer(VkDeviceSize size_,VkBufferUsageFlags usage, VkMemoryPropertyFlags props,VkBuffer& buf,VkDeviceMemory& buf_mem);
        // void CopyBuffer(VkBuffer src,VkBuffer dest,VkDeviceSize sz);
        // QueueFamilyIndex FindQueueFamilies(VkPhysicalDevice devide);
        // SwapChainSupportDetail QuerySwapChain(VkPhysicalDevice device);
        VkImage tex_image;
        VkDeviceMemory tex_image_mem;
        std::unordered_multiset<int> have_to_draw; 
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
        /**
         * @brief Get the Time object
         * 
         * @return long long (ms)
         */
        long long GetTime()const;
        TIME start;
    public:
        App();
        void run();
        void Add(int xp,int yp,int im);
        void Remove(int im);
        void Update();
};

}
