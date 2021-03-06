#include <iostream>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\glfw3.h>
#include <GLFW\glfw3native.h>

#define GLM_FORCE_LEFT_HANDED
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include <bgfx\bgfx.h>
#include <bgfx\platform.h>
#include "../../bgfx/examples/common/imgui/imgui.h"
#include <dear-imgui\imgui_impl_glfw.h>
#include <bx\math.h>

#include <ZEngine-Core/Component/Transform.h>
#include <ZEngine-Core/Rendering/MeshFactory.h>
#include <ZEngine-Core/Rendering/Mesh.h>
#include <ZEngine-Core/Rendering/Material.h>
#include <ZEngine-Core/Rendering/Shader.h>

#include "TransformInspector.h"

#include <fstream>

void error_handle(int errorCode, const char* message)
{
	std::cout << "Code: " << errorCode << std::endl;
	std::cout << "Message: " << message << std::endl;
	system("PAUSE");
}

uint32_t createRGBA(int r, int g, int b, int a)
{
	return ((r & 0xff) << 24) + ((g & 0xff) << 16) + ((b & 0xff) << 8) + (a & 0xff);
}

int main_nope(int argc, char* argv[])
{
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
	}

	glfwSetErrorCallback(&error_handle);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow* window = glfwCreateWindow(1024, 600, "Hello World", NULL, NULL);

	if (!window)
	{
		std::cout << "Failed to create a window" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Set the GLFW window to bgfx
	bgfx::PlatformData pd;
	pd.backBuffer = NULL;
	pd.backBufferDS = NULL;
	pd.context = NULL;
	pd.nwh = glfwGetWin32Window(window);
	pd.ndt = NULL;
	bgfx::setPlatformData(pd);

	auto r = bgfx::Resolution();
	r.width = 1024;
	r.height = 600;

	auto i = bgfx::Init();
	i.resolution = r;
	i.type = bgfx::RendererType::OpenGL;

	bgfx::init(i);
	bgfx::setDebug(BGFX_DEBUG_TEXT);

	imguiCreate();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui::StyleColorsDark();

	bgfx::VertexDecl decl;
	decl.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Float)
	.end();

	struct vert
	{
		glm::vec3 pos;
		float r, g, b, a;
	};

	vert vertices[] =
	{
		/*{ {0.0f, 0.0f, -1.0f} , 0xff00ff00 }, // bottom left
		{ {0.5f, 0.0f, -1.0f}, 0xff00ff00 }, // bottom right
		{ {0.5f, 0.5f, -1.0f}, 0xff00ff00 }, // top right
		{ {0.0f, 0.5f, -1.0f}, 0xff00ff00 } // top left*/

		{ { -1.0f,  1.0f, 0 }, 0, 1, 0, 1 }, // top left
		{ {  1.0f,  1.0f, 0 }, 0, 1, 0, 1 }, // top right
		{ { -1.0f, -1.0f, 0 }, 0, 1, 0, 1 }, // bottom left
		{ {  1.0f, -1.0f, 0 }, 0, 1, 0, 1 } // bottom right
	};

	uint16_t indices[] = 
	{
		0, 2, 1,
		2, 3, 1
	};

	auto vertexBufferMem = bgfx::copy(&vertices, sizeof(vertices));
	auto indexBufferMem = bgfx::copy(&indices, sizeof(indices));

	auto vertexBuffer = bgfx::createVertexBuffer(vertexBufferMem, decl);
	auto indexBuffer = bgfx::createIndexBuffer(indexBufferMem);

	vert otherVertices[] =
	{
		/*{ {0.0f, 0.0f, -1.0f} , 0xff00ff00 }, // bottom left
		{ {0.5f, 0.0f, -1.0f}, 0xff00ff00 }, // bottom right
		{ {0.5f, 0.5f, -1.0f}, 0xff00ff00 }, // top right
		{ {0.0f, 0.5f, -1.0f}, 0xff00ff00 } // top left*/

		{ { -1.0f,  1.0f, 0 }, 0, 0, 0, 1 }, // top left
		{ {  1.0f,  1.0f, 0 }, 0, 0, 0, 1 }, // top right
		{ { -1.0f, -1.0f, 0 }, 0, 0, 0, 1 }, // bottom left
		{ {  1.0f, -1.0f, 0 }, 0, 0, 0, 1 } // bottom right
	};

	vertexBufferMem = bgfx::copy(&otherVertices, sizeof(otherVertices));
	auto vertexBuffer2 = bgfx::createVertexBuffer(vertexBufferMem, decl);

	bgfx::ShaderHandle vertexShader;
	bgfx::ShaderHandle fragShader;
	
	std::ifstream in("vs_cubes.bin", std::ifstream::in | std::ifstream::binary | std::ifstream::ate);
	
	if (in.is_open())
	{
		std::ifstream::pos_type size = in.tellg();
		in.seekg(0, std::ios::beg);

		unsigned int allocSize = (unsigned int)size + 1;

		auto buffer = bgfx::alloc(allocSize);
		in.read((char*)buffer->data, size);

		// Null terminate
		buffer->data[allocSize - 1] = '\0';

		vertexShader = bgfx::createShader(buffer);
	}

	in.close();

	in = std::ifstream("fs_cubes.bin", std::ios::in | std::ios::binary | std::ios::ate);

	if (in.is_open())
	{
		std::ifstream::pos_type size = in.tellg();
		in.seekg(0, std::ios::beg);

		unsigned int allocSize = (unsigned int)size + 1;

		auto buffer = bgfx::alloc(allocSize);
		in.read((char*)buffer->data, size);

		// Null terminate
		buffer->data[allocSize - 1] = '\0';

		fragShader = bgfx::createShader(buffer);
	}

	in.close();

	bgfx::ProgramHandle program = bgfx::createProgram(vertexShader, fragShader, true);

	glm::vec2 position(0, 0);

	auto mesh = MeshFactory::CreateCube("derp");
	auto shader = new Shader("derpshader");
	shader->Load("vs_cubes.bin", "fs_cubes.bin");

	auto material = new Material("derpsdsd");
	material->SetShader(shader);

	auto mesh2 = MeshFactory::CreateCube("derp2");
	mesh2->SetColors(std::vector<glm::vec4>(mesh2->GetVertices().size(), { 1, 0, 0, 1 }));

	Transform t, t2;

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		int displayW, displayH;
		glfwGetFramebufferSize(window, &displayW, &displayH);

		float cameraSize = 5.0f;
		
		// These should be done in a component view and/or windows
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Create the menu bar & window using IMGUI
			{
				ImGui::BeginMainMenuBar();

				if (ImGui::BeginMenu("File"))
				{
					ImGui::MenuItem("New Map", NULL, (bool*)NULL);
					ImGui::MenuItem("Open Map", NULL, (bool*)NULL);
					ImGui::MenuItem("Save Map", NULL, (bool*)NULL);
					ImGui::MenuItem("Close", NULL, (bool*)NULL);

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Edit"))
				{
					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Window"))
				{

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("About"))
				{
					ImGui::EndMenu();
				}

				ImGui::EndMainMenuBar();
			}

			// Example winodw
			{
				ImGui::Begin("Derp");

				ImGui::SliderFloat("Camera Size", &cameraSize, 0, 100);
				ImGui::InputFloat2("Pos", &position[0]);

				ImGui::End();
			}

			// Not sure where this should go
			bgfx::touch(0);

			// These should be done by a camera
			bgfx::setViewMode(0, bgfx::ViewMode::Default);
			bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, createRGBA(200, 255, 0, 255));
			bgfx::setViewRect(0, 0, 0, displayW, displayH);

			{
				float aspectRatio = (float)displayW / displayH;
				//auto projMatrix = glm::ortho<float>(-aspectRatio * cameraSize, aspectRatio * cameraSize, -cameraSize, cameraSize, 0.1f, 100.0f);
				auto projMatrix = glm::perspective<float>(glm::radians(60.0f), aspectRatio, 0.1, 100);
				auto viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 10));

				bgfx::setViewTransform(0, &viewMatrix[0][0], &projMatrix[0][0]);
			}

			// This should be done by a renderer (MeshRenderer)
			{
				auto transform = glm::mat4(1.0f);

				bgfx::setTransform(&transform[0][0]);

				bgfx::setVertexBuffer(0, vertexBuffer);
				bgfx::setIndexBuffer(indexBuffer);

				bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);

				bgfx::submit(0, program);
			}

			// This should be done by a renderer (MeshRenderer)
			{
				TransformInspector inspector;
				inspector.Inspect(&t);
				inspector.RenderElement();


				auto transform = glm::mat4(1.0f);
				transform = t.GetLocalTransformMatrix();

				auto transform2 = t2.GetLocalTransformMatrix();

				/*bgfx::setTransform(&transform[0][0]);

				bgfx::setVertexBuffer(0, vertexBuffer2);
				bgfx::setIndexBuffer(indexBuffer);

				bgfx::setState(BGFX_STATE_DEFAULT | BGFX_STATE_PT_TRISTRIP);

				bgfx::submit(0, program);*/

				// These should be done by a camera
				bgfx::setViewMode(1, bgfx::ViewMode::Default);
				bgfx::setViewClear(1, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, createRGBA(200, 255, 0, 255));
				bgfx::setViewRect(1, 0, 0, displayW, displayH);

				{
					float aspectRatio = (float)displayW / displayH;
					//auto projMatrix = glm::ortho<float>(-aspectRatio * cameraSize, aspectRatio * cameraSize, -cameraSize, cameraSize, 0.1f, 100.0f);
					auto projMatrix = glm::perspective<float>(glm::radians(60.0f), aspectRatio, 0.1, 100);
					auto viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(position, 10));

					bgfx::setViewTransform(1, &viewMatrix[0][0], &projMatrix[0][0]);
				}

				mesh->Draw(1, { material }, transform);
				mesh2->Draw(1, { material }, transform2);
			}

			ImGui::EndFrame();
			imguiEndFrame();
		}
		
		// This should be done by either a Global Renderer or Display
		bgfx::frame();
	}

	bgfx::destroy(program);

	imguiDestroy();
	bgfx::shutdown();

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

