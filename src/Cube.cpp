#include "Cube.h"

Cube::Cube(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : Model(dxdevice, dxdevice_context)
{
	//First
	v00.Pos = { -0.5f, -0.5f, 0.0f };
	v00.Normal = { 0, 0, 1 };
	v00.TexCoord = { 0, 0 };
	v01.Pos = { 0.5, -0.5f, 0.0f };
	v01.Normal = { 0, 0, 1 };
	v01.TexCoord = { 0, 1 };
	v02.Pos = { 0.5, 0.5f, 0.0f };
	v02.Normal = { 0, 0, 1 };
	v02.TexCoord = { 1, 1 };
	v03.Pos = { -0.5, 0.5f, 0.0f };
	v03.Normal = { 0, 0, 1 };
	v03.TexCoord = { 1, 0 };
	//Second
	v10.Pos = { 0.5f, 0.5f, 0.0f };
	v10.Normal = { 0, 0, 1 };
	v10.TexCoord = { 0, 0 };
	v11.Pos = { 0.5, 0.5f, 0.0f };
	v11.Normal = { 0, 0, 1 };
	v11.TexCoord = { 0, 1 };
	v12.Pos = { 0.5, 0.5f, 0.0f };
	v12.Normal = { 0, 0, 1 };
	v12.TexCoord = { 1, 1 };
	v13.Pos = { -0.5, 0.5f, 0.0f };
	v13.Normal = { 0, 0, 1 };
	v13.TexCoord = { 1, 0 };
	//Third
	v20.Pos = { -0.5f, -0.5f, 0.0f };
	v20.Normal = { 0, 0, 1 };
	v20.TexCoord = { 0, 0 };
	v21.Pos = { 0.5, -0.5f, 0.0f };
	v21.Normal = { 0, 0, 1 };
	v21.TexCoord = { 0, 1 };
	v22.Pos = { 0.5, 0.5f, 0.0f };
	v22.Normal = { 0, 0, 1 };
	v22.TexCoord = { 1, 1 };
	v23.Pos = { -0.5, 0.5f, 0.0f };
	v23.Normal = { 0, 0, 1 };
	v23.TexCoord = { 1, 0 };
	//Forth
	v30.Pos = { -0.5f, -0.5f, 0.0f };
	v30.Normal = { 0, 0, 1 };
	v30.TexCoord = { 0, 0 };
	v31.Pos = { 0.5, -0.5f, 0.0f };
	v31.Normal = { 0, 0, 1 };
	v31.TexCoord = { 0, 1 };
	v32.Pos = { 0.5, 0.5f, 0.0f };
	v32.Normal = { 0, 0, 1 };
	v32.TexCoord = { 1, 1 };
	v33.Pos = { -0.5, 0.5f, 0.0f };
	v33.Normal = { 0, 0, 1 };
	v33.TexCoord = { 1, 0 };

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

	// Populate the index array with two triangles
	// Triangle #1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	// Triangle #2
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// Triangle #3
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(7);
	// Triangle #4
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(7);

	// Tringale #5
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	// Triangle #6
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

	// Triangle #7
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	// Triangle #8
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);

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