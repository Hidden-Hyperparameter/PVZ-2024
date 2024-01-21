#include "object.h"
namespace Vulkan
{
void Object::Update(int move_x,int move_y){
    x_pos_=move_x,y_pos_=move_y;
}
Object::Object(App* app,Image* im):app_(app),WINDOW_Width(app->window_->width),WINDOW_Height(app->window_->height),x_size_(im->x_size_),y_size_(im->y_size_),descriptor_(im->descriptot_),pipe_lay_(im->pipe_lay_){
    vertexs_=init_vertexs;
    float x_size_normalized_=(float)x_size_/WINDOW_Width;
    float y_size_normalized_=(float)y_size_/WINDOW_Height;
    // vertexs_[0].pos={2*x_normalized_-1,2*y_normalized_-1};
    // vertexs_[1].pos={2*x_normalized_+2*x_size_normalized_-1,2*y_normalized_-1};
    // vertexs_[2].pos={2*x_normalized_+2*x_size_normalized_-1,2*y_normalized_+2*y_size_normalized_-1};
    // vertexs_[3].pos={2*x_normalized_-1,2*y_normalized_+2*y_size_normalized_-1};
    vertexs_[0].pos={-1,-1};
    vertexs_[3].pos={-1,2*y_size_normalized_-1};
    vertexs_[2].pos={2*x_size_normalized_-1,2*y_size_normalized_-1};
    vertexs_[1].pos={2*x_size_normalized_-1,-1};
    Device* device=im->device_;
    device->CreateBuffer(vertex_buff_,vertex_buff_memory_,VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,vertexs_);
    device->CreateBuffer(index_buff_,index_buff_memory_,VK_BUFFER_USAGE_INDEX_BUFFER_BIT,index);
}
void Object::Draw(const VkCommandBuffer& command_buff,int curr_frame_){
    vkCmdBindDescriptorSets(
        command_buff,VK_PIPELINE_BIND_POINT_GRAPHICS,
        *pipe_lay_,
        0,1,
        &descriptor_->descriptor_set_[app_->curr_frame_],
        0,nullptr
    );
    VkDeviceSize offser=0;
    vkCmdBindVertexBuffers(command_buff,0,1,&vertex_buff_,& offser);
    vkCmdBindIndexBuffer(command_buff,index_buff_,offser,VK_INDEX_TYPE_UINT16);
    PushConst push;
    push.offset=glm::vec2{(float)(2.0f*x_pos_)/WINDOW_Width,(float)(2.0f*y_pos_)/WINDOW_Height};
    // printf("push constant has offeset %lf and %lf\n",push.offset[0],push.offset[1]);
    /**
     * If the line below gives your an error such that @param pipe_lay_ is nulllptr, then check whether you have initialized @param pipe_lay_ after the creation of image. Due to the problems(at the beginning, @param pipe_lay_ isn't initialized), this initialization of @param pipe_lay_ can't be added in the constructor.
     * 
     */
    vkCmdPushConstants(command_buff,*pipe_lay_,VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT,0,sizeof(PushConst),&push);
    vkCmdDrawIndexed(command_buff,index.size(),1,0,0,0);
}
} // namespace Vulkan
