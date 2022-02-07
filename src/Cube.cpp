#include "Cube.h"
#include <fstream>
#include <algorithm>
#include "vec/vec.h"
#include "parseutil.h"

Cube::Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : Model(dxdevice, dxdevice_context)
{
	//First/Front
	v00.Pos = { 1, 1, 1 };
	v00.Normal = { 0, 0, 1 };
	v00.TexCoord = { 0, 0 };

	v01.Pos = { 1, -1, 1 };
	v01.Normal = { 0, 0, 1 };
	v01.TexCoord = { 0, 1 };

	v02.Pos = { -1, 1, 1 };
	v02.Normal = { 0, 0, 1 };
	v02.TexCoord = { 1, 1 };

	v03.Pos = { -1, -1, 1 };
	v03.Normal = { 0, 0, 1 };
	v03.TexCoord = { 1, 0 };
	//Second/Back
	v10.Pos = { 1, 1, -1 };
	v10.Normal = { 0, 0, 1 };
	v10.TexCoord = { 0, 0 };

	v11.Pos = { 1, -1, -1 };
	v11.Normal = { 0, 0, 1 };
	v11.TexCoord = { 0, 1 };

	v12.Pos = { -1, 1, -1 };
	v12.Normal = { 0, 0, 1 };
	v12.TexCoord = { 1, 1 };

	v13.Pos = { -1, -1, -1 };
	v13.Normal = { 0, 0, 1 };
	v13.TexCoord = { 1, 0 };
	//Third
	v20.Pos = { 1, 1, -1};
	v20.Normal = { 0, 0, 1 };
	v20.TexCoord = { 0, 0 };

	v21.Pos = { 1, -1, -1};
	v21.Normal = { 0, 0, 1 };
	v21.TexCoord = { 0, 1 };

	v22.Pos = {1, 1, 1};
	v22.Normal = { 0, 0, 1 };
	v22.TexCoord = { 1, 1 };

	v23.Pos = { 1, -1, 1 };
	v23.Normal = { 0, 0, 1 };
	v23.TexCoord = { 1, 0 };
	//Forth
	v30.Pos = { 1, 1, -1 };
	v30.Normal = { 0, 0, 1 };
	v30.TexCoord = { 0, 0 };

	v31.Pos = { -1, 1, -1 };
	v31.Normal = { 0, 0, 1 };
	v31.TexCoord = { 0, 1 };

	v32.Pos = { 1, 1, 1 };
	v32.Normal = { 0, 0, 1 };
	v32.TexCoord = { 1, 1 };

	v33.Pos = { -1, 1, 1 };
	v33.Normal = { 0, 0, 1 };
	v33.TexCoord = { 1, 0 };
	//Fith
	v40.Pos = { 1, -1, -1 };
	v40.Normal = { 0, 0, 1 };
	v40.TexCoord = { 0, 0 };

	v41.Pos = { -1, -1, -1 };
	v41.Normal = { 0, 0, 1 };
	v41.TexCoord = { 0, 1 };

	v42.Pos = { 1, -1, 1 };
	v42.Normal = { 0, 0, 1 };
	v42.TexCoord = { 1, 1 };

	v43.Pos = { -1, -1, 1 };
	v43.Normal = { 0, 0, 1 };
	v43.TexCoord = { 1, 0 };
	//Sixth
	v50.Pos = { -1, -1, 1 };
	v50.Normal = { 0, 0, 1 };
	v50.TexCoord = { 0, 0 };

	v51.Pos = { -1, 1, 1 };
	v51.Normal = { 0, 0, 1 };
	v51.TexCoord = { 0, 1 };

	v52.Pos = { -1, -1, -1 };
	v52.Normal = { 0, 0, 1 };
	v52.TexCoord = { 1, 1 };

	v53.Pos = { -1, 1, -1 };
	v53.Normal = { 0, 0, 1 };
	v53.TexCoord = { 1, 0 };


	//First
	vertices.push_back(v00);
	vertices.push_back(v01);
	vertices.push_back(v02);
	vertices.push_back(v03);
	//Second
	vertices.push_back(v10);
	vertices.push_back(v11);
	vertices.push_back(v12);
	vertices.push_back(v13);
	//Third
	vertices.push_back(v20);
	vertices.push_back(v21);
	vertices.push_back(v22);
	vertices.push_back(v23);
	//Forth
	vertices.push_back(v30);
	vertices.push_back(v31);
	vertices.push_back(v32);
	vertices.push_back(v33);
	//Fith
	vertices.push_back(v40);
	vertices.push_back(v41);
	vertices.push_back(v42);
	vertices.push_back(v43);
	//Sixth
	vertices.push_back(v50);
	vertices.push_back(v51);
	vertices.push_back(v52);
	vertices.push_back(v53);

	// Populate the index array with twelve triangles
	// Triangle #1
	indices.push_back(3);
	indices.push_back(1);
	indices.push_back(0);
	// Triangle #2
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);
	// Triangle #3
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(7);
	// Triangle #4
	indices.push_back(7);
	indices.push_back(6);
	indices.push_back(4);
	// Tringale #5
	indices.push_back(8);
	indices.push_back(10);
	indices.push_back(11);
	// Triangle #6
	indices.push_back(11);
	indices.push_back(9);
	indices.push_back(8);
	// Triangle #7
	indices.push_back(15);
	indices.push_back(14);
	indices.push_back(12);
	// Triangle #8
	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(15);
	// Triangle #9
	indices.push_back(16);
	indices.push_back(18);
	indices.push_back(19);
	// Triangle #10
	indices.push_back(19);
	indices.push_back(17);
	indices.push_back(16);
	// Triangle #11
	indices.push_back(23);
	indices.push_back(22);
	indices.push_back(20);
	// Triangle #12
	indices.push_back(20);
	indices.push_back(21);
	indices.push_back(23);

	// Vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0 };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = (UINT)(vertices.size() * sizeof(Vertex));

	// Data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &vertices[0];
	// Create vertex buffer on device using descriptor & data
	const HRESULT vhr = dxdevice->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);
	SETNAME(vertex_buffer, "VertexBuffer");

	//  Index array descriptor
	D3D11_BUFFER_DESC ibufferDesc = { 0 };
	ibufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibufferDesc.CPUAccessFlags = 0;
	ibufferDesc.Usage = D3D11_USAGE_DEFAULT;
	ibufferDesc.MiscFlags = 0;
	ibufferDesc.ByteWidth = (UINT)(indices.size() * sizeof(unsigned));
	// Data resource
	D3D11_SUBRESOURCE_DATA idata;
	idata.pSysMem = &indices[0];
	// Create index buffer on device using descriptor & data
	const HRESULT ihr = dxdevice->CreateBuffer(&ibufferDesc, &idata, &index_buffer);
	SETNAME(index_buffer, "IndexBuffer");

	nbr_indices = (unsigned int)indices.size();

	materials.push_back(defaultMaterial);

	// Copy materials from mesh
	append_materials(this->materials);

	for (auto& mtl : materials)
	{
		HRESULT hr;

		// Load Diffuse texture
		if (mtl.Kd_texture_filename.size()) {

			hr = LoadTextureFromFile(dxdevice, mtl.Kd_texture_filename.c_str(), &mtl.diffuse_texture);
			std::cout << "\t" << mtl.Kd_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}
	}

	dxdevice_context->PSSetShaderResources(0, 1, &mtl.diffuse_texture_SRV);
}



void Cube::Render() const
{
	// Bind our vertex buffer
	const UINT32 stride = sizeof(Vertex); //  sizeof(float) * 8;
	const UINT32 offset = 0;
	dxdevice_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// Bind our index buffer
	dxdevice_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);

	// Make the drawcall
	dxdevice_context->DrawIndexed(nbr_indices, 0, 0);
}

Cube::~Cube()
{

}