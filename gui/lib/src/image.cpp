#include "image.h"
namespace Vulkan
{

Image::Image(Command* commander,int window_x,int window_y,int x_size,int y_size,int x_pos,int y_pos):device_(commander->device_),commander_(commander),x_size_(x_size),y_size_(y_size),x_pos_(x_pos),y_pos_(y_pos),WINDOW_Width(window_x),WINDOW_Height(window_y){
    /**
     * @brief fix that so that the size is no longer 50!
     * 
     */
    vertexs_=init_vertexs;
    float x_normalized_=(float)x_pos/window_x;
    float x_size_normalized_=(float)x_size_/window_x;
    float y_normalized_=(float)y_pos/window_y;
    float y_size_normalized_=(float)y_size_/window_y;
    // vertexs_[0].pos={2*x_normalized_-1,2*y_normalized_-1};
    // vertexs_[1].pos={2*x_normalized_+2*x_size_normalized_-1,2*y_normalized_-1};
    // vertexs_[2].pos={2*x_normalized_+2*x_size_normalized_-1,2*y_normalized_+2*y_size_normalized_-1};
    // vertexs_[3].pos={2*x_normalized_-1,2*y_normalized_+2*y_size_normalized_-1};
    vertexs_[0].pos={-1,-1};
    vertexs_[3].pos={-1,2*y_size_normalized_-1};
    vertexs_[2].pos={2*x_size_normalized_-1,2*y_size_normalized_-1};
    vertexs_[1].pos={2*x_size_normalized_-1,-1};
    device_->CreateBuffer(vertex_buff_,vertex_buff_memory_,VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,vertexs_);
    device_->CreateBuffer(index_buff_,index_buff_memory_,VK_BUFFER_USAGE_INDEX_BUFFER_BIT,index);
    //descriptot 
}
void Image::Draw(const VkCommandBuffer& command_buff,int curr_frame_){
    vkCmdBindDescriptorSets(
        command_buff,VK_PIPELINE_BIND_POINT_GRAPHICS,
        commander_->graphics_pipeline_->pipeline_layout_,
        0,1,
        &descriptot_->descriptor_set_[commander_->app_->curr_frame_],
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
void Image::Update(int move_x,int move_y){
    x_pos_=move_x,y_pos_=move_y;
}
bool Image::Load(const std::string& image_name_){
    FileHelper::IMAGE* im=nullptr;uint32_t w,h;
    std::tie(w,h,im)=FileHelper::LoadImage(image_name_);//this line should be modified
    if(im==nullptr)return false;
    auto image_size=4*w*h;
    /**
     * FIXME: is this line functioning?
     * 
     */
    x_size_=w,y_size_=h;
    device_->CreateGeneralBuffer(image_size,VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, staging_buff_, staging_buff_mem_);
    void* data;
    vkMapMemory(device_->device_, staging_buff_mem_, 0, image_size, 0, &data);
    memcpy(data, im, image_size);
    vkUnmapMemory(device_->device_, staging_buff_mem_);
    FileHelper::FreeImage(im);
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = static_cast<uint32_t>(w);
    imageInfo.extent.height = static_cast<uint32_t>(h);
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    assert(vkCreateImage(device_->device_, &imageInfo, nullptr, &image_) == VK_SUCCESS&&"failed to create image!");
    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device_->device_, image_, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = device_->FindMemoryType(memRequirements.memoryTypeBits,     VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    assert(vkAllocateMemory(device_->device_, &allocInfo, nullptr, &image_mem_) == VK_SUCCESS && "failed to allocate image memory!");

    vkBindImageMemory(device_->device_, image_, image_mem_, 0);

    Transition(image_, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
    VkCommandBuffer commandBuffer = commander_->BeginOneTimeCommand();
    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = {0, 0, 0};
    region.imageExtent = {w,h,1};
    vkCmdCopyBufferToImage(
        commandBuffer,
        staging_buff_,
        image_,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );
    commander_->EndOneTimeCommand(std::move(commandBuffer));
    Transition(image_, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    vkDestroyBuffer(device_->device_, staging_buff_, nullptr);
    vkFreeMemory(device_->device_, staging_buff_mem_, nullptr);
//Create image view
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image =image_;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = VK_FORMAT_R8G8B8A8_SRGB;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;
    assert(vkCreateImageView(device_->device_, &viewInfo, nullptr, &image_view_) == VK_SUCCESS && "Create image view fails");
    descriptot_=new Descriptor();
    auto set_lay=descriptot_->CreateSetLayout(this,
    {
       // {0,VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,VK_SHADER_STAGE_VERTEX_BIT},
        {1,VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,VK_SHADER_STAGE_FRAGMENT_BIT}
    }
    );
descriptot_->ContinueConstruct(
    {//these are descriptor info
        //{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,MAX_FRAMES_IN_FLIGHT},
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,commander_->MAX_FRAMES_IN_FLIGHT}
    }
);
return true;
} 

void Image::Transition(VkImage image, VkFormat format, VkImageLayout old,VkImageLayout new_){
    VkCommandBuffer command_buff = commander_->BeginOneTimeCommand();
    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout =old;
    barrier.newLayout = new_;
    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (old == VK_IMAGE_LAYOUT_UNDEFINED && new_ == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    } else if (old == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_ ==    VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    } else {
        assert(false && "unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        command_buff,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );
    commander_->EndOneTimeCommand(std::move(command_buff));


}
VkDescriptorImageInfo Image::GetImageDescriptotInfo(int i){
    return VkDescriptorImageInfo{
        commander_->sampler_,
        image_view_,
         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
    };
}
Image::~Image(){
    vkDestroyImageView(device_->device_,image_view_,nullptr);
    vkDestroyImage(device_->device_, image_, nullptr);
    vkFreeMemory(device_->device_, image_mem_, nullptr);
}

} // namespace Vulkan
