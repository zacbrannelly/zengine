#include "FreeCamera.h"
#include "../Map/Objects/Entity.h"
#include "../Input/InputManager.h"
#include "../Component/Transform.h"

#include <iostream>

using namespace ZEngine;

FreeCamera::FreeCamera() : Camera()
{
  _speed = 0.1f;
  _sensitivity = 0.1f;
}

void FreeCamera::Init()
{
  _inputManager = InputManager::GetInstance();
}

void FreeCamera::Update()
{
  if (!_inputManager->IsMouseGrabbed()) return;

  auto owner = GetOwner();
  if (owner == nullptr) return;

  auto transform = GetOwner()->GetTransform();
  if (transform == nullptr) return;

  glm::quat rotationQuat = transform->GetRotationQuaternion();
  glm::vec2 delta = _inputManager->GetMouseDelta();

  if (delta.x != 0 || delta.y != 0)
  {
    glm::quat yaw = glm::angleAxis(glm::radians(delta.x * _sensitivity), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat pitch = glm::angleAxis(glm::radians(delta.y * _sensitivity), glm::vec3(1.0f, 0.0f, 0.0f));

    rotationQuat = yaw * rotationQuat * pitch;
    transform->SetRotationQuaternion(rotationQuat);
  }

  if (_inputManager->GetButtonDown(BUTTON_KEY_W))
  {
    transform->Translate(transform->GetForward() * _speed);
  }

  if (_inputManager->GetButtonDown(BUTTON_KEY_S))
  {
    transform->Translate(-transform->GetForward() * _speed);
  }

  if (_inputManager->GetButtonDown(BUTTON_KEY_A))
  {
    transform->Translate(-transform->GetRight() * _speed);
  }

  if (_inputManager->GetButtonDown(BUTTON_KEY_D))
  {
    transform->Translate(transform->GetRight() * _speed);
  }

  if (_inputManager->GetButtonDown(BUTTON_KEY_Q))
  {
    transform->Translate(transform->GetUp() * _speed);
  }

  if (_inputManager->GetButtonDown(BUTTON_KEY_E))
  {
    transform->Translate(-transform->GetUp() * _speed);
  }
}

FreeCamera::~FreeCamera()
{
}
