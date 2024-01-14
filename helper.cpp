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

	//These two below are external graphics function which are used to make the effect of putimage() better,
	// so that there won't be black bars around the item picture.
	void _putimagePNG(int  picture_x, int picture_y, IMAGE* picture) {
		DWORD* dst = GetImageBuffer();
		DWORD* draw = GetImageBuffer();
		DWORD* src = GetImageBuffer(picture);
		int picture_width = picture->getwidth();
		int picture_height = picture->getheight();
		int graphWidth = getwidth();
		int graphHeight = getheight();
		int dstX = 0;
		for (int iy = 0; iy < picture_height; iy++)
		{
			for (int ix = 0; ix < picture_width; ix++)
			{
				int srcX = ix + iy * picture_width;
				int sa = ((src[srcX] & 0xff000000) >> 24);//R
				int sr = ((src[srcX] & 0xff0000) >> 16);  //G
				int sg = ((src[srcX] & 0xff00) >> 8);     //B
				int sb = src[srcX] & 0xff;
				if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
				{
					dstX = (ix + picture_x) + (iy + picture_y) * graphWidth;
					int dr = ((dst[dstX] & 0xff0000) >> 16);
					int dg = ((dst[dstX] & 0xff00) >> 8);
					int db = dst[dstX] & 0xff;
					draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)
						| ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)
						| (sb * sa / 255 + db * (255 - sa) / 255);
				}
			}
		}
	}
	//when in use, call this function
	void PutTransparentImage(int x, int y, IMAGE* picture) {
		IMAGE imgTmp, imgTmp2, imgTmp3;
		int winWidth = getwidth();
		int winHeight = getheight();
		if (y < 0) {
			SetWorkingImage(picture);
			getimage(&imgTmp, 0, -y,
				picture->getwidth(), picture->getheight() + y);
			SetWorkingImage();
			y = 0;
			picture = &imgTmp;
		}
		else if (y >= getheight() || x >= getwidth()) {
			return;
		}
		else if (y + picture->getheight() > winHeight) {
			SetWorkingImage(picture);
			getimage(&imgTmp, x, y, picture->getwidth(), winHeight - y);
			SetWorkingImage();
			picture = &imgTmp;
		}
		if (x < 0) {
			SetWorkingImage(picture);
			getimage(&imgTmp2, -x, 0, picture->getwidth() + x, picture->getheight());
			SetWorkingImage();
			x = 0;
			picture = &imgTmp2;
		}
		if (x > winWidth - picture->getwidth()) {
			SetWorkingImage(picture);
			getimage(&imgTmp3, 0, 0, winWidth - x, picture->getheight());
			SetWorkingImage();
			picture = &imgTmp3;
		}
		_putimagePNG(x, y, picture);
	}
}