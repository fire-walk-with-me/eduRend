#pragma once
#include "Camera.h"


void Camera::rotate(InputHandler* input_handler)
{
	mousedx = input_handler->GetMouseDeltaX() / 1000;
	mousedy = input_handler->GetMouseDeltaY() / 1000;

	mat4f::rotation(0)
}

