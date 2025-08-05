#pragma once

#include "singleton.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include <string>
#include <unordered_map>
#include "game.h"


// TupleIndex 结构体模板，递归查找类型 T 在类型包 Ts... 中的索引
template<typename T, typename... Ts>
struct TupleIndex;

// 基本情况：如果 T 是 Ts 中的第一个类型，则返回 0
template<typename T, typename... Ts>
struct TupleIndex<T, T, Ts...> {
    constexpr static std::size_t value = 0;
};

// 递归情况：如果 T 不是第一个类型，则递归查找后续类型，索引加 1
template<typename T, typename U, typename... Ts>
struct TupleIndex<T, U, Ts...> {
    constexpr static std::size_t value = 1 + TupleIndex<T, Ts...>::value;
};

// 如果找不到 T，返回 -1，表示 T 不在类型包 Ts... 中
template<typename T>
struct TupleIndex<T> {
    constexpr static std::size_t value = -1;
};

template<typename T, typename... Ts>
constexpr std::size_t TupleIndex_v = TupleIndex<T, Ts...>::value;





// 资源类型特征结构体，包含加载器和删除器
template<typename T>
struct ResourceTraits;

template<>
struct ResourceTraits<SDL_Texture> {
    static SDL_Texture* load(const std::string& filename)
    {
        return IMG_LoadTexture(G_GAME.renderer(), filename.c_str());
    }

    static void release(SDL_Texture* texture)
    {
        SDL_DestroyTexture(texture);
    }
};

template<>
struct ResourceTraits<Mix_Chunk> {
    static Mix_Chunk* load(const std::string& filename)
    {
        return Mix_LoadWAV(filename.c_str());
    }

    static void release(Mix_Chunk* chunk)
    {
        Mix_FreeChunk(chunk);
    }
};

// 资源管理器
template<typename... Ts>
class ResourceManager : public Singleton<ResourceManager<Ts...>>
{
public:
    ~ResourceManager() {
        std::apply([this](auto&&... resource_map)
        {
            (clearResources(resource_map), ...); // 展开调用
        }, resources_);
    }

    // 加载资源：只允许在资源类型列表中
    template<typename T>
    std::enable_if_t<TupleIndex_v<T, Ts...> != -1, T*>
    loadResource(const std::string& filename)
    {
        auto& map = std::get<TupleIndex_v<T, Ts...>>(resources_);
        auto it = map.find(filename);
        if (it != map.end())
        {
            return it->second;
        }
        LOG_INFO("Loading resource: %s", filename.c_str());
        T* resource = ResourceTraits<T>::load(filename);
        map[filename] = resource;
        return resource;
    }

private:
    // 清理资源
    template<typename T>
    void clearResources(std::unordered_map<std::string, T*>& resource_map)
    {
        for (auto& [filename, resource] : resource_map)
        {
            ResourceTraits<T>::release(resource);
            LOG_INFO("Destroying resource: %s", filename.c_str());
        }
    }

private:
    std::tuple<std::unordered_map<std::string, Ts*>...> resources_;
};

#define G_RESOURCE_MGR ResourceManager<SDL_Texture, Mix_Chunk>::Instance()