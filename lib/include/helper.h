#pragma once
#ifndef __HELPER__
#define __HELPER__
#include <stdio.h>
#include <iostream>
// #include <graphics.h>

/**
 * FIXME:
 * 
 */
#define IMAGE std::pair<int,int>

namespace Helper {
	const wchar_t* CharToWchar(const char* path);
	const wchar_t* NumToWchar(int x);
	void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture);
	void PutTransparentImage(int x, int y, IMAGE* picture)=delete;
	std::string NumToString(int x);
};


#endif