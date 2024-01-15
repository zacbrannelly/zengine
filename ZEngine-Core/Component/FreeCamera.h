#include "Camera.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

class InputManager;

class FreeCamera : public Camera
{
public:
  FreeCamera();
  ~FreeCamera();

  void Init() override;
  void Update() override;

  void SetSpeed(float speed);
  float GetSpeed() const;

  void SetSensitivity(float sensitivity);
  float GetSensitivity() const;

private:
  float _speed;
  float _sensitivity;
  InputManager* _inputManager;
  bool _enabled;
  glm::quat _originalRotation;
};
