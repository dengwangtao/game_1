#include "scene.h"
#include "object.h"
#include "player.h"
#include "tools.h"
#include "animation.h"

Scene::Scene()
{
    LOG_DEBUG("Scene created");
}

Scene::~Scene()
{
    for (auto it = objects_.begin(); it != objects_.end(); ++it)
    {
        it->second->onDestroy();
        delete it->second;
    }
    objects_.clear();
    players_.clear();

    for (auto it = animations_.begin(); it != animations_.end(); ++it)
    {
        delete *it;
    }
    animations_.clear();

    LOG_DEBUG("Scene destroyed");
}

s32 Scene::update(s64 now_ms)
{
    updateCollision();

    updateAnimations(now_ms);

    for (auto* obj : tobe_removed_objects_)
    {
        removeObject(obj);
    }
    tobe_removed_objects_.clear();

    for (auto* ani : tobe_removed_animations_)
    {
        removeAnimation(ani);
    }

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
        LOG_WARN("Object with guid: %lld already exists, removed it", guid);
    }

    LOG_INFO("Adding object with guid: %s", obj->DebugString().c_str());
    objects_[obj->guid()] = obj;

    if (obj->IsPlayer())
    {
        players_.insert(obj->guid());
    }
    

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

    obj->onDestroy();

    if (obj->IsPlayer())
    {
        players_.erase(obj->guid());
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
    obj->set_is_marked_for_removal(true);
    tobe_removed_objects_.insert(obj);
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

s32 Scene::updateCollision()
{
    for (auto it = objects_.begin(); it != objects_.end(); ++it)
    {
        auto obj = it->second;
        for (auto it2 = std::next(it); it2 != objects_.end(); ++it2)
        {
            auto obj2 = it2->second;
            if (obj->is_marked_for_removal() || obj2->is_marked_for_removal())
            {
                continue;
            }
            auto rect1 = obj->GetRect();
            auto rect2 = obj2->GetRect();
            if (Tools::is_rect_overlap(rect1, rect2))
            {
                obj->onCollision(obj2);
                obj2->onCollision(obj);
            }
        }
    }


    return 0;
}


s32 Scene::addAnimation(Animation* animation)
{
    if (animation == nullptr)
    {
        return -1;
    }

    animations_.insert(animation);
    LOG_INFO("Adding animation with name: %s", animation->DebugString().c_str());

    return 0;
}
s32 Scene::removeAnimation(Animation* animation)
{
    if (animation == nullptr)
    {
        return -1;
    }

    animations_.erase(animation);

    return 0;
}
s32 Scene::markRemoveAnimation(Animation *animation)
{
    if (animation == nullptr)
    {
        return -1;
    }
    tobe_removed_animations_.insert(animation);

    return 0;
}
s32 Scene::updateAnimations(s64 now_ms)
{

    for (auto* animation : animations_)
    {
        animation->update(now_ms);
    }

    return 0;
}