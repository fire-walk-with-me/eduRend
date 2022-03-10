
//
//  Model.h
//
//  Carl Johan Gribel 2016, cjgribel@gmail.com
//

#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "stdafx.h"
#include <vector>
#include "vec\vec.h"
#include "vec\mat.h"
#include "ShaderBuffers.h"
#include "Drawcall.h"
#include "OBJLoader.h"
#include "Texture.h"

using namespace linalg;

class Model
{
protected:
	// Pointers to the current device and device context
	ID3D11Device* const			dxdevice;
	ID3D11DeviceContext* const	dxdevice_context;

	// Pointers to the class' vertex & index arrays
	ID3D11Buffer* vertex_buffer = nullptr;
	ID3D11Buffer* index_buffer = nullptr;
	ID3D11Buffer* colorAndShininess_buffer = nullptr;


public:

	Model(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context);

	Material defaultMaterial;
	bool skybox;

	//
	// Abstract render method: must be implemented by derived classes
	//
	virtual void Render() const = 0;

	void compute_TB(Vertex& v0, Vertex& v1, Vertex& v2);

	void InitColorAndShininessBuffer();
	void UpdateColorAndShininessBuffer(vec4f ambient, vec4f diffuse, vec4f specular, vec4f shininess) const;

	struct ColorAndShininessBuffer
	{
		vec4f ambient;
		vec4f diffuse;
		vec4f specular;
		vec4f shininess;
	};

	virtual ~Model()
	{
		SAFE_RELEASE(vertex_buffer);
		SAFE_RELEASE(index_buffer);
		SAFE_RELEASE(colorAndShininess_buffer);
	}
};

class QuadModel : public Model
{
	unsigned nbr_indices = 0;

public:

	QuadModel(ID3D11Device* dx3ddevice, ID3D11DeviceContext* dx3ddevice_context);

	virtual void Render() const;

	~QuadModel() { }
};

class OBJModel : public Model
{
	// index ranges, representing drawcalls, within an index array
	struct IndexRange
	{
		unsigned int start;
		unsigned int size;
		unsigned ofs;
		int mtl_index;
	};

	std::vector<IndexRange> index_ranges;
	std::vector<Material> materials;

	void append_materials(const std::vector<Material>& mtl_vec)
	{
		materials.insert(materials.end(), mtl_vec.begin(), mtl_vec.end());
	}


public:

	OBJModel* parent;
	mat4f transform;

	const char* cube_filename[6];


	mat4f translation;
	mat4f rotation;
	mat4f scaling;


	OBJModel(const std::string& objfile, OBJModel* parent, ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context, bool isSkybox);

	virtual void Render() const;

	mat4f getTransform();
	mat4f getParentTransform();
	mat4f getParentTranslation();
	mat4f getParentRoation();
	mat4f getParentScaling();

	void setTransform(mat4f translation, mat4f rotation, mat4f scaling);

	~OBJModel();
};

#endif