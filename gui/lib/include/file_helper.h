#pragma once
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include <map>
#include <stb_image.h>

#ifdef VULKAN_DEBUG
    #ifdef _WIN32
        #include <direct.h>
        #include <Windows.h>
    #else
        #include <unistd.h>
    #endif
#endif
class FileHelper{
public:
    static std::vector<char> ReadFile(const std::string& file_name);
    typedef stbi_uc IMAGE;
    static std::tuple<int,int,FileHelper::IMAGE*> FileHelper::LoadImage(const std::string& file_name); 
    static void FreeImage(IMAGE* image);
};