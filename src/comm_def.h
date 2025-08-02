#pragma once

#include <SDL_log.h>
#include <cstring>

using u64 = unsigned long long;
using s64 = signed long long;
using u32 = unsigned int;
using s32 = signed int;
using f32 = float;
using f64 = double;
using u16 = unsigned short;
using s16 = signed short;
using u8 = unsigned char;
using s8 = signed char;


#if defined(_WIN32)
    #define __SHORT_FILE__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
    #define __SHORT_FILE__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

// # log相关宏定义
#define LOG_DEBUG(fmt, ...) SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "%s:%d " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...) SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "%s:%d " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...) SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "%s:%d " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s:%d " fmt, __SHORT_FILE__, __LINE__, ##__VA_ARGS__)



#define DEF_Property_default(type, name, default_value) \
public: \
    type name() const { return name##_; } \
    type* mutable_##name() { return &name##_; } \
    void set_##name(type value) { name##_ = value; } \
protected: \
    type name##_ = default_value;

#define DEF_Property(type, name) \
    DEF_Property_default(type, name, {})