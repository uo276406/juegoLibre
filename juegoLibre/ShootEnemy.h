#pragma once
#include "Enemy.h"
class ShootEnemy : public Enemy
{
public:
    ShootEnemy(float x, float y, Game* game);

    void update() override;

    void draw(float scrollX, float scrollY) override;

    Animation* animation;

    Projectile* shoot() override;

    int cadence;
};

