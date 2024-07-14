#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
  public:
    Transform();

    void setPosition(const glm::vec3& position);
    glm::vec3 getPosition() const;
    void setRotation(const double rotation);
    double getRotation() const;
    void setScale(const glm::vec2& scale);
    glm::vec2 getScale() const;

    glm::mat2x3 GetLocalMatrix();

  private:
    glm::vec3 position;
    double rotation;
    glm::vec2 scale;

    glm::mat2x3 localMatrix;
    bool dirty;

    void RecalculateMatrix();
};

#endif