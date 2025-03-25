set(PHYSX_BIN_PATH "${CMAKE_SOURCE_DIR}/3rdparty/PhysX/physx/bin/UNKNOWN/checked")
message(STATUS "PhysX bin path: ${PHYSX_BIN_PATH}")

set(PHYSX_COMMON "${PHYSX_BIN_PATH}/libPhysXCommon_static.a")
set(PHYSX_CORE "${PHYSX_BIN_PATH}/libPhysX_static.a")
set(PHYSX_FOUNDATION "${PHYSX_BIN_PATH}/libPhysXFoundation_static.a")
set(PHYSX_CHARACTER_KINEMATIC "${PHYSX_BIN_PATH}/libPhysXCharacterKinematic_static.a")
set(PHYSX_COOKING "${PHYSX_BIN_PATH}/libPhysXCooking_static.a")
set(PHYSX_PVD "${PHYSX_BIN_PATH}/libPhysXPvdSDK_static.a")
set(PHYSX_EXTENSIONS "${PHYSX_BIN_PATH}/libPhysXExtensions_static.a")

add_dependencies(ZEngine-Core SDL2_vendorized)
add_dependencies(ZEngine-Core SDL2_mixer_vendorized)

# The path used in DllImport attribute in C# code
set(ZENGINE_INTEROP_DLL_IMPORT_PATH __Internal)
