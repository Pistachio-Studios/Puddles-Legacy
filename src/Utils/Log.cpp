#include "Utils/Log.h"

#ifdef _MSC_VER

#include <windows.h>
#include <stdio.h>

void Log(LogLevel level, const char file[], int line, const char* format, ...)
{
	static char tmpString1[4096];
	static char tmpString2[4096];
	static va_list ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmpString1, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmpString2, 4096, "\n%s(%d) : %s", file, line, tmpString1);

	OutputDebugString(tmpString2);
}

#else

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
void Log(LogLevel level, const char* file, int line, const char* format, ...) {
    va_list args;
    va_start(args, format);

    std::string filePath(file);
    size_t lastSlash = filePath.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        filePath = filePath.substr(lastSlash + 1);
    }

    const char* color;
    switch (level) {
        case INFO:
            color = "\033[0m";  // Reset to default color
            break;
        case WARNING:
            color = "\033[33m";  // Yellow
            break;
        case ERROR:
            color = "\033[31m";  // Red
            break;
    }

    printf("%s[%s:%d] ", color, filePath.c_str(), line);
    vprintf(format, args);
    printf("\033[0m\n");  // Reset to default color

    va_end(args);
}

#endif