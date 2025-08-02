#pragma once

enum ObjectType
{
    NONE,
    PLAYER,
    ENEMY,
    BULLET,
};

constexpr const char* ObjectTypeNames[] = { "NONE", "PLAYER", "ENEMY", "BULLET" };

class Player;
class Enemy;
class Bullet;


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
