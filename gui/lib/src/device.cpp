#include "device.h"

namespace Vulkan{
Device::Device(App* app):app_(app),enable_validation_lyers_(app->enable_validation_lyers),MAX_FRAMES_IN_FLIGHT(app->MAX_FRAMES_IN_FLIGHT){
    assert((!enable_validation_lyers_)||app->CheckValidationLayer() && "validation layer is not supported");//validation layers are not supported
    //Create Vulkan Instance
    VkApplicationInfo app_info{};
    app_info.sType=VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.pApplicationName="Default";
    app_info.applicationVersion=VK_MAKE_VERSION(1,0,0);
    app_info.pEngineName="No Engine";
    app_info.engineVersion=VK_MAKE_VERSION(1,0,0);
    app_info.apiVersion=VK_API_VERSION_1_0;

    VkInstanceCreateInfo create_info{};
    create_info.sType=VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    create_info.pApplicationInfo=&app_info;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    create_info.enabledExtensionCount = glfwExtensionCount;
    create_info.ppEnabledExtensionNames = glfwExtensions;

    if(enable_validation_lyers_){
        create_info.enabledLayerCount=app_->validation_layers_.size();
        create_info.ppEnabledLayerNames=app_->validation_layers_.data();
    }else{
        create_info.enabledLayerCount = 0;
    }
    assert(vkCreateInstance(&create_info,nullptr,&instance_)==VK_SUCCESS && "create instance failed");
    assert(glfwCreateWindowSurface(instance_,app_->GetGLFWindow(),nullptr,&surface_)==VK_SUCCESS && "Create surface failed");
    //Create physical and logical device
    uint32_t devise_cnt=0;
    vkEnumeratePhysicalDevices(instance_,&devise_cnt,nullptr);
    assert(devise_cnt && "no device supported");//no device supported
    std::vector<VkPhysicalDevice> devices(devise_cnt);
    vkEnumeratePhysicalDevices(instance_,&devise_cnt,devices.data());
    for(const auto& dev:devices){
        if(IsDeviceSuitable(dev)){
            physic_device_=dev;
            break;
        }
    }
    assert(physic_device_!=VK_NULL_HANDLE && "no device supported");
    detail_=QuerySwapChain(physic_device_);

    family_=FindQueueFamilies(physic_device_);
    std::vector<VkDeviceQueueCreateInfo> queue_crete_infos;
    std::set<uint32_t> distinct_queues={family_.graphics_family_.second,family_.present_family_.second};
    float queue_priority=1.0f;
    for(auto que: distinct_queues){
        VkDeviceQueueCreateInfo queue_info{};
        queue_info.sType=VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_info.queueFamilyIndex=que;
        queue_info.queueCount=1;
        queue_info.pQueuePriorities=&queue_priority;
        queue_crete_infos.push_back(queue_info);
    }
    VkPhysicalDeviceFeatures features{};
    features.samplerAnisotropy = VK_TRUE;
    VkDeviceCreateInfo create_infp{};
    create_infp.sType=VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    create_infp.queueCreateInfoCount=queue_crete_infos.size();
    create_infp.pQueueCreateInfos=queue_crete_infos.data();
    create_infp.pEnabledFeatures=&features;
    create_infp.enabledExtensionCount=device_extensions_.size();
    create_infp.ppEnabledExtensionNames=device_extensions_.data();
    if(enable_validation_lyers_){
        create_infp.enabledLayerCount=app_->validation_layers_.size();
        create_infp.ppEnabledLayerNames=app_->validation_layers_.data();
    }else{
        create_infp.enabledLayerCount=0;
    }
    assert(vkCreateDevice(physic_device_,&create_infp,nullptr,&device_)==VK_SUCCESS&&"Create Logical Device failed");
    QueueFamilyIndex index=FindQueueFamilies(physic_device_);
    vkGetDeviceQueue(device_,index.graphics_family_.second,0,&graphics_queue_);
    vkGetDeviceQueue(device_,index.present_family_.second,0,&present_queue_);
    //Create command pool
    VkCommandPoolCreateInfo pool_inf{};
    pool_inf.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    pool_inf.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    pool_inf.queueFamilyIndex = index.graphics_family_.second;
    assert(vkCreateCommandPool(device_,&pool_inf,nullptr,&command_pool_)==VK_SUCCESS && "Failed to create command pool");
    #ifdef VULKAN_DEBUG
    printf("Logic device creation done.\n");
    #endif

    //Create sync objects
    in_flight_.resize(MAX_FRAMES_IN_FLIGHT);
    image_available_.resize(MAX_FRAMES_IN_FLIGHT);
    render_finish_.resize(MAX_FRAMES_IN_FLIGHT);
    VkSemaphoreCreateInfo se_info{};
    se_info.sType=VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkFenceCreateInfo fe_info{};
    fe_info.sType=VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fe_info.flags=VK_FENCE_CREATE_SIGNALED_BIT;//IMPORTANT, otherwise the program is going to wait for the first fence forever.
    for(size_t i=0;i<MAX_FRAMES_IN_FLIGHT;++i){
        assert((
        vkCreateSemaphore(device_,&se_info,nullptr,&image_available_[i])==VK_SUCCESS
    )&&(
        vkCreateSemaphore(device_,&se_info,nullptr,&render_finish_[i])==VK_SUCCESS
    )&&(
        vkCreateFence(device_,&fe_info,nullptr,&in_flight_[i])==VK_SUCCESS
    ) && "Create sync objects fail");
    } 
}
SwapChainSupportDetail Device::QuerySwapChain(VkPhysicalDevice device){
    SwapChainSupportDetail det;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device,surface_,&det.capab_);
    uint32_t format_cnt;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device,surface_,&format_cnt,nullptr);
    if(format_cnt){
        det.formats_.resize(format_cnt);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device,surface_,&format_cnt,det.formats_.data());
    }
    uint32_t present_cnt;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device,surface_,&present_cnt,nullptr);
    if(present_cnt){
        det.present_mode_.resize(present_cnt);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,surface_,&present_cnt,det.present_mode_.data());
    }
    return det;
}
bool Device::CheckDeviceExtensionSupport(VkPhysicalDevice device){
   uint32_t extension_cnt;
   vkEnumerateDeviceExtensionProperties(device,nullptr,&extension_cnt,nullptr);
   std::vector<VkExtensionProperties> availbe_extensions(extension_cnt);
   vkEnumerateDeviceExtensionProperties(device,nullptr,&extension_cnt,availbe_extensions.data());
   std::set<std::string> require_extension(device_extensions_.begin(),device_extensions_.end());
   for(const auto& extension:availbe_extensions){
    require_extension.erase(extension.extensionName);
   }
   return require_extension.empty();
}
bool Device::IsDeviceSuitable(VkPhysicalDevice device){
    QueueFamilyIndex index = FindQueueFamilies(device);
    if(!index.IsGood())return false;
    bool extension_support = CheckDeviceExtensionSupport(device);
    if(!extension_support)return false;
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    if(supportedFeatures.samplerAnisotropy!=VK_TRUE)return false;
    SwapChainSupportDetail swap_chain=QuerySwapChain(device);
    bool  swap_chain_adequate=(!swap_chain.formats_.empty())&&(!swap_chain.present_mode_.empty());
    return swap_chain_adequate;
}
QueueFamilyIndex Device::FindQueueFamilies(VkPhysicalDevice devide){
    QueueFamilyIndex index;
    uint32_t queue_family_cnt=0;
    vkGetPhysicalDeviceQueueFamilyProperties(devide,&queue_family_cnt,nullptr);
    std::vector<VkQueueFamilyProperties> queue_famlies(queue_family_cnt);
    vkGetPhysicalDeviceQueueFamilyProperties(devide,&queue_family_cnt,queue_famlies.data());
    int i=0;
    for(const auto& queue_fml:queue_famlies){
        if(queue_fml.queueFlags&VK_QUEUE_GRAPHICS_BIT){
            index.graphics_family_=std::make_pair(true,i);   
        }
        VkBool32 present_support = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(devide,i,surface_,&present_support);
        if(present_support){
            index.present_family_=std::make_pair(true,i);
        }
        i++;
    }
    return index;
}
uint32_t Device::FindMemoryType(uint32_t type_filter,VkMemoryPropertyFlags properties){
    VkPhysicalDeviceMemoryProperties mem_prop;
    vkGetPhysicalDeviceMemoryProperties(physic_device_,&mem_prop);
    for(uint32_t i=0;i<mem_prop.memoryTypeCount;++i){
        if(type_filter&(1<<i)){
            if((mem_prop.memoryTypes[i].propertyFlags & properties)==properties)return i;
        }
    }
    assert(false&&"Can't find memory type");
}
void Device::CreateGeneralBuffer(VkDeviceSize size_,VkBufferUsageFlags usage, VkMemoryPropertyFlags props,VkBuffer& buf,VkDeviceMemory& buf_mem){
    VkBufferCreateInfo buf_inf{};
    buf_inf.sType=VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    buf_inf.size=size_;
    buf_inf.usage=usage;
    buf_inf.sharingMode=VK_SHARING_MODE_EXCLUSIVE;
    assert(vkCreateBuffer(device_,&buf_inf,nullptr,&buf)==VK_SUCCESS && "create buffer fail");
    VkMemoryRequirements mem_req;
    
    vkGetBufferMemoryRequirements(device_,buf,&mem_req);
    VkMemoryAllocateInfo alloc_inf{};
    alloc_inf.sType=VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    alloc_inf.allocationSize=mem_req.size;
    alloc_inf.memoryTypeIndex=FindMemoryType(mem_req.memoryTypeBits,props);
    assert(vkAllocateMemory(device_,&alloc_inf,nullptr,&buf_mem)==VK_SUCCESS && "allocate buffer memory fail");
    vkBindBufferMemory(device_,buf,buf_mem,0);
}
void Device::CopyBuffer(VkBuffer src,VkBuffer dest,VkDeviceSize sz){
    VkCommandBufferAllocateInfo alloc_inf{};
    alloc_inf.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    alloc_inf.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    alloc_inf.commandPool=command_pool_;
    alloc_inf.commandBufferCount=1;
    VkCommandBuffer command_buffer;
    vkAllocateCommandBuffers(device_,&alloc_inf,&command_buffer);
    VkCommandBufferBeginInfo begin_inf{};
    begin_inf.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    begin_inf.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; 
    vkBeginCommandBuffer(command_buffer,&begin_inf);
    VkBufferCopy copy_region{};
    copy_region.srcOffset=0;
    copy_region.dstOffset=0;
    copy_region.size=sz;
    vkCmdCopyBuffer(command_buffer,src,dest,1,&copy_region);
    vkEndCommandBuffer(command_buffer);
    VkSubmitInfo submit_inf{};
    submit_inf.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submit_inf.commandBufferCount=1;
    submit_inf.pCommandBuffers=&command_buffer;
    vkQueueSubmit(graphics_queue_,1,&submit_inf,VK_NULL_HANDLE);
    vkQueueWaitIdle(graphics_queue_);
    vkFreeCommandBuffers(device_,command_pool_,1,&command_buffer);
}
template<typename T>
void Device::CreateBuffer(VkBuffer& buf,VkDeviceMemory& buf_mem,VkBufferUsageFlagBits flag,const std::vector<T>& datas){
    VkDeviceSize sz=datas.size()*sizeof(T);
    VkBuffer stage_buf;VkDeviceMemory stage_mem;
    CreateGeneralBuffer(sz,VK_BUFFER_USAGE_TRANSFER_SRC_BIT,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,stage_buf,stage_mem);
    //copy the data to the allocated memory
    void* data;
    vkMapMemory(device_,stage_mem,0,sz,0,&data);
    memcpy(data,datas.data(),sz);
    vkUnmapMemory(device_,stage_mem);
    CreateGeneralBuffer(sz,VK_BUFFER_USAGE_TRANSFER_DST_BIT|flag,VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,buf,buf_mem);
    CopyBuffer(stage_buf,buf,sz);
    #ifdef VULKAN_DEBUG
    printf("Create buffer:done.\n");
    #endif
    vkDestroyBuffer(device_,stage_buf,nullptr);
    vkFreeMemory(device_,stage_mem,nullptr);
}
/**
 * These two lines are used for solving the template un-compileable issue. I have learnt it through Alist.(Yao-class-cpp-studio assignment 3)
*/
template void Device::CreateBuffer(VkBuffer& buf,VkDeviceMemory& buf_mem,VkBufferUsageFlagBits flag,const std::vector<Vertex>& datas);
template void Device::CreateBuffer(VkBuffer& buf,VkDeviceMemory& buf_mem,VkBufferUsageFlagBits flag,const std::vector<uint16_t>& datas);

void Device::KillBuffer(VkBuffer& buf,VkDeviceMemory& buf_mem){
    vkDestroyBuffer(device_,buf,nullptr);
    vkFreeMemory(device_,buf_mem,nullptr);
}


Device::~Device(){
    for(size_t i=0;i<MAX_FRAMES_IN_FLIGHT;++i){
    vkDestroySemaphore(device_,image_available_[i],nullptr);
    vkDestroySemaphore(device_,render_finish_[i],nullptr);
    vkDestroyFence(device_,in_flight_[i],nullptr);
    }
    vkDestroyCommandPool(device_,command_pool_,nullptr);
    vkDestroySurfaceKHR(instance_,surface_,nullptr);
    vkDestroyDevice(device_,nullptr);
    vkDestroyInstance(instance_,nullptr);
}
}//namespace Vulkan