#pragma once
#include "Enemy.h"
class JumpEnemy : public Enemy
{
public:
    JumpEnemy(float x, float y, Game* game);

    void update() override;

    void draw(float scrollX, float scrollY) override;

    void jump();

    int jumpInterval = 120;

    int orientation;
    Animation* aDyingRight;
    Animation* aDyingLeft;
    Animation* aMovingRight;
    Animation* aMovingLeft;


};

