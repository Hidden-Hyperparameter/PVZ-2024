#include "swap_chain.h"
namespace Vulkan
{
SwapChain::SwapChain(Device* device):device_(device){
    SwapChainSupportDetail support = device_->detail_;
    VkSurfaceFormatKHR surface=ChooseSwapSurfaceFormat(support.formats_);
    VkPresentModeKHR present=ChooseSwapPresentMode(support.present_mode_);
    VkExtent2D extent = ChooseSwapExtent(support.capab_);
    uint32_t image_cnt=support.capab_.minImageCount+1;
    uint32_t mx=support.capab_.maxImageCount;
    if(image_cnt>mx&&mx>0){
        image_cnt=mx;
    }

    VkSwapchainCreateInfoKHR create_inf{};
    create_inf.sType=VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    create_inf.surface=device_->surface_;
    create_inf.minImageCount=image_cnt;
    create_inf.imageFormat=surface.format;
    create_inf.imageColorSpace=surface.colorSpace;
    create_inf.imageExtent=extent;
    create_inf.imageArrayLayers=1;
    create_inf.imageUsage=VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    QueueFamilyIndex index=device_->family_;
    uint32_t queue_index[2]={index.graphics_family_.second,index.present_family_.second};
    if(index.graphics_family_!=index.present_family_){
        create_inf.imageSharingMode=VK_SHARING_MODE_CONCURRENT;
        create_inf.queueFamilyIndexCount=2;
        create_inf.pQueueFamilyIndices=queue_index;
    }else{
        create_inf.imageSharingMode=VK_SHARING_MODE_EXCLUSIVE;
    }
    create_inf.preTransform=support.capab_.currentTransform;
    create_inf.compositeAlpha=VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    create_inf.presentMode=present;
    create_inf.clipped=VK_TRUE;
    create_inf.oldSwapchain=VK_NULL_HANDLE;
    assert(vkCreateSwapchainKHR(device_->device_,&create_inf,nullptr,&swap_chain_)==VK_SUCCESS && "Create swap chain fail");
    
    vkGetSwapchainImagesKHR(device_->device_,swap_chain_,&image_cnt,nullptr);
    swap_chain_images_.resize(image_cnt);
    vkGetSwapchainImagesKHR(device_->device_,swap_chain_,&image_cnt,swap_chain_images_.data());
    swap_chain_image_format_=surface.format;
    swap_chain_extent_=extent;
    #ifdef VULKAN_DEBUG
    printf("Swap chain construction:done.\n");
    #endif

    CreateImageViews();
    CreateRenderPass();
    CreateFrameBuffers();
}
VkSurfaceFormatKHR SwapChain::ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& avaiable){
    for(const auto& av:avaiable){
        if(av.format==VK_FORMAT_B8G8R8A8_SRGB&&av.colorSpace==VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)return av;
    }
    return avaiable[0];
}
VkPresentModeKHR SwapChain::ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& available){
    for(const auto& av:available){
        if(av==VK_PRESENT_MODE_MAILBOX_KHR)return av;
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}
VkExtent2D SwapChain::ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& caps){
    if(caps.currentExtent.width!=0x7fffffff){
        return caps.currentExtent;
    }else{
        int width,height;
        glfwGetFramebufferSize(device_->app_->GetGLFWindow(),&width,&height);
        VkExtent2D actual={(uint32_t)width,(uint32_t)height};
        typedef uint32_t T;
        auto fit_in = [=](T a,T b,T c){return (a<b)?b:((a>c)?c:a);};
        actual.width=fit_in(actual.width,caps.minImageExtent.width,caps.maxImageExtent.width);
        actual.height=fit_in(actual.height,caps.minImageExtent.height,caps.maxImageExtent.height);
        return actual;
    }
}
void SwapChain::CreateImageViews(){
    swap_chain_image_views_.resize(swap_chain_images_.size());
    for(size_t i=0;i<swap_chain_images_.size();++i){
        VkImageViewCreateInfo create_inf{};
        create_inf.sType=VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        create_inf.image=swap_chain_images_[i];
        create_inf.viewType=VK_IMAGE_VIEW_TYPE_2D;
        create_inf.format=swap_chain_image_format_;
        create_inf.components.r=create_inf.components.g=create_inf.components.b=create_inf.components.a=VK_COMPONENT_SWIZZLE_IDENTITY;
        auto& something=create_inf.subresourceRange;
        something.aspectMask=VK_IMAGE_ASPECT_COLOR_BIT;
        something.baseMipLevel=0;
        something.levelCount=1;
        something.baseArrayLayer=0;
        something.layerCount=1;
        assert(vkCreateImageView(device_->device_,&create_inf,nullptr,&swap_chain_image_views_[i])==VK_SUCCESS && "Create image view fail");
    }
}
void SwapChain::CreateFrameBuffers(){
    swap_chain_frame_buffers_.resize(swap_chain_image_views_.size());
    for(size_t i=0;i<swap_chain_image_views_.size();++i){
        VkImageView attachments[1]={swap_chain_image_views_[i]};
        VkFramebufferCreateInfo frame_buf_inf{};
        frame_buf_inf.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        frame_buf_inf.renderPass = render_pass_;
        frame_buf_inf.attachmentCount = 1;
        frame_buf_inf.pAttachments = attachments;
        frame_buf_inf.width = swap_chain_extent_.width;
        frame_buf_inf.height = swap_chain_extent_.height;
        frame_buf_inf.layers = 1;
        assert(vkCreateFramebuffer(device_->device_,&frame_buf_inf,nullptr,&swap_chain_frame_buffers_[i])==VK_SUCCESS && "Create frame buffer fail");
        #ifdef VULKAN_DEBUG
        printf("Create frame buffer no.%d:done.\n",i);
        #endif
    }
}
SwapChain::~SwapChain(){
    for(auto frame:swap_chain_frame_buffers_){
        vkDestroyFramebuffer(device_->device_,frame,nullptr);
    } 
    vkDestroyRenderPass(device_->device_,render_pass_,nullptr);
    for(auto image_view :swap_chain_image_views_){
        vkDestroyImageView(device_->device_,image_view,nullptr);
    }
    vkDestroySwapchainKHR(device_->device_,swap_chain_,nullptr);
}
void SwapChain::CreateRenderPass(){
    VkAttachmentDescription color_attach{};
    color_attach.format=swap_chain_image_format_;
    color_attach.samples=VK_SAMPLE_COUNT_1_BIT;
    color_attach.loadOp=VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attach.storeOp=VK_ATTACHMENT_STORE_OP_STORE;
    color_attach.stencilLoadOp=VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attach.stencilStoreOp=VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attach.initialLayout=VK_IMAGE_LAYOUT_UNDEFINED;
    color_attach.finalLayout=VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    VkAttachmentReference color_attach_ref{};
    color_attach_ref.attachment=0;
    color_attach_ref.layout=VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription sub{};
    sub.pipelineBindPoint=VK_PIPELINE_BIND_POINT_GRAPHICS;
    sub.colorAttachmentCount=1;
    sub.pColorAttachments=&color_attach_ref;

    VkRenderPassCreateInfo render_pass_info{};
    render_pass_info.sType=VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount=1;
    render_pass_info.pAttachments=&color_attach;
    render_pass_info.subpassCount=1;
    render_pass_info.pSubpasses=&sub;

    VkSubpassDependency dep{};
    dep.srcSubpass=VK_SUBPASS_EXTERNAL;
    dep.dstSubpass=0;
    dep.srcStageMask=VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;//wait for this
    dep.srcAccessMask=0;
    dep.dstStageMask=VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dep.dstAccessMask=VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;//involve action
    render_pass_info.dependencyCount=1;
    render_pass_info.pDependencies=&dep;
    assert(vkCreateRenderPass(device_->device_,&render_pass_info,nullptr,&render_pass_)==VK_SUCCESS&& "Create render pass fail");
}
} // namespace Vulkan
