#pragma once
//#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
namespace lve{
    class Window{
        public:
        Window(int w,int h,std::string name);
        GLFWwindow* window_;
        void initWindow();
        const int width,height;
        std::string windowName;
        ~Window();
        bool ShouldClose();
        Window(const Window& )=delete;
        Window& operator=(const Window& )=delete;
    };
}
