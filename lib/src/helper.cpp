#include "helper.h"
// #define WCHAR_IS_CHAR
namespace Helper {
	const wchar_t* CharToWchar(const char* path) {
		wchar_t final_path[128];
		for (int j = 0;; j++) {
			#ifndef WCHAR_IS_CHAR
			final_path[j] = (wchar_t)path[j];
			#else
			final_path[j] = path[j];
			#endif
			if (path[j] == 0)break;
		}
		return final_path;
	}
	const wchar_t* NumToWchar(int x) {
		auto ch=(NumToString(x)).c_str();
		//sprintf_s(ch,sizeof(ch), "%d", x);
		//std::cout << "To " << (std::string)(ch) << std::endl;
		return CharToWchar(ch);
	}
	std::string NumToString(int x){
		std::string ch;
		if (x) {
			int num[5], cnt = 0;
			while (x) {
				num[cnt++] = x % 10; x /= 10;
			}
			for (int i = 0; i < cnt; i++) {
				ch += ( num[cnt - i - 1] + '0');
			}
		}
		else {
			ch="0";
		}
		return ch;
	}
}