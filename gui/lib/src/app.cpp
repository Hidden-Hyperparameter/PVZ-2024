#include "app.h"
#include <cassert>
#include <set>

namespace Vulkan{
GLFWwindow* App::GetGLFWindow()const {return window_.window_;}
void App::run(){
    while(!window_.ShouldClose()){
        glfwPollEvents();
        Update();
        DrawFrame();
    }
}
App::App():window_(1000,1000,"Vulkan"){
    #ifdef VULKAN_DEBUG
    printf("Initing Vulkan in debug mode...\n");
    #else
    printf("Initing Vulkan in release mode...\n");
    #endif
    const int TOT_IMAGES=2;
    all_vertex.resize(TOT_IMAGES);
    commander_=new Command(this);
    for(int i=0;i<TOT_IMAGES;++i){
        images_.push_back(new Image(commander_,i,window_.width,window_.height,50,50));
    }
    images_[0]->Load("assets/texture1.png");
    images_[1]->Load("assets/texture3.png");
    commander_->MakeDescriptotAndPipeline();
    for(auto im:images_){
        im->pipe_lay_=&commander_->graphics_pipeline_->pipeline_layout_;
    }
    time_cnt=TIMER;
    //uniform buffer
}
void App::DrawFrame(){    
    auto command_buff=commander_->StartCommandBuffer(curr_frame_);
    for(auto im:images_){
        im->Draw(command_buff,curr_frame_);
    }
    commander_->SubmitCommandBuffer(curr_frame_);
    curr_frame_++;
    if(curr_frame_==MAX_FRAMES_IN_FLIGHT)curr_frame_=0;
}
void App::Update(){
    auto curr=TIMER;
    auto dur=DUR(time_cnt,curr);
    images_[0]->Update(dur/100,0);
    images_[1]->Update(0,dur/100);
}
bool App::CheckValidationLayer(){
    uint32_t layer_cnt;
    vkEnumerateInstanceLayerProperties(&layer_cnt,nullptr);
    std::vector<VkLayerProperties> available_lyers(layer_cnt);
    vkEnumerateInstanceLayerProperties(&layer_cnt,available_lyers.data());
    for(const char* lyer_nm:validation_layers_){
        bool lyer_found=false;
        for(auto lyer_ppt:available_lyers){
            if(strcmp(lyer_nm,lyer_ppt.layerName)==0){
                lyer_found=true;
                break;
            }
        }
        if(!lyer_found)return false;
    }
    return true;
}
}