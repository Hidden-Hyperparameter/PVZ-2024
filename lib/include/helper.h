#pragma once
#ifndef __HELPER__
#define __HELPER__
#include <stdio.h>
#include <iostream>


namespace Helper {
	const wchar_t* CharToWchar(const char* path);
	const wchar_t* NumToWchar(int x);
	/**
	 * @brief change int to string
	 * @attention x must be non-negative
	 */
	std::string NumToString(int x);
};


#endif