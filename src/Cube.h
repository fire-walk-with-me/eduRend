#pragma once
#include "Model.h"

#define ALLOWED_TEXTURE_SUFFIXES { "bmp", "jpg", "png", "tga", "gif" }

class Cube : public Model
{
	struct IndexRange
	{
		unsigned int start;
		unsigned int size;
		unsigned ofs;
		int mtl_index;
	};

	Material defaultMaterial;

	std::vector<IndexRange> index_ranges;
	std::vector<Material> materials;

	void append_materials(const std::vector<Material>& mtl_vec)
	{
		materials.insert(materials.end(), mtl_vec.begin(), mtl_vec.end());
	}

	unsigned nbr_indices = 0;
public:

	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	 Vertex v00, v01, v02, v03,
			v10, v11, v12, v13,
			v20, v21, v22, v23,
			v30, v31, v32, v33,
			v40, v41, v42, v43,
			v50, v51, v52, v53;

	void Render() const;

	Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context);

	~Cube();
};