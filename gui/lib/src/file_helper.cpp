#include "file_helper.h"
#define RELATIVE {"../../","","gui/","../../gui/"} //Change this line based on the current working directory. More specifically, please APPEND instead of REMOVE items from the list.
std::vector<char> FileHelper::ReadFile(const std::string& file_name){
    #ifdef VULKAN_DEBUG
    char cwd[1024];
    _getcwd(cwd, sizeof(cwd));
    #ifdef GUI_TEST
    printf("(Now we are working on GUI test mode)\n");
    #endif
    printf("Current working directory: %s\n", cwd);
    #endif
    std::vector<std::string> relative=RELATIVE;
    for(auto try_path:relative){
        std::string new_name=try_path+file_name;
        std::ifstream f(new_name,std::ios::ate|std::ios::binary);
        std::cout<<"Loading file named ["<<new_name<<"]..."<<std::endl;
        if(f.is_open()){
            size_t f_size=f.tellg();
            std::vector<char> buf(f_size);
            f.seekg(0);
            f.read(buf.data(),f_size);
            f.close();
            std::cout<<"Succeeded.\n";
            return buf;
        }
    }
    /**
     * If this line fails, edit the definition of macro RELATIVE
     * 
     */
    assert("failed to load file"&&false);
}
std::tuple<int,int,FileHelper::IMAGE*> FileHelper::LoadImage(const std::string& file_name){
    #ifdef VULKAN_DEBUG
    char cwd[1024];
    #ifdef _WIN32
    _getcwd(cwd, sizeof(cwd));
    #else
    getcwd(cwd, sizeof(cwd));
    #endif
    #ifdef GUI_TEST
    printf("(Now we are working on GUI test mode)\n");
    #endif
    printf("Current working directory: %s\n", cwd);
    #endif
    int tex_width,tex_height,tex_channel;
    const std::vector<std::string> relative=RELATIVE;
    for(auto try_path:relative){
        std::string new_name=try_path+file_name;
        std::cout<<"Loading image named ["<<new_name<<"]..."<<std::endl;
        IMAGE* pixel=stbi_load(new_name.c_str(),&tex_width,&tex_height,&tex_channel,STBI_rgb_alpha);
        if(pixel){
            std::cout<<"Succeeded.\n";
            return std::make_tuple(tex_width,tex_height,pixel);
        }
        // uint64_t image_size=tex_height*tex_width*4;
    }
    std::cout<<"Can't find the image"<<std::endl;
    /**
     * If this line fails, edit the definition of macro RELATIVE
     * 
     */
    return std::make_tuple(0,0,nullptr);
}
void FileHelper::FreeImage(FileHelper::IMAGE* image){
    assert(image && "can't free a empty image");
    stbi_image_free(image);
}
#undef RELATIVE
