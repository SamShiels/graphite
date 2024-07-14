#ifndef RENDER_OBJECT_H
#define RENDER_OBJECT_H

#include <glm/glm.hpp>
#include <transform.h>

class RenderObject {
  public:
    RenderObject() : transform() {};
    Transform transform;
};

#endif