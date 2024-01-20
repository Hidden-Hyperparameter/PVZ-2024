#version 450

// layout(binding = 0) uniform UniformBufferObject {
//     mat4 model;
//     mat4 view;
//     mat4 proj;
// } ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(push_constant)uniform Push{
    vec2 offset;
    vec3 color;
}push;
void main() {
    // gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition+push.offset, 0.0, 1.0);
    gl_Position =  vec4(inPosition+push.offset, 0.0, 1.0);
    // gl_Position =  vec4(inPosition+push.offset, 0.0, 1.0);
    // fragColor = vec3(1.0,1.0,0.0);
    fragTexCoord = vec2(-inTexCoord[0],inTexCoord[1]);
    // fragColor=push.color;
    fragColor=inColor;
}