#include "scene.h"
#include "object.h"

Scene::Scene()
{
    LOG_DEBUG("Scene created");
}

Scene::~Scene()
{
    for (auto it = objects_.begin(); it != objects_.end(); ++it)
    {
        it->second->clean();
        delete it->second;
    }
    objects_.clear();

    LOG_DEBUG("Scene destroyed");
}

s32 Scene::addObject(Object *obj)
{
    if (obj == nullptr)
    {
        return -1;
    }
    objects_[obj->guid()] = obj;

    return 0;
}

s32 Scene::removeObject(Object* obj)
{
    if (obj == nullptr)
    {
        return -1;
    }
    auto it = objects_.find(obj->guid());
    if (it == objects_.end())
    {
        return -2;
    }
    delete it->second;
    objects_.erase(it);
    return 0;
}

Object* Scene::getObject(u64 guid)
{
    auto it = objects_.find(guid);
    if (it == objects_.end())
    {
        return nullptr;
    }
    return it->second;
}
