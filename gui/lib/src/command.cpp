#include "command.h"
namespace Vulkan
{
Command::Command(App* app):MAX_FRAMES_IN_FLIGHT(app->MAX_FRAMES_IN_FLIGHT){
    app_=(app),device_=(new Device(app)),swap_chain_=(new SwapChain(device_));
    
    //uniform buffer
    //Create uniform buffer
    // uniform_buff_.resize(device_->MAX_FRAMES_IN_FLIGHT);
    // uniform_buff_mem_.resize(device_->MAX_FRAMES_IN_FLIGHT);
    // uniform_buff_map_.resize(device_->MAX_FRAMES_IN_FLIGHT);
    // for(int i=0;i<device_->MAX_FRAMES_IN_FLIGHT;i++){
    //     device_->CreateGeneralBuffer(sizeof(UniformBuffer),VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,uniform_buff_[i],uniform_buff_mem_[i]);
    //     vkMapMemory(device_->device_,uniform_buff_mem_[i],0,sizeof(UniformBuffer),0,&uniform_buff_map_[i]);
    // }
    VkSamplerCreateInfo samplerInfo{};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = VK_FILTER_LINEAR;
    samplerInfo.minFilter = VK_FILTER_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samplerInfo.anisotropyEnable = VK_TRUE;
    VkPhysicalDeviceProperties properties{};
    vkGetPhysicalDeviceProperties(device_->physic_device_, &properties);
    samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
    samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
    samplerInfo.unnormalizedCoordinates = VK_FALSE;
    samplerInfo.compareEnable = VK_FALSE;
    samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 0.0f;
    assert(vkCreateSampler(device_->device_, &samplerInfo, nullptr, &sampler_) == VK_SUCCESS && "failed to create texture sampler!");
}

void Command::MakeDescriptotAndPipeline(){
    //create sampler info
    auto virtual_descriptor=new Descriptor();
    assert(!app_->images_.empty() && "can't use images when there is no images");
    auto set_lay=virtual_descriptor->CreateSetLayout(app_->images_[0],
    {
       // {0,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_VERTEX_BIT},
        {1,VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,VK_SHADER_STAGE_FRAGMENT_BIT}
    }
    );
    graphics_pipeline_=(new PipeLine(swap_chain_,set_lay));
    delete virtual_descriptor;
    
     
    command_buff_.resize(device_->MAX_FRAMES_IN_FLIGHT);
    VkCommandBufferAllocateInfo alloc_inf{};
    alloc_inf.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_inf.commandPool = device_->command_pool_;
    alloc_inf.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_inf.commandBufferCount = command_buff_.size();
    assert(vkAllocateCommandBuffers(device_->device_,&alloc_inf,command_buff_.data())==VK_SUCCESS && "Allocate command buffer fail"); 
}  
VkCommandBuffer&& Command::BeginOneTimeCommand(){
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = device_->command_pool_;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device_->device_, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return std::move(commandBuffer);
}
void Command::EndOneTimeCommand(VkCommandBuffer&& command_buff){
    vkEndCommandBuffer(command_buff);
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &command_buff;
    vkQueueSubmit(device_->graphics_queue_, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device_->graphics_queue_);
    vkFreeCommandBuffers(device_->device_, device_->command_pool_, 1, &command_buff);
}
VkCommandBuffer&& Command::StartCommandBuffer(uint32_t frame){
    vkWaitForFences(device_->device_,1,&device_->in_flight_[frame],VK_TRUE,UINT64_MAX);
    vkResetFences(device_->device_,1,&device_->in_flight_[frame]);
    vkAcquireNextImageKHR(device_->device_,swap_chain_->swap_chain_,UINT64_MAX,device_->image_available_[frame],VK_NULL_HANDLE,&image_index_);
    vkResetCommandBuffer(command_buff_[frame],0);
    auto command_buff=command_buff_[frame];
    VkCommandBufferBeginInfo beg_inf{};
    beg_inf.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    assert(vkBeginCommandBuffer(command_buff,&beg_inf)==VK_SUCCESS && "Begin command buffer fail");
    //start render pass
    VkRenderPassBeginInfo render_pass_beg{};
    render_pass_beg.sType=VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    render_pass_beg.renderPass=swap_chain_->render_pass_;
    render_pass_beg.framebuffer=swap_chain_->swap_chain_frame_buffers_[image_index_];
    render_pass_beg.renderArea.offset={0,0};
    render_pass_beg.renderArea.extent=swap_chain_->swap_chain_extent_;
    VkClearValue clear_col={{{0.0f,0.0f,0.0f,1.0f}}};//color is black
    render_pass_beg.clearValueCount=1;
    render_pass_beg.pClearValues=&clear_col;
    vkCmdBeginRenderPass(command_buff,&render_pass_beg,VK_SUBPASS_CONTENTS_INLINE);
    //draw
    vkCmdBindPipeline(command_buff,VK_PIPELINE_BIND_POINT_GRAPHICS,graphics_pipeline_->graphics_pipeline_);
    //IMPORTANT: what to draw
    // VkDeviceSize* offset;
    // vkCmdBindIndexBuffer(command_buff,index_buff_,0,VK_INDEX_TYPE_UINT16);

    VkViewport view_port{};
    view_port.x=0.0f;
    view_port.y=0.0f;
    view_port.width=(float)swap_chain_->swap_chain_extent_.width;
    view_port.height=(float)swap_chain_->swap_chain_extent_.height;
    view_port.minDepth=0.0f;
    view_port.maxDepth=1.0f;
    vkCmdSetViewport(command_buff,0,1,&view_port);
    VkRect2D scissor{};
    scissor.offset={0,0};
    scissor.extent=swap_chain_->swap_chain_extent_;
    vkCmdSetScissor(command_buff,0,1,&scissor);


    // UniformBuffer ubo;
    //set uniform buffer

    //uniform buffer
    // memcpy(uniform_buff_map_[app_->curr_frame_], &ubo, sizeof(ubo));

    return std::move(command_buff);

}
void Command::SubmitCommandBuffer(uint32_t frame){
    auto command_buff=command_buff_[frame];
    vkCmdEndRenderPass(command_buff);
    assert(vkEndCommandBuffer(command_buff)==VK_SUCCESS && "end command buffer fail");
    VkSubmitInfo submit_inf{};
    submit_inf.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO;
    VkSemaphore wait_se[1]={device_->image_available_[frame]};
    VkPipelineStageFlags wait_stage[1]={VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submit_inf.waitSemaphoreCount=1;
    submit_inf.pWaitSemaphores=wait_se;
    submit_inf.pWaitDstStageMask=wait_stage;
    submit_inf.commandBufferCount=1;
    submit_inf.pCommandBuffers=&command_buff;
    VkSemaphore signal_se[1]={device_->render_finish_[frame]};
    submit_inf.signalSemaphoreCount=1;
    submit_inf.pSignalSemaphores=signal_se;
    assert(vkQueueSubmit(device_->graphics_queue_,1,&submit_inf,device_->in_flight_[frame])==VK_SUCCESS && "submit command fail");
    VkPresentInfoKHR present_inf{};
    present_inf.sType=VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    present_inf.waitSemaphoreCount=1;
    present_inf.pWaitSemaphores=signal_se;
    VkSwapchainKHR swap_chains[1]={swap_chain_->swap_chain_};
    present_inf.swapchainCount=1;
    present_inf.pSwapchains=swap_chains;
    present_inf.pImageIndices=static_cast<decltype(present_inf.pImageIndices)>(&image_index_);
    vkQueuePresentKHR(device_->present_queue_,&present_inf);
}
// VkDescriptorBufferInfo Command::GetBufferDescriptotInfo(int i){
//     return VkDescriptorBufferInfo {
//         uniform_buff_[i],
//         0,VK_WHOLE_SIZE
//     };
// }



Command::~Command(){
    vkDeviceWaitIdle(device_->device_);
    vkDestroySampler(device_->device_,sampler_, nullptr);
    // for(int i=0;i<device_->MAX_FRAMES_IN_FLIGHT;i++){
    //     vkDestroyBuffer(device_->device_,uniform_buff_[i],nullptr);
    //     vkFreeMemory(device_->device_,uniform_buff_mem_[i],nullptr);  
    // }
}
} // namespace Vulkan
