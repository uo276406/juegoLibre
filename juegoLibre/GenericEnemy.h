#pragma once
#include "Actor.h"
class GenericEnemy :
    public Actor
{

    void virtual draw() = 0; // Va a sobrescribir
    void virtual update() = 0;

    GenericEnemy(string filename, float x, float y, int width, int height, Game* game);

};

