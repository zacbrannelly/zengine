#include <iostream>
#include <sstream>
#include <fstream>

#include <ZEngine-Core/Display/Display.h>
#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Input/InputManager.h>
#include <ZEngine-Core/Rendering/Graphics.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Assets/Objects/MapAsset.h>
#include <ZEngine-Core/Physics/Time.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Map/MapManager.h>
#include <ZEngine-Core/Audio/AudioSystem.h>
#include <ZEngine-Core/Scripting/CSharp/CSharpScriptSystem.h>
#include <ZEngine-Core/Component/Camera.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <bimg/bimg.h>
#include <bx/bx.h>
#include <bx/allocator.h>
#include <bx/file.h>

bgfx::TextureHandle captureTexture;
uint8_t* captureBuffer = nullptr;
int captureSize = 0;

typedef struct FrameInfo
{
  int width;
  int height;
  int pitch;
  int size;
  uint8_t* data;
} FrameInfo;

void SetupEngine(void* windowPtr)
{
	// Initialize the factory (register the types)
	Factory::Init();

	// Init window
	/*Display display("ZEngine 3.0 - Player | By Zac Brannelly", 1366, 768);
	display.Init();*/

  // auto inputManager = InputManager::GetInstance();
  // inputManager->Init(&display);

	// Init scripting
	auto scriptSystem = CSharpScriptSystem::GetInstance();
	scriptSystem->Init();

	auto audioSystem = AudioSystem::GetInstance();
	audioSystem->Init();

	// Init graphics sub-system
	auto graphics = Graphics::GetInstance();
	graphics->Init(windowPtr, 1024, 600);
  // graphics->Init(&display);

	// Init asset management system
	auto assetManager = AssetManager::GetInstance();
	assetManager->Init();

	auto catalog = new AssetCatalog();
	catalog->LoadCatalog("catalog.zengine");

	assetManager->SetCatalog(catalog);

  scriptSystem->LoadProjectAssembly("ProjectTemplate.Scripts.dll");
	auto mapAsset = assetManager->LoadAsset("test map", "Assets/maps/test_map.map", MAP_ASSET)->Cast<MapAsset>();

	if (mapAsset == nullptr)
	{
		std::cout << "Failed to load the map!" << std::endl;
    return;
	}

  auto mapManager = MapManager::GetInstance();
  mapManager->SetCurrentMap(mapAsset->GetMap());

  // Setup
  auto camera = mapAsset->GetMap()->GetCameras().at(0);
  camera->SetRenderToTexture(true);
  camera->SetViewId(2);

  captureTexture = bgfx::createTexture2D(1024, 600, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_READ_BACK | BGFX_TEXTURE_BLIT_DST);

  bgfx::TextureInfo textureInfo;
  bgfx::calcTextureSize(textureInfo, 1024, 600, 1, false, false, 1, bgfx::TextureFormat::BGRA8);
  captureSize = textureInfo.storageSize;
  captureBuffer = new uint8_t[captureSize];
  memset(captureBuffer, 0, captureSize);

  // auto captureTexture = bgfx::createTexture2D(1024, 600, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_READ_BACK | BGFX_TEXTURE_BLIT_DST);

  // Render
  /*for (int i = 0; i < 30; i++) {
  camera->Render(-1);
  mapAsset->GetMap()->RenderWorld(2);
  graphics->Render();

  bgfx::TextureHandle textureHandle = camera->GetRenderTexture();

  auto captureTexture = bgfx::createTexture2D(1024, 600, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_READ_BACK | BGFX_TEXTURE_BLIT_DST);

  // Copy the texture to a capture texture
  graphics->Viewport(2, 0, 0, 1024, 600);
  graphics->Clear(2, 0, 1, 0, 1);
  bgfx::blit(2, captureTexture, 0, 0, textureHandle, 0, 0, 1024, 600);

  // Save the texture to a file
  
  // Get the data
  bgfx::TextureInfo textureInfo;
  bgfx::calcTextureSize(textureInfo, 1024, 600, 1, false, false, 1, bgfx::TextureFormat::BGRA8);
  uint32_t size = textureInfo.storageSize;
  uint8_t* data = new uint8_t[size];
  bgfx::readTexture(captureTexture, data);

  // Save the data
  bx::FileWriter fileWriter;
  bx::Error err;
  bx::open(&fileWriter, "test.png", false, &err);
  int pitch = 1024 * 4 * sizeof(uint8_t);
  bimg::imageWritePng(&fileWriter, 1024, 600, pitch, data, bimg::TextureFormat::BGRA8, false, &err);
  bx::close(&fileWriter);

  }*/

  // bgfx::TextureInfo textureInfo;
  // bgfx::calcTextureSize(textureInfo, 1024, 600, 1, false, false, 1, bgfx::TextureFormat::BGRA8);
  // uint32_t size = textureInfo.storageSize;
  // uint8_t* data = new uint8_t[size];
  // memset(data, 0, size);

  // while (!display.CloseRequested())
  // {
  //   display.Update();

  //   mapAsset->GetMap()->Render();
  //   graphics->Render();

  //   bgfx::TextureHandle textureHandle = camera->GetRenderTexture();
  //   bgfx::blit(1, captureTexture, 0, 0, textureHandle, 0, 0, 1024, 600);

  //   // Get the data
  //   bgfx::readTexture(captureTexture, data);

  //   std::ofstream file("test.png", std::ios::binary);
  //   file.write((char*)data, size);
  // }

  // display.Shutdown();
	// assetManager->Shutdown();
	// graphics->Shutdown();
	// audioSystem->Shutdown();
	// scriptSystem->Shutdown();
}

void RenderFrame(FrameInfo* frameInfo)
{
  auto graphics = Graphics::GetInstance();
  auto mapManager = MapManager::GetInstance();
  auto camera = mapManager->GetCurrentMap()->GetCameras().at(0);

  mapManager->GetCurrentMap()->Render();
  graphics->Render();

  bgfx::TextureHandle textureHandle = camera->GetRenderTexture();
  bgfx::blit(1, captureTexture, 0, 0, textureHandle, 0, 0, 1024, 600);

  bgfx::readTexture(captureTexture, captureBuffer);
  graphics->Render();
  graphics->Render();

  // std::ofstream file("render.png", std::ios::binary);
  // file.write((char*)captureBuffer, captureSize);

  frameInfo->width = 1024;
  frameInfo->height = 600;
  frameInfo->pitch = 1024 * 4 * sizeof(uint8_t);
  frameInfo->size = captureSize;
  frameInfo->data = new uint8_t[captureSize];
  memcpy(frameInfo->data, captureBuffer, captureSize);
}

void ShutdownEngine()
{
  AssetManager::GetInstance()->Shutdown();
	Graphics::GetInstance()->Shutdown();
	AudioSystem::GetInstance()->Shutdown();
	CSharpScriptSystem::GetInstance()->Shutdown();
}

extern "C" {
  void ZEngine_Editor_Web_SetupEngine(void* windowPtr) {
    SetupEngine(windowPtr);
  }
  const void ZEngine_Editor_Web_RenderFrame(FrameInfo* frameInfo) {
    RenderFrame(frameInfo);
  }
  void ZEngine_Editor_Web_DestroyFrame(FrameInfo* frameInfo) {
    delete[] frameInfo->data;
  }
  void ZEngine_Editor_Web_ShutdownEngine() {
    ShutdownEngine();
  }
}

int main(int argc, char** argv)
{
  SetupEngine(nullptr);
  return 0;
}