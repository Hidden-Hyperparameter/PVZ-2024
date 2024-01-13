namespace Helper {
	 const wchar_t *CharToWchar(const char* path) {
		 wchar_t final_path[100];
		for (int j = 0;; j++) {
			final_path[j] = (wchar_t)path[j];
			if (path[j] == 0)break;
		}
		return final_path;
	}

}