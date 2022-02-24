//
//  Model.cpp
//
//  CJ Gribel 2016, cjgribel@gmail.com
//

#include "Model.h"

Model::Model(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : dxdevice(dxdevice), dxdevice_context(dxdevice_context)
{
	InitColorAndShininessBuffer();
}

QuadModel::QuadModel(ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : Model(dxdevice, dxdevice_context)
{
	// Vertex and index arrays
	// Once their data is loaded to GPU buffers, they are not needed anymore
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;

	// Populate the vertex array with 4 vertices
	Vertex v0, v1, v2, v3;
	v0.Pos = { -0.5, -0.5f, 0.0f };
	v0.Normal = { 1, 1, 1 };
	v0.TexCoord = { 0, 0 };
	v1.Pos = { 0.5, -0.5f, 0.0f };
	v1.Normal = { 1, 1, 1 };
	v1.TexCoord = { 0, 1 };
	v2.Pos = { 0.5, 0.5f, 0.0f };
	v2.Normal = { 1, 1, 1 };
	v2.TexCoord = { 1, 1 };
	v3.Pos = { -0.5, 0.5f, 0.0f };
	v3.Normal = { 1, 1, 1 };
	v3.TexCoord = { 1, 0 };
	vertices.push_back(v0);
	vertices.push_back(v1);
	vertices.push_back(v2);
	vertices.push_back(v3);

	// Populate the index array with two triangles
	// Triangle #1
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(3);
	// Triangle #2
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


void QuadModel::Render() const
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

OBJModel::OBJModel(const std::string& objfile, OBJModel* parent, ID3D11Device* dxdevice, ID3D11DeviceContext* dxdevice_context) : Model(dxdevice, dxdevice_context)
{
	// Load the OBJ
	OBJLoader* mesh = new OBJLoader();
	mesh->Load(objfile);

	this->parent = parent;

	// Load and organize indices in ranges per drawcall (material)
	std::vector<unsigned> indices;
	unsigned int i_ofs = 0;

	for (auto& dc : mesh->drawcalls)
	{
		// Append the drawcall indices
		for (auto& tri : dc.tris)
			indices.insert(indices.end(), tri.vi, tri.vi + 3);

		// Create a range
		unsigned int i_size = (unsigned int)dc.tris.size() * 3;
		int mtl_index = dc.mtl_index > -1 ? dc.mtl_index : -1;
		index_ranges.push_back({ i_ofs, i_size, 0, mtl_index });

		i_ofs = (unsigned int)indices.size();
	}

	for (int i = 0; i < indices.size(); i += 3) 
	{
		compute_TB(mesh->vertices[indices[i + 0]], mesh->vertices[indices[i + 1]], mesh->vertices[indices[i + 2]]);
	}

	// Vertex array descriptor
	D3D11_BUFFER_DESC vbufferDesc = { 0 };
	vbufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbufferDesc.CPUAccessFlags = 0;
	vbufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vbufferDesc.MiscFlags = 0;
	vbufferDesc.ByteWidth = (UINT)(mesh->vertices.size() * sizeof(Vertex));
	// Data resource
	D3D11_SUBRESOURCE_DATA vdata;
	vdata.pSysMem = &(mesh->vertices)[0];
	// Create vertex buffer on device using descriptor & data
	HRESULT vhr = dxdevice->CreateBuffer(&vbufferDesc, &vdata, &vertex_buffer);
	SETNAME(vertex_buffer, "VertexBuffer");

	// Index array descriptor
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
	HRESULT ihr = dxdevice->CreateBuffer(&ibufferDesc, &idata, &index_buffer);
	SETNAME(index_buffer, "IndexBuffer");

	// Copy materials from mesh
	append_materials(mesh->materials);

	// Go through materials and load textures (if any) to device
	std::cout << "Loading textures..." << std::endl;
	for (auto& mtl : materials)
	{
		HRESULT hr;

		// Load Diffuse texture
		if (mtl.Kd_texture_filename.size()) 
		{
			hr = LoadTextureFromFile(dxdevice, dxdevice_context, mtl.Kd_texture_filename.c_str(), &mtl.diffuse_texture);
			std::cout << "\t" << mtl.Kd_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}

		// + other texture types here - see Material class
		if (mtl.normal_texture_filename.size())
		{
			hr = LoadTextureFromFile(dxdevice, dxdevice_context, mtl.normal_texture_filename.c_str(), &mtl.normal_texture);
			std::cout << "\t" << mtl.normal_texture_filename << (SUCCEEDED(hr) ? " - OK" : "- FAILED") << std::endl;
		}
	}
	std::cout << "Done." << std::endl;

	SAFE_DELETE(mesh);
}

void Model::InitColorAndShininessBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC MatrixBuffer_desc = { 0 };
	MatrixBuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBuffer_desc.ByteWidth = sizeof(ColorAndShininessBuffer);
	MatrixBuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBuffer_desc.MiscFlags = 0;
	MatrixBuffer_desc.StructureByteStride = 0;
	ASSERT(hr = dxdevice->CreateBuffer(&MatrixBuffer_desc, nullptr, &colorAndShininess_buffer));
}

void Model::UpdateColorAndShininessBuffer(vec4f ambient, vec4f diffuse, vec4f specular, vec4f shininess) const
{
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(colorAndShininess_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	ColorAndShininessBuffer* matrix_buffer_ = (ColorAndShininessBuffer*)resource.pData;
	//matrix_buffer_->materials = ambient + diffuse + specular * shininess;
	matrix_buffer_->ambient = ambient;
	matrix_buffer_->diffuse = diffuse;
	matrix_buffer_->specular = specular;
	matrix_buffer_->shininess = shininess;
	dxdevice_context->Unmap(colorAndShininess_buffer, 0);
}

mat4f OBJModel::getTransform() {
	return this->transform;
}

mat4f OBJModel::getParentTransform() {
	if (this->parent != nullptr)
		return this->parent->getTransform();
}

mat4f OBJModel::getParentTranslation() {
	return this->parent->translation;
}

mat4f OBJModel::getParentRoation() {
	return this->parent->rotation;
}

mat4f OBJModel::getParentScaling() {
	return this->parent->scaling;
}

void OBJModel::setTransform(mat4f translation, mat4f rotation, mat4f scaling) {
	this->translation = translation;
	this->rotation = rotation;
	this->scaling = scaling;
	this->transform = translation * rotation * scaling;
}

void OBJModel::Render() const
{
	// Bind vertex buffer
	const UINT32 stride = sizeof(Vertex);
	const UINT32 offset = 0;
	dxdevice_context->IASetVertexBuffers(0, 1, &vertex_buffer, &stride, &offset);

	// Bind index buffer
	dxdevice_context->IASetIndexBuffer(index_buffer, DXGI_FORMAT_R32_UINT, 0);
	dxdevice_context->PSSetConstantBuffers(1, 1, &colorAndShininess_buffer);

	// Iterate drawcalls
	for (auto& irange : index_ranges)
	{
		// Fetch material
		const Material& mtl = materials[irange.mtl_index];

		// Bind diffuse texture to slot t0 of the PS
		dxdevice_context->PSSetShaderResources(0, 1, &mtl.diffuse_texture.texture_SRV);
		// + bind other textures here, e.g. a normal map, to appropriate slots
		dxdevice_context->PSSetShaderResources(1, 1, &mtl.normal_texture.texture_SRV);

		UpdateColorAndShininessBuffer(mtl.Ka.xyz1(), mtl.Kd.xyz1(), mtl.Ks.xyz1(), vec4f(0, 0, 0, 0.5f));

		// Make the drawcall
		dxdevice_context->DrawIndexed(irange.size, irange.start, 0);
	}

	/*for (Material material : materials)
	{
		UpdateColorAndShininessBuffer(material.Ka.xyz1(),  material.Kd.xyz1(),  material.Ks.xyz1(), vec4f(0,0,0, 0.5f));
	}*/
}

void Model::compute_TB(Vertex& v0, Vertex& v1, Vertex& v2)
{
	vec3f tangent, binormal;
	vec3f D, E;
	vec2f F, G;

	D = v1.Pos - v0.Pos;
	E = v2.Pos - v0.Pos;

	F = v1.TexCoord - v0.TexCoord;
	G = v2.TexCoord - v0.TexCoord;

	float partOne = 1.0f / (F.x * G.y - F.y * G.x);

	tangent.x = partOne * (G.y * D.x - F.y * E.x);
	tangent.y = partOne * (G.y * D.y - F.y * E.y);
	tangent.z = partOne * (G.y * D.z - F.y * E.z);

	binormal.x = partOne * (-G.x * D.x + F.x * E.x);
	binormal.y = partOne * (-G.x * D.y + F.x * E.y);
	binormal.z = partOne * (-G.x * D.z + F.x * E.z);

	v0.Tangent = v1.Tangent = v2.Tangent = tangent;
	v0.Binormal = v1.Binormal = v2.Binormal = binormal;
}

OBJModel::~OBJModel()
{
	for (auto& material : materials)
	{
		SAFE_RELEASE(material.diffuse_texture.texture_SRV);

		// Release other used textures ...
	}
}