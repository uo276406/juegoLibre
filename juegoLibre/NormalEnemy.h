#pragma once
#include "Enemy.h"
class NormalEnemy :
    public Enemy
{
public: 
    NormalEnemy(float x, float y, Game* game);

    void update() override;

    void draw(float scrollX, float scrollY) override;

    Projectile* shoot() override;

    Animation* aDying;
    Animation* aMoving;
    Animation* animation;
};

