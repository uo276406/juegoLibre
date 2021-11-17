#pragma once
#include "Enemy.h"
class JumpEnemy : public Enemy
{
public:
    JumpEnemy(float x, float y, Game* game);

    void update() override;

    void draw(float scrollX, float scrollY) override;

    Animation* animation;
    Animation* aMoving;
    Animation* aDying;


};

