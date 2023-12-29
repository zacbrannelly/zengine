################################################################################
# Target
################################################################################

# Make sure SDL2 (in vendorized form) is built before ZEngine-Core.
add_dependencies(ZEngine-Core SDL2_vendorized)
add_dependencies(ZEngine-Core SDL2_image_vendorized)
add_dependencies(ZEngine-Core SDL2_mixer_vendorized)

# Make sure the library is built as a framework on iOS.
set(FRAMEWORK_BUNDLE_IDENTIFIER "com.zacbrannelly.ZEngine-Core")
set_target_properties(ZEngine-Core PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  MACOSX_FRAMEWORK_IDENTIFIER ${FRAMEWORK_BUNDLE_IDENTIFIER}
  MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/framework.plist.in
  # "current version" in semantic format in Mach-O binary file
  VERSION 1.0.0
  # "compatibility version" in semantic format in Mach-O binary file
  SOVERSION 1.0.0
  XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET}
  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${CODE_SIGN_IDENTITY}
  XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
  XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY ${DEVICE_FAMILY}
  XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
)

# Copy the headers into the framework after building is complete.
file(GLOB_RECURSE HEADER_FILES "${CMAKE_CURRENT_LIST_DIR}/*.h")
foreach(HEADER ${HEADER_FILES})
  get_filename_component(DIR ${HEADER} DIRECTORY)
  get_filename_component(FILE_NAME ${HEADER} NAME)
  string(REPLACE "${CMAKE_CURRENT_LIST_DIR}" "" RELATIVE_DIR ${DIR})
  set(SOURCE_PATH ${HEADER})
  set(DEST_PATH "$<TARGET_FILE_DIR:ZEngine-Core>/Headers/${RELATIVE_DIR}/${FILE_NAME}")

  add_custom_command(TARGET ZEngine-Core POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:ZEngine-Core>/Headers/${RELATIVE_DIR}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_PATH} ${DEST_PATH}
    COMMENT "Copying ${FILE_NAME} to framework headers"
  )
endforeach()

################################################################################
# Linking
################################################################################

# Point the framework to the correct PhysX binaries to link to.
set(PHYSX_BIN_PATH "${CMAKE_SOURCE_DIR}/3rdparty/PhysX/physx/bin/ios64/checked")

target_link_libraries(ZEngine-Core
  PUBLIC ${COREAUDIO}
  PUBLIC ${AVFOUNDATION}
  PUBLIC ${AUDIOTOOLBOX}
  PUBLIC ${COREHAPTICS}
  PUBLIC ${GAMECONTROLLER}
  PUBLIC ${COREMOTION}
  PUBLIC ${COREGRAPHICS}
  PUBLIC ${OPENGLES}
)

################################################################################
# C# Interop Project
################################################################################

# For iOS we build the Interop project as a static library using NativeAOT.
set(ZENGINE_INTEROP_IOS_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Scripting/CSharp/Lib/ZEngine.Interop.iOS)
add_custom_target(ZEngine-Interop-Dotnet ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_PATH} ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_IOS_PROJECT_PATH} ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
  COMMAND ${CMAKE_COMMAND} -E env --unset=TARGETNAME ${DOTNET_EXECUTABLE} publish ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/ZEngine.Interop.csproj
  DEPENDS ZEngine-Core
)

add_library(ZEngine-Interop SHARED "Scripting/CSharp/Lib/dummy.cpp")
add_dependencies(ZEngine-Interop ZEngine-Interop-Dotnet)

set(INTEROP_FRAMEWORK_BUNDLE_IDENTIFIER "com.zacbrannelly.ZEngine-Interop")
set_target_properties(ZEngine-Interop PROPERTIES
  FRAMEWORK TRUE
  FRAMEWORK_VERSION A
  MACOSX_FRAMEWORK_IDENTIFIER ${INTEROP_FRAMEWORK_BUNDLE_IDENTIFIER}
  MACOSX_FRAMEWORK_INFO_PLIST ${CMAKE_CURRENT_LIST_DIR}/framework.plist.in
  # "current version" in semantic format in Mach-O binary file
  VERSION 1.0.0
  # "compatibility version" in semantic format in Mach-O binary file
  SOVERSION 1.0.0
  XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET}
  XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ${CODE_SIGN_IDENTITY}
  XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
  XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY ${DEVICE_FAMILY}
  XCODE_ATTRIBUTE_SKIP_INSTALL "YES"
)

add_custom_command(TARGET ZEngine-Interop POST_BUILD
  # Copy the ZEngine.Interop shared library into the ZEngine-Interop.framework bundle.
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/bin/Release/net8.0/ios-arm64/publish $<TARGET_FILE_DIR:ZEngine-Interop>
  # Rename ZEngine.Interop to ZEngine-Interop (to match the framework name).
  COMMAND mv $<TARGET_FILE_DIR:ZEngine-Interop>/ZEngine.Interop $<TARGET_FILE_DIR:ZEngine-Interop>/ZEngine-Interop
  # Rename the library from ZEngine.Interop to ZEngine-Interop in the binary itself to avoid runtime errors.
  COMMAND install_name_tool -id @rpath/ZEngine-Interop.framework/ZEngine-Interop $<TARGET_FILE:ZEngine-Interop>
  # Sign the framework bundle again.
  COMMAND codesign --force --sign ${CODE_SIGN_IDENTITY} --preserve-metadata=identifier,entitlements,flags $<TARGET_FILE_DIR:ZEngine-Interop>
  COMMENT "Inject the ZEngine-Interop shared library into the ZEngine-Interop.framework bundle and codesign it."
  DEPENDS ${ZENGINE_INTEROP_PROJECT_BUILD_PATH}
)
