#pragma once
#include "Camera.h"


void Camera::rotate(InputHandler* input_handler)
{
	mousedx = input_handler->GetMouseDeltaX() / 300;
	mousedy = input_handler->GetMouseDeltaY() / 300;

	yaw = yaw -= mousedx;
	pitch = pitch -= mousedy;

	if (yaw > PI * 2)
		yaw = 0;
	if (pitch > PI * 2)
		pitch = 0;

	rotation = mat4f::rotation(0, yaw, pitch);
}

void Camera::moveTo(const vec3f& p)
{
	position = p;
}

void Camera::move(const vec3f& v)
{	
	position += v * rotation.get_3x3().inverse();
}

mat4f Camera::get_ProjectionMatrix()
{
	return mat4f::projection(vfov, aspect, zNear, zFar);
}

mat4f Camera::get_WorldToViewMatrix()
{
	// Assuming a camera's position and rotation is defined by matrices T(p) and R,
	// the View-to-World transform is T(p)*R (for a first-person style camera).
	//
	// World-to-View then is the inverse of T(p)*R;
	// inverse(T(p)*R) = inverse(R)*inverse(T(p)) = transpose(R)*T(-p)
	// Since now there is no rotation, this matrix is simply T(-p)

	worldToView = mat4f::translation(position) * rotation;

	viewToWorld = worldToView.inverse();

	return  viewToWorld;
}

vec3f Camera::get_CameraPosition() 
{
	return position;
}