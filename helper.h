#pragma once
#include <stdio.h>
#include <iostream>
#include "helper.h"
namespace Helper {
	const wchar_t* CharToWchar(const char* path) {
		wchar_t final_path[128];
		for (int j = 0;; j++) {
			final_path[j] = (wchar_t)path[j];
			if (path[j] == 0)break;
		}
		return final_path;
	}
	const wchar_t* NumToWchar(int x) {
		char ch[64];
		if (x) {
			int num[4], cnt = 0;
			while (x) {
				num[cnt++] = x % 10; x /= 10;
			}
			for (int i = 0; i < cnt; i++) {
				ch[i] = num[cnt - i - 1] + '0';
			}ch[cnt] = 0;
		}
		else {
			ch[0] = '0', ch[1] = 0;
		}
		
		//sprintf_s(ch,sizeof(ch), "%d", x);
		//std::cout << "To " << (std::string)(ch) << std::endl;
		return CharToWchar(ch);
	}
};