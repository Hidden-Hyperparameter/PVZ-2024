#include "pipeline.h"
namespace Vulkan
{
PipeLine::PipeLine(SwapChain* swap_chain,const VkDescriptorSetLayout& descriptor_set_layout):swap_chain_(swap_chain),device_(swap_chain->device_)/*,descriptot_(descriptor)*/{
    auto vert_shader_code=FileHelper::ReadFile("shaders/simple_shader.vert.spv");
    auto frag_shader_code=FileHelper::ReadFile("shaders/simple_shader.frag.spv");
    assert(vert_shader_code.size()&&frag_shader_code.size() && "load shader code fail");
    VkShaderModule vert_module=CreateShaderModule(vert_shader_code);
    VkShaderModule frag_module=CreateShaderModule(frag_shader_code);

    VkPipelineShaderStageCreateInfo vert_shader_info{};
    vert_shader_info.sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_shader_info.stage=VK_SHADER_STAGE_VERTEX_BIT;
    vert_shader_info.module=vert_module;
    vert_shader_info.pName="main";//invoke the function in shaders

    VkPipelineShaderStageCreateInfo frag_shader_info{};
    frag_shader_info.sType=VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_shader_info.stage=VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_shader_info.module=frag_module;
    frag_shader_info.pName="main";

    VkPipelineShaderStageCreateInfo shader_stages[2]={
        vert_shader_info,
        frag_shader_info
    };

    std::vector<VkDynamicState> dynamic_states={//viewport and scissor
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR  
    };
    VkPipelineDynamicStateCreateInfo dynamic_state_info{};
    dynamic_state_info.sType=VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamic_state_info.dynamicStateCount=dynamic_states.size();
    dynamic_state_info.pDynamicStates=dynamic_states.data();

    //IMPORTANT, interface with the GLSL shader
    auto binding_descript=Vertex::GetBinding();
    auto attribute_descript=Vertex::GetAttribute();
    VkPipelineVertexInputStateCreateInfo vertex_input_info{};
    vertex_input_info.sType=VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_info.vertexBindingDescriptionCount=1;
    vertex_input_info.vertexAttributeDescriptionCount=attribute_descript.size();
    vertex_input_info.pVertexBindingDescriptions=&binding_descript;
    vertex_input_info.pVertexAttributeDescriptions=attribute_descript.data();
    //END IMPORTANT

    VkPipelineInputAssemblyStateCreateInfo input_assembly{};
    input_assembly.sType=VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly.topology=VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;//modify-able
    input_assembly.primitiveRestartEnable=VK_FALSE;

    //IMPORTANT, associated with interface size
    VkViewport view_port{};
    view_port.x=0.0f;
    view_port.y=0.0f;
    view_port.width=swap_chain_->swap_chain_extent_.width;
    view_port.height=swap_chain_->swap_chain_extent_.height;
    view_port.minDepth=0.0f;
    view_port.maxDepth=1.0f;
    VkRect2D scissor{};
    scissor.offset={0,0};
    scissor.extent=swap_chain_->swap_chain_extent_;
    //END IMPORTANT 
    VkPipelineViewportStateCreateInfo view_port_state{};
    view_port_state.sType=VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    view_port_state.viewportCount=1;
    view_port_state.scissorCount=1;
    view_port_state.pViewports=&view_port;
    view_port_state.pScissors=&scissor;

    VkPipelineRasterizationStateCreateInfo rast{};
    rast.sType=VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rast.depthClampEnable=VK_FALSE;
    rast.rasterizerDiscardEnable=VK_FALSE;
    rast.polygonMode = VK_POLYGON_MODE_FILL;
    rast.lineWidth=1.0f;//no more than 1.0f
    rast.cullMode=VK_CULL_MODE_BACK_BIT;
    rast.frontFace=VK_FRONT_FACE_CLOCKWISE;
    rast.depthBiasEnable=VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE;

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    VkPushConstantRange push{};
    push.offset=0;
    push.size=sizeof(PushConst);
    push.stageFlags=VK_SHADER_STAGE_VERTEX_BIT|VK_SHADER_STAGE_FRAGMENT_BIT;

//add descriptor stage


    VkPipelineLayoutCreateInfo pipe_lay_inf{};
    pipe_lay_inf.sType=VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipe_lay_inf.pushConstantRangeCount=1;
    pipe_lay_inf.pPushConstantRanges=&push;
    pipe_lay_inf.setLayoutCount=1;
    pipe_lay_inf.pSetLayouts=&descriptor_set_layout;

    // pipe_lay_inf.pPushConstantRanges=nullptr;
    assert(vkCreatePipelineLayout(device_->device_,&pipe_lay_inf,nullptr,&pipeline_layout_)==VK_SUCCESS && "create pipeline layout fail");   
    VkGraphicsPipelineCreateInfo pipeline_info{};
    pipeline_info.sType=VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipeline_info.stageCount=2;
    pipeline_info.pStages=shader_stages;

    pipeline_info.pVertexInputState=&vertex_input_info;
    pipeline_info.pInputAssemblyState=&input_assembly;
    pipeline_info.pViewportState=&view_port_state;
    pipeline_info.pRasterizationState=&rast;
    pipeline_info.pMultisampleState=&multisampling;
    pipeline_info.pColorBlendState=&colorBlending;
    pipeline_info.pDynamicState=&dynamic_state_info;
    pipeline_info.layout=pipeline_layout_;
    pipeline_info.renderPass=swap_chain_->render_pass_;
    pipeline_info.subpass=0;
    pipeline_info.basePipelineHandle=VK_NULL_HANDLE;

    assert(vkCreateGraphicsPipelines(device_->device_,VK_NULL_HANDLE,1,&pipeline_info,nullptr,&graphics_pipeline_)==VK_SUCCESS && "Create graphics pipeline fail");
    vkDestroyShaderModule(device_->device_,vert_module,nullptr);
    vkDestroyShaderModule(device_->device_,frag_module,nullptr);
}
VkShaderModule PipeLine::CreateShaderModule(const std::vector<char>& code){
    VkShaderModuleCreateInfo creat_inf{};
    creat_inf.sType=VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    creat_inf.codeSize=code.size();
    creat_inf.pCode=reinterpret_cast<const uint32_t*>(code.data());
    VkShaderModule shader_mod;
    assert(vkCreateShaderModule(device_->device_,&creat_inf,nullptr,&shader_mod)==VK_SUCCESS && "create shader module fail");
    return shader_mod;
}
PipeLine::~PipeLine(){
    vkDestroyPipeline(device_->device_,graphics_pipeline_,nullptr);
    vkDestroyPipelineLayout(device_->device_,pipeline_layout_,nullptr);
}
} // namespace Vulkan
