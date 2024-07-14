#include <renderObject.h>
#include <sprite.h>

Sprite::Sprite() : RenderObject() {
  this->width = 0;
  this->height = 0;
}

Sprite::Sprite(float width, float height) : RenderObject() {
  this->width = width;
  this->height = height;
}
