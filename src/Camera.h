
//
//  Camera.h
//
//	Basic camera class
//

#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "vec\vec.h"
#include "vec\mat.h"
#include "InputHandler.h"

using namespace linalg;

class Camera
{
public:
	// Aperture attributes
	float vfov, aspect;	
	
	// Clip planes in view space coordinates
	// Evrything outside of [zNear, zFar] is clipped away on the GPU side
	// zNear should be > 0
	// zFar should depend on the size of the scene
	// This range should be kept as tight as possibly to improve
	// numerical precision in the z-buffer
	float zNear, zFar;	
	float mousedx, mousedy;
	float yaw, pitch;
	mat4f rotation;
	mat4f worldToView, viewToWorld;
						
	vec3f position;

	Camera(
		float vfov,
		float aspect,
		float zNear,
		float zFar) :
		vfov(vfov), aspect(aspect), zNear(zNear), zFar(zFar)
	{
		position = {0.0f, 0.0f, 0.0f};
	}

	// Move to an absolute position
	void moveTo(const vec3f& p);

	void rotate(InputHandler* input_handler);

	// Move relatively
	void move(const vec3f& v);


	// Return World-to-View matrix for this camera
	mat4f get_WorldToViewMatrix();


	// Matrix transforming from View space to Clip space
	// In a performance sensitive situation this matrix should be precomputed
	// if possible
	mat4f get_ProjectionMatrix();
};

#endif