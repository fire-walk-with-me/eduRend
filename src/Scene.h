
#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "stdafx.h"
#include "InputHandler.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Cube.h"

// New files
// Material
// Texture <- stb

// TEMP


class Scene
{
protected:
	ID3D11Device*			dxdevice;
	ID3D11DeviceContext*	dxdevice_context;
	int						window_width;
	int						window_height;

public:

	Scene(
		ID3D11Device* dxdevice,
		ID3D11DeviceContext* dxdevice_context,
		int window_width,
		int window_height);

	virtual void Init() = 0;

	virtual void Update(
		float dt,
		InputHandler* input_handler) = 0;
	
	virtual void Render() = 0;
	
	virtual void Release() = 0;

	virtual void WindowResize(
		int window_width,
		int window_height);
};

class OurTestScene : public Scene
{
	//
	// Constant buffers (CBuffers) for data that is sent to shaders
	//

	// CBuffer for transformation matrices
	ID3D11Buffer* transformation_buffer = nullptr;
	// + other CBuffers
	ID3D11Buffer* cameraAndLight_buffer = nullptr;
	ID3D11Buffer* colorAndShininess_buffer = nullptr;
	

	// 
	// CBuffer client-side definitions
	// These must match the corresponding shader definitions 
	//

	struct TransformationBuffer
	{
		mat4f ModelToWorldMatrix;
		mat4f WorldToViewMatrix;
		mat4f ProjectionMatrix;
	};

	struct CameraAndLightBuffer 
	{
		vec3f cameraPosition;
		vec3f lightPosition;
	};

	struct ColorAndShininessBuffer 
	{


	};

	//
	// Scene content
	//
	Camera* camera;

	QuadModel* quad;
	OBJModel* sponza;
	OBJModel* me;
	OBJModel* sphere;
	OBJModel* childSphere1;
	OBJModel* childSphere2;
	OBJModel* childSphere3;
	Cube* cube;

	//Model-to-world transformation matrices
	mat4f Mcube;

	// Model-to-world transformation matrices
	mat4f Msponza;
	mat4f Mquad;

	mat4f Mme;

	mat4f Msphere;
	mat4f MchildSphere1;
	mat4f MchildSphere2;
	mat4f MchildSphere3;

	// World-to-view matrix
	mat4f Mview;
	// Projection matrix
	mat4f Mproj;

	// Misc
	float angle = 0;			// A per-frame updated rotation angle (radians)...
	float angle_vel = fPI / 2;	// ...and its velocity (radians/sec)
	float camera_vel = 5.0f;	// Camera movement velocity in units/s
	float fps_cooldown = 0;
	vec3f lightSource;

	void InitTransformationBuffer();
	void InitCameraAndLightBuffer();
	void InitColorAndShininessBuffer();

	void UpdateTransformationBuffer(mat4f ModelToWorldMatrix, mat4f WorldToViewMatrix, mat4f ProjectionMatrix);

	void UpdateCameraAndLightBuffer(vec3f cameraPosition, vec3f lightPosition);
	void UpdateColorAndShininessBuffer();

public:
	OurTestScene(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context, int window_width, int window_height);

	void Init() override;

	void Update(float dt, InputHandler* input_handler) override;

	void Render() override;

	void Release() override;

	void WindowResize(int window_width, int window_height) override;
};

#endif