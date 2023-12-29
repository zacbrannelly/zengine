#import "EngineInterface.h"

#include <ZEngine-Core/Misc/Factory.h>
#include <ZEngine-Core/Display/Display.h>
#include <ZEngine-Core/Input/InputManager.h>
#include <ZEngine-Core/Audio/AudioSystem.h>
#include <ZEngine-Core/Rendering/Graphics.h>
#include <ZEngine-Core/Assets/AssetManager.h>
#include <ZEngine-Core/Assets/AssetCatalog.h>
#include <ZEngine-Core/Map/MapManager.h>
#include <ZEngine-Core/Map/Map.h>
#include <ZEngine-Core/Assets/Objects/MapAsset.h>
#include <ZEngine-Core/Physics/Physics3DSystem.h>
#include <ZEngine-Core/Physics/Time.h>
#include <ZEngine-Core/Scripting/CSharp/CSharpScriptSystem.h>

@interface EngineInterface () 
{
    // Declare a private variable here
    Display* _display;
    InputManager* _inputSystem;
    AudioSystem* _audioSystem;
    Physics3DSystem* _physicsSystem;
    Graphics* _graphicsSystem;
    AssetManager* _assetSystem;
    MapManager* _mapManager;
    CSharpScriptSystem* _scriptSystem;
    Time* _time;
}
@end

@implementation EngineInterface

- (void) initEngine:(void*) nativeWindowPtr withWidth:(int)width andHeight:(int)height
{
    Factory::Init();
    
    _display = new Display("Test", width, height);
    _display->Init();
    
    _inputSystem = InputManager::GetInstance();
    _inputSystem->Init(_display);
    
    _audioSystem = AudioSystem::GetInstance();
    _audioSystem->Init();
    
    _physicsSystem = Physics3DSystem::GetInstance();
    _physicsSystem->Init();
    
    _graphicsSystem = Graphics::GetInstance();
    _graphicsSystem->Init(nativeWindowPtr, width, height);
    
    _assetSystem = AssetManager::GetInstance();
    _assetSystem->Init();

    _scriptSystem = CSharpScriptSystem::GetInstance();
    _scriptSystem->Init();

    _time = Time::GetInstance();
    _time->Init();
    
    NSString* basePath = [[NSBundle mainBundle] bundlePath];
    _assetSystem->SetBasePath([basePath UTF8String]);
    
    NSString* catalogPath = [[NSBundle mainBundle] pathForResource:@"catalog" ofType:@"zengine"];
    AssetCatalog* catalog = new AssetCatalog();
    catalog->LoadCatalog([catalogPath UTF8String]);
    _assetSystem->SetCatalog(catalog);

    _mapManager = MapManager::GetInstance();

    auto mapAsset = _assetSystem->LoadAsset("test map", "Assets/maps/test_map.map", MAP_ASSET)->Cast<MapAsset>();
    _mapManager->SetCurrentMap(mapAsset->GetMap());
}

- (void) updateEngine
{
    auto currentMap = _mapManager->GetCurrentMap();
    if (currentMap != nullptr)
    {
        _physicsSystem->Update();
        currentMap->Update();
        currentMap->Render();
    }
    
    _graphicsSystem->Clear(0, 0, 0, 0, 255);
    _graphicsSystem->Viewport(0, 0, 0, _display->GetWidth(), _display->GetHeight());
    _graphicsSystem->Touch(0);
    
    _graphicsSystem->Render();

    _time->Tick();
}

- (void) shutdown
{
    _assetSystem->Shutdown();
    _inputSystem->Shutdown();
    _audioSystem->Shutdown();
    _physicsSystem->Shutdown();
    _graphicsSystem->Shutdown();
    _display->Shutdown();
    _scriptSystem->Shutdown();
    _time->Shutdown();
    
    delete _display;
}

@end
