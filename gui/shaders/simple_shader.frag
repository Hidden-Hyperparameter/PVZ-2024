#version 450
layout(binding=1)uniform sampler2D texSamper;//this line of code must be put on the top
layout(location=0)in vec3 fragColor;
layout(location=1)in vec2 fragTexCoord;
layout(location=0)out vec4 outColor;
void main(){
    outColor=texture(texSamper,fragTexCoord);//r,g,b,alpha
    // outColor=vec4(fragTexCoord,0.0,1.0);
    // outColor=vec4(push.color,1.0);
}