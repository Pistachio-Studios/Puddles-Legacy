#include "Log.h"

#include <stdio.h>
#include <cstdarg>

// #ifdef FULL_PATH
// void Log(const char* file, int line, const char* format, ...) {
// 	va_list args;
// 	va_start(args, format);

// 	printf("[%s:%d] ", file, line);
// 	vprintf(format, args);
// 	printf("\n");

// 	va_end(args);
// }
//#elif
#include <string>
void Log(const char* file, int line, const char* format, ...) {
	va_list args;
	va_start(args, format);

	std::string filePath(file);
	size_t lastSlash = filePath.find_last_of("/\\");
	if (lastSlash != std::string::npos) {
		filePath = filePath.substr(lastSlash + 1);
	}

	printf("[%s:%d] ", filePath.c_str(), line);
	vprintf(format, args);
	printf("\n");

	va_end(args);
}
//#endif