#pragma once
#include "Model.h"

class Cube : public Model
{
	unsigned nbr_indices = 0;
public:
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	Vertex v00, v01, v02, v03, 
		   v10, v11, v12, v13,
		   v20, v21, v22, v23,
		   v30, v31, v32, v33;

	void Render() const;

	Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context);

	~Cube();
};