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
    auto id=app.LoadOneImageNoFailure("assets/empty.png");
    // Vulkan::Object obj1=Vulkan::Object(& app);
    Vulkan::App::object_t obj1=app.AddOneObject();
    Vulkan::App::object_t obj2=app.AddOneObject();
    int virtual_time=0;
    while(app.IsRunning()){
        app.PollEvent();
        //update
        auto pass=app.GetTime();
        if(pass<(virtual_time+1)*50)continue;
        virtual_time=pass/50;
        app.AddToDrawList(500 + (pass%1000)*glm::cos((float)pass/1000),500 + (pass%1000)*glm::sin((float)pass/1000),obj1,id);
        app.AddToDrawList(500 - (pass%1000)*glm::cos((float)pass/1000),500 - (pass%1000)*glm::sin((float)pass/1000),obj2,id);
        app.DrawFrame();
    }
    // printf("Hellp,gui!\n");
    return 0;
    return 0;
}