#include <iostream>
#include <cstdlib>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include "lib/include/app.h"
#include <chrono>
int main(){
    Vulkan::App app(1000,1000,"Test");
    auto time=std::chrono::steady_clock::now();
    while(app.IsRunning()){
        glfwPollEvents();
        //update
        auto pass=app.GetTime();
        app.Add((pass%1000)*glm::cos((float)pass/1000),(pass%1000)*glm::sin((float)pass/1000),0);
        app.DrawFrame();
    }
    // printf("Hellp,gui!\n");
    return 0;
    return 0;
}