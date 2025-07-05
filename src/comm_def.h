#pragma once

#include <SDL_log.h>


using u64 = unsigned long long;
using s64 = signed long long;
using u32 = unsigned int;
using s32 = signed int;
using u16 = unsigned short;
using s16 = signed short;
using u8 = unsigned char;
using s8 = signed char;


// # log相关宏定义
#define LOG_DEBUG(fmt, ...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, fmt, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, fmt, ##__VA_ARGS__)