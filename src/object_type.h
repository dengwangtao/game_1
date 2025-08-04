#pragma once

enum ObjectType
{
    NONE,
    PLAYER,
    ENEMY,
    BULLET,
    ITEM,
    SHIELD,
};

constexpr const char* ObjectTypeNames[] = { "NONE", "PLAYER", "ENEMY", "BULLET", "ITEM", "SHIELD" };

class Player;
class Enemy;
class Bullet;
class Item;
class Shield;


template<typename T>
struct ObjectTraits
{
    constexpr static ObjectType obj_type = ObjectType::NONE;
};

template<>
struct ObjectTraits<Player>
{
    constexpr static ObjectType obj_type = ObjectType::PLAYER;
};

template<>
struct ObjectTraits<Enemy>
{
    constexpr static ObjectType obj_type = ObjectType::ENEMY;
};

template<>
struct ObjectTraits<Bullet>
{
    constexpr static ObjectType obj_type = ObjectType::BULLET;
};

template<>
struct ObjectTraits<Item>
{
    constexpr static ObjectType obj_type = ObjectType::ITEM;
};

template<>
struct ObjectTraits<Shield>
{
    constexpr static ObjectType obj_type = ObjectType::SHIELD;
};