#include "Logger.h"
#include <dxgidebug.h>

namespace Logger {
	//出力ウィンドウに文字を出す関数
	void Log(const std::string& message) {
		OutputDebugStringA(message.c_str());
	}
	void Log(const std::wstring& message) {
		OutputDebugStringW(message.c_str());
	}
}