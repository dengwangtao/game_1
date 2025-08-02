#pragma once
#ifndef SCENE_H
#define SCENE_H


#include "comm_def.h"
#include "object_type.h"

#include <SDL.h>
#include <unordered_map>

class Object;
class Scene
{
public:
	Scene();
	virtual ~Scene();

    virtual s32 init() = 0;
    virtual s32 update(s64 now_ms) = 0;
    virtual s32 render() = 0;
    virtual s32 clean() = 0;
    virtual s32 handleEvent(SDL_Event* event) = 0;


    template<typename T, typename... Args>
    T* addObject(Args&&... args);

    s32 addObject(Object* obj);
    s32 removeObject(Object* obj);
    Object* getObject(u64 guid);

protected:
    std::unordered_map<u64, Object*> objects_;
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

#endif // !SCENE_H