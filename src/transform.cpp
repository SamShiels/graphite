#include <renderObject.h>
#include <transform.h>

Transform::Transform() {
  position = glm::vec3(0.0);
  rotation = 0.0;
  scale = glm::vec2(0.0);

  this->dirty = true;
}

void Transform::setPosition(const glm::vec3& position) {
  this->position = position;
  this->dirty = true;
}

glm::vec3 Transform::getPosition() const {
  return this->position;
}

void Transform::setRotation(const double rotation) {
  this->rotation = rotation;
  this->dirty = true;
}

double Transform::getRotation() const {
  return this->rotation;
}

void Transform::setScale(const glm::vec2& scale) {
  this->scale = scale;
  this->dirty = true;
}

glm::vec2 Transform::getScale() const {
  return this->scale;
}

glm::mat2x3 Transform::GetLocalMatrix() {
  if (this->dirty) {
    this->RecalculateMatrix();
    this->dirty = false;
  }

  return localMatrix;
}

void Transform::RecalculateMatrix() {
  glm::mat3 localMatrix = glm::mat3(1.0);

  localMatrix[2][0] = position.x;
  localMatrix[2][1] = position.y;

  glm::mat3 rotationMatrix = glm::mat3(1.0);
  float angle = glm::radians(rotation);
  float cos = std::cos(angle);
  float sin = std::sin(angle);

  rotationMatrix[0][0] =  cos;
  rotationMatrix[0][1] = -sin;
  rotationMatrix[1][0] =  sin;
  rotationMatrix[1][1] =  cos;

  localMatrix = rotationMatrix * localMatrix;

  glm::mat3 scaleMatrix = glm::mat3(1.0);
  localMatrix = scaleMatrix * localMatrix;

  this->localMatrix = localMatrix;
}