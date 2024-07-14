#ifndef SPRITE_H
#define SPRITE_H

#include <renderObject.h>

class Sprite : public RenderObject {
  public:
    Sprite();
    Sprite(float width, float height);

    float width;
    float height;
};

#endif