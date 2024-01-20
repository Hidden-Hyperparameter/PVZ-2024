#include "app.h"
#include <cassert>
#include <set>

namespace Vulkan{
GLFWwindow* App::GetGLFWindow()const {return window_->window_;}
void App::run(){
    while(!window_->ShouldClose()){
        glfwPollEvents();
        // Update();
        DrawFrame();
    }
}
void App::Add(int xp,int yp,int im){
    assert(images_.count(im) && "This image isn't initialzed yet");
    images_[im]->Update(xp,yp);
    have_to_draw.insert(im);
}
int App::LoadOneImage(const std::string& name){
    auto extent=GetImageSize(name);
    Image* construct=(new Image(commander_,window_->width,window_->height,extent.first,extent.second));
    construct->pipe_lay_=& commander_->graphics_pipeline_->pipeline_layout_;
    bool res=construct->Load(name);
    if(res){
        images_[image_cnt_]=(construct);
        return image_cnt_++;
    }else{
        return LOAD_IMAGE_FAIL;
    }
}
int App::LoadOneImageNoFailure(const std::string& name){
    auto re=LoadOneImage(name);
    assert(re!=LOAD_IMAGE_FAIL && "Failed to load image. If there are possible failures, please use LoadOneImage instead.");
    return re;
}
void App::PollEvent(){glfwPollEvents();}

bool App::IsRunning()const{return !window_->ShouldClose();}
App::App(int w,int h,const std::string& name):window_(new lve::Window(w,h,name)){
    #ifdef VULKAN_DEBUG
    printf("Initing Vulkan in debug mode...\n");
    #else
    printf("Initing Vulkan in release mode...\n");
    #endif
    commander_=new Command(this);
    Image* construct=(new Image(commander_,window_->width,window_->height,50,50));
    bool res=construct->Load("empty.png");
    assert(res!=LOAD_IMAGE_FAIL && "failed to load image. Please see file_helper.cpp and change the code.");
    images_[image_cnt_++]=(construct);
    commander_->MakeDescriptotAndPipeline();
    for(auto im:images_){
        im.second->pipe_lay_=&commander_->graphics_pipeline_->pipeline_layout_;
    }
    start=TIMER;
    //uniform buffer
}
const std::pair<uint32_t,uint32_t> App::GetImageSize(int id){
    assert(id<images_.size()&& "image index out of bound\n");
    return std::make_pair(images_[id]->x_size_,images_[id]->y_size_);
}
void App::Remove(int im){have_to_draw.erase(im);}
App::App(){
    
}
void App::DrawFrame(){    
    auto command_buff=commander_->StartCommandBuffer(curr_frame_);
    for(auto im:have_to_draw){
        images_[im]->Draw(command_buff,curr_frame_);
    }
    commander_->SubmitCommandBuffer(curr_frame_);
    curr_frame_++;
    if(curr_frame_==MAX_FRAMES_IN_FLIGHT)curr_frame_=0;
    // have_to_draw.clear();
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
std::pair<int,int> App::GetImageSize(const std::string& name){
    auto ret=FileHelper::LoadImage(name);
    // assert(std::get<2>(ret)!=nullptr && "Failed to load image");
    return std::make_pair(std::get<0>(ret),std::get<1>(ret));
}
long long App::GetTime()const {
    auto c=TIMER;
    return DUR(start,c);
}
}