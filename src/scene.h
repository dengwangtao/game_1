#pragma once
#ifndef SCENE_H
#define SCENE_H


#include "comm_def.h"
#include "object_type.h"
#include "player.h"

#include <SDL.h>
#include <unordered_map>
#include <unordered_set>
#include <functional>

class Object;
class Player;
class Scene
{
public:
	Scene();
	virtual ~Scene();

    virtual s32 init() = 0;
    virtual s32 update(s64 now_ms);
    virtual s32 render() = 0;
    virtual s32 clean() = 0;
    virtual s32 handleEvent(SDL_Event* event) = 0;


    template<typename T, typename... Args>
    T* addObject(Args&&... args);

    s32 addObject(Object* obj);
    s32 removeObject(Object* obj);
    s32 markRemoveObject(Object* obj);
    Object* getObject(u64 guid);

    // 碰撞检测
    s32 updateCollision();

    template<typename T>
    s32 foreachObject(const std::function<s32(T&)> func);


protected:
    std::unordered_map<u64, Object*> objects_;
    std::unordered_set<u64> players_;
    std::vector<Object*> tobe_removed_objects_;
};



template<typename T, typename... Args>
T* Scene::addObject(Args&&... args)
{
    static_assert(std::is_base_of<Object, T>::value, "T must be derived from Object");

    // 获取类型
    auto obj_type = ObjectTraits<T>::obj_type;

    T* obj = new T(obj_type, std::forward<Args>(args)...);
    addObject(obj);
    return obj;
}


template<typename T>
s32 Scene::foreachObject(const std::function<s32(T&)> func)
{
    if constexpr (std::is_same_v<T, Player>)
    {
        for (auto guid : players_)
        {
            auto* obj = getObject(guid);
            if (obj != nullptr)
            {
                Player* player = static_cast<Player*>(obj);
                if (func(*player))
                {
                    break;
                }
            }
        }
    }
    else
    {
        for (auto& [_, obj] : objects_)
        {
            if (dynamic_cast<T*>(obj) != nullptr)
            {
                if (func(obj))
                {
                    break;
                }
            }
        }
    }
    return 0;
}

#endif // !SCENE_H