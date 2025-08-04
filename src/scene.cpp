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

s32 Scene::update(s64 now_ms)
{
    for (auto* obj : tobe_removed_objects_)
    {
        removeObject(obj);
    }
    tobe_removed_objects_.clear();
    return 0;
}

s32 Scene::addObject(Object *obj)
{
    if (obj == nullptr)
    {
        return -1;
    }

    u64 guid = obj->guid();
    if (objects_.find(guid) != objects_.end())
    {
        delete objects_[guid];
        objects_.erase(guid);
        LOG_WARN("Object with guid: %s already exists, removed it", guid);
    }

    LOG_INFO("Adding object with guid: %s", obj->DebugString().c_str());
    objects_[obj->guid()] = obj;

    return 0;
}

s32 Scene::removeObject(Object* obj)
{
    if (obj == nullptr)
    {
        return -1;
    }
    LOG_INFO("Removing object with guid: %s", obj->DebugString().c_str());
    auto it = objects_.find(obj->guid());
    if (it == objects_.end())
    {
        return -2;
    }
    delete it->second;
    objects_.erase(it);
    return 0;
}

s32 Scene::markRemoveObject(Object *obj)
{
    if (obj == nullptr)
    {
        return -1;
    }
    tobe_removed_objects_.push_back(obj);
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
