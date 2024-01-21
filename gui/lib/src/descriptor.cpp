#include "descriptor.h"
namespace Vulkan {
const VkDescriptorSetLayout& Descriptor::CreateSetLayout(Image* image,const std::vector<std::tuple<uint32_t, VkDescriptorType, VkShaderStageFlags> >& binding_list){
  image_=image,device_=image_->commander_->device_;
      max_set_=(image_->commander_->MAX_FRAMES_IN_FLIGHT) ;

  for (auto tp : binding_list) {
    uint32_t binding;
    VkDescriptorType descriptorType;
    VkShaderStageFlags stageFlags;
    std::tie(binding, descriptorType, stageFlags) = tp;
    assert(!bindings_.count(binding) && "Binding already in use");
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = binding;
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = stageFlags;
    bindings_[binding] = layoutBinding;
  }
  std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings{};
  for (auto kv : bindings_) {
    setLayoutBindings.push_back(kv.second);
  }

  VkDescriptorSetLayoutCreateInfo descriptorSetLayoutInfo{};
  descriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  descriptorSetLayoutInfo.bindingCount = (setLayoutBindings.size());
  descriptorSetLayoutInfo.pBindings = setLayoutBindings.data();

  assert(
      vkCreateDescriptorSetLayout(
          device_->device_,
          &descriptorSetLayoutInfo,
          nullptr,
          &set_layout_) == VK_SUCCESS &&
      "failed to create descriptor set layout!");
  return set_layout_;
}
void Descriptor::ContinueConstruct(
    const std::vector<std::pair<VkDescriptorType,uint32_t> >& pool_size){
 for (auto p : pool_size) {
    pool_size_.push_back({p.first, p.second});
  }

  VkDescriptorPoolCreateInfo descriptorPoolInfo{};
  descriptorPoolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
  descriptorPoolInfo.poolSizeCount = pool_size_.size();
  descriptorPoolInfo.pPoolSizes = pool_size_.data();
  descriptorPoolInfo.maxSets = max_set_;
  descriptorPoolInfo.flags = 0;
  assert(
      vkCreateDescriptorPool(device_->device_, &descriptorPoolInfo, nullptr, &descriptor_pool_) ==
          VK_SUCCESS &&
      "Failed to create descriptor pool\n");

  descriptor_set_.resize(device_->MAX_FRAMES_IN_FLIGHT);

  for (int i = 0; i < descriptor_set_.size(); ++i) {
std::vector<VkWriteDescriptorSet> writes_;

    for(int binding=0;binding<2;++binding){
      if(bindings_.count(binding)==0)continue;
      assert(bindings_.count(binding) == 1 && "Layout does not contain specified binding");
      auto& bindingDescription = bindings_[binding];
      assert(
          bindingDescription.descriptorCount == 1 &&
          "Binding single descriptor info, but binding expects multiple");
      auto tp=bindingDescription.descriptorType;
      VkWriteDescriptorSet write{};
      write.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
      write.descriptorType = tp;
      write.dstBinding = binding;
      write.descriptorCount = 1;
      // if(tp==VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER){
      //   auto buffer_info = commander_->GetBufferDescriptotInfo(i);
      // write.pBufferInfo = &buffer_info;

      // }else
       if(tp==VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER){
        auto image_info = image_->GetImageDescriptotInfo(i);
        write.pImageInfo=&image_info;
      }else{
        assert(false&& "not supported");
      }

      writes_.push_back(write);
    }
    VkDescriptorSetAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = descriptor_pool_;
      allocInfo.pSetLayouts = &set_layout_;
      allocInfo.descriptorSetCount = 1;

      bool success =
          (vkAllocateDescriptorSets(device_->device_, &allocInfo, &descriptor_set_[i]) == VK_SUCCESS);
     
        for (auto& write_one : writes_) {
          write_one.dstSet = descriptor_set_[i];
        }
        vkUpdateDescriptorSets(device_->device_, writes_.size(), writes_.data(), 0, nullptr);
  }
}
// Descriptor::Descriptor(
//     Command* command,
//     const std::vector<std::pair<VkDescriptorType, uint32_t> >& pool_size,
//     const std::vector<std::tuple<uint32_t, VkDescriptorType, VkShaderStageFlags> >& binding_list)
//   {
    
// }
Descriptor::~Descriptor() {
if(!descriptor_set_.empty()){
    vkFreeDescriptorSets(
      device_->device_,
      descriptor_pool_,
      descriptor_set_.size(),
      descriptor_set_.data());
  vkDestroyDescriptorPool(device_->device_, descriptor_pool_, nullptr);
}
}
}  // namespace Vulkan
