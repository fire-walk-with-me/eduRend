
#include "Scene.h"

Scene::Scene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	dxdevice(dxdevice),
	dxdevice_context(dxdevice_context),
	window_width(window_width),
	window_height(window_height)
{ }

void Scene::WindowResize(
	int window_width,
	int window_height)
{
	this->window_width = window_width;
	this->window_height = window_height;
}

OurTestScene::OurTestScene(
	ID3D11Device* dxdevice,
	ID3D11DeviceContext* dxdevice_context,
	int window_width,
	int window_height) :
	Scene(dxdevice, dxdevice_context, window_width, window_height)
{ 
	InitTransformationBuffer();
	// + init other CBuffers
}

//
// Called once at initialization
//
void OurTestScene::Init()
{
	camera = new Camera(
		45.0f * fTO_RAD,		// field-of-view (radians)
		(float)window_width / window_height,	// aspect ratio
		1.0f,					// z-near plane (everything closer will be clipped/removed)
		500.0f);				// z-far plane (everything further will be clipped/removed)

	// Move camera to (0,0,5)
	camera->moveTo({ 0, 0, 5 });

	// Create objects
	quad = new QuadModel(dxdevice, dxdevice_context);
	cube = new Cube(dxdevice, dxdevice_context);
	sponza = new OBJModel("assets/crytek-sponza/sponza.obj", nullptr, dxdevice, dxdevice_context);
	me = new OBJModel("assets/me/meModel.obj", nullptr, dxdevice, dxdevice_context);
	sphere = new OBJModel("assets/sphere/sphere.obj", nullptr, dxdevice, dxdevice_context);
	childSphere1 = new OBJModel("assets/sphere/sphere.obj", sphere, dxdevice, dxdevice_context);
	childSphere2 = new OBJModel("assets/sphere/sphere.obj", sphere, dxdevice, dxdevice_context);
	childSphere3 = new OBJModel("assets/sphere/sphere.obj", childSphere2, dxdevice, dxdevice_context);
}

//
// Called every frame
// dt (seconds) is time elapsed since the previous frame
//
void OurTestScene::Update(
	float dt,
	InputHandler* input_handler)
{
	// Basic camera control
	if (input_handler->IsKeyPressed(Keys::W))
		camera->move({ 0.0f, 0.0f, -camera_vel * dt });
	if (input_handler->IsKeyPressed(Keys::S))
		camera->move({ 0.0f, 0.0f, camera_vel * dt });
	if (input_handler->IsKeyPressed(Keys::D))
		camera->move({ camera_vel * dt, 0.0f, 0.0f });
	if (input_handler->IsKeyPressed(Keys::A))
		camera->move({ -camera_vel * dt, 0.0f, 0.0f });
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		camera->rotate(input_handler);

	// Now set/update object transformations
	// This can be done using any sequence of transformation matrices,
	// but the T*R*S order is most common; i.e. scale, then rotate, and then translate.
	// If no transformation is desired, an identity matrix can be obtained 
	// via e.g. Mquad = linalg::mat4f_identity; 

	// Quad model-to-world transformation
	Mquad = mat4f::translation(0, 0, 0) *				// No translation
			mat4f::rotation(-angle, 0.0f, 1.0f, 0.0f) *	// Rotate continuously around the y-axis
			mat4f::scaling(1.5, 1.5, 1.5);				// Scale uniformly to 150%

	Mcube = mat4f::translation(-2, -2, 5) *				// little bit of translation
			mat4f::rotation(angle, 0.0f, 1.0f, 0.0f) *	// Rotate continuously around the y-axis
			mat4f::scaling(1, 1, 1);					// Scale uniformly to 100%

	// Sponza model-to-world transformation
	Msponza = mat4f::translation(0, -5, 0) *				 // Move down 5 units
		      mat4f::rotation(fPI / 2, 0.0f, 1.0f, 0.0f) *   // Rotate pi/2 radians (90 degrees) around y
			  mat4f::scaling(0.05f);						 // The scene is quite large so scale it down to 5%

	Mme = mat4f::translation(-1.7f, 0, -30) *					// Move down 5 units
		  mat4f::rotation(angle / 2, 0.0f, 1.0f, 0.0f) *		// Rotate pi/2 radians (90 degrees) around y
		  mat4f::scaling(2,2,2);							 

	//Msphere = mat4f::translation(-1.7f, 0, -15) *					// Move down 5 units
	//		  mat4f::rotation(angle / 2, 0.0f, 1.0f, 0.0f) *		// Rotate pi/2 radians (90 degrees) around y
	//		  mat4f::scaling(1, 1, 1);

	sphere->setTransform(mat4f::translation(-1.7f, 0, -15), mat4f::rotation(angle / 2, 0.0f, 1.0f, 0.0f), mat4f::scaling(1, 1, 1));

	childSphere1->setTransform(childSphere1->getParentTransform() * mat4f::translation(-1.7f, 0, -5), mat4f::rotation(angle * 2, 0.0f, 1.0f, 0.0f), mat4f::scaling(0.3f, 0.3f, 0.3f));
	childSphere2->setTransform(childSphere2->getParentTransform() * mat4f::translation(1.7f, 0, -6), mat4f::rotation(angle, 0.0f, 1.0f, 0.0f), mat4f::scaling(0.6f, 0.6f, 0.6f));
	childSphere3->setTransform(childSphere3->getParentTransform() * mat4f::translation(-1.7f, 0, -5), mat4f::rotation(-angle / 2, 0.0f, 1.0f, 0.0f), mat4f::scaling(0.2f, 0.2f, 0.2f));

	// Increment the rotation angle.
	angle += angle_vel * dt;

	// Print fps
	fps_cooldown -= dt;
	if (fps_cooldown < 0.0)
	{
		std::cout << "fps " << (int)(1.0f / dt) << std::endl;
		//printf("fps %i\n", (int)(1.0f / dt));
		fps_cooldown = 2.0;
	}
}

//
// Called every frame, after update
//
void OurTestScene::Render()
{
	// Bind transformation_buffer to slot b0 of the VS
	dxdevice_context->VSSetConstantBuffers(0, 1, &transformation_buffer);

	// Obtain the matrices needed for rendering from the camera
	Mview = camera->get_WorldToViewMatrix();
	Mproj = camera->get_ProjectionMatrix();

	// Load matrices + the Quad's transformation to the device and render it
	UpdateTransformationBuffer(Mquad, Mview, Mproj);
	//quad->Render();

	// Load matrices + the Quad's transformation to the device and render it
	UpdateTransformationBuffer(Mcube, Mview, Mproj);
	cube->Render();

	UpdateTransformationBuffer(Mme, Mview, Mproj);
	me->Render();

	UpdateTransformationBuffer(sphere->getTransform(), Mview, Mproj);
	sphere->Render();

	UpdateTransformationBuffer(childSphere1->getTransform(), Mview, Mproj);
	childSphere1->Render();

	UpdateTransformationBuffer(childSphere2->getTransform(), Mview, Mproj);
	childSphere2->Render();

	UpdateTransformationBuffer(childSphere3->getTransform(), Mview, Mproj);
	childSphere3->Render();

	// Load matrices + Sponza's transformation to the device and render it
	UpdateTransformationBuffer(Msponza, Mview, Mproj);
	sponza->Render();
}

void OurTestScene::Release()
{
	SAFE_DELETE(quad);
	SAFE_DELETE(sponza);
	SAFE_DELETE(camera);

	SAFE_RELEASE(transformation_buffer);
	// + release other CBuffers
}

void OurTestScene::WindowResize(
	int window_width,
	int window_height)
{
	if (camera)
		camera->aspect = float(window_width) / window_height;

	Scene::WindowResize(window_width, window_height);
}

void OurTestScene::InitTransformationBuffer()
{
	HRESULT hr;
	D3D11_BUFFER_DESC MatrixBuffer_desc = { 0 };
	MatrixBuffer_desc.Usage = D3D11_USAGE_DYNAMIC;
	MatrixBuffer_desc.ByteWidth = sizeof(TransformationBuffer);
	MatrixBuffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	MatrixBuffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	MatrixBuffer_desc.MiscFlags = 0;
	MatrixBuffer_desc.StructureByteStride = 0;
	ASSERT(hr = dxdevice->CreateBuffer(&MatrixBuffer_desc, nullptr, &transformation_buffer));
}

void OurTestScene::UpdateTransformationBuffer(
	mat4f ModelToWorldMatrix,
	mat4f WorldToViewMatrix,
	mat4f ProjectionMatrix)
{
	// Map the resource buffer, obtain a pointer and then write our matrices to it
	D3D11_MAPPED_SUBRESOURCE resource;
	dxdevice_context->Map(transformation_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	TransformationBuffer* matrix_buffer_ = (TransformationBuffer*)resource.pData;
	matrix_buffer_->ModelToWorldMatrix = ModelToWorldMatrix;
	matrix_buffer_->WorldToViewMatrix = WorldToViewMatrix;
	matrix_buffer_->ProjectionMatrix = ProjectionMatrix;
	dxdevice_context->Unmap(transformation_buffer, 0);
}