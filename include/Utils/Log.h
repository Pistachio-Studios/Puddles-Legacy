#ifndef __LOG_H__
#define __LOG_H__

enum LogLevel {
    INFO,
    WARNING,
    ERROR
};

//With this you can choose between full-path printing or relative-path printing
#define FULL_PATH 1

#define LOG(format, ...) Log(INFO, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) Log(WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Log(ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)

void Log(LogLevel level, const char file[], int line, const char* format, ...);

#endif  // __LOG_H__