################################################################################
# Compiler flags
################################################################################
set (CMAKE_CXX_STANDARD 20)

################################################################################
# Source files
################################################################################
if (NOT IOS)
  add_executable (ZEngine-Player main.cpp)
else()
  include(BundleUtilities)

  set(APP_NAME "ZEngine-Player")
  set(APP_BUNDLE_IDENTIFIER "com.zacbrannelly.ZEngine-Player")

  set(PRODUCT_NAME ${APP_NAME})
  set(EXECUTABLE_NAME ${APP_NAME})
  set(MACOSX_BUNDLE_EXECUTABLE_NAME ${APP_NAME})
  set(MACOSX_BUNDLE_INFO_STRING ${APP_BUNDLE_IDENTIFIER})
  set(MACOSX_BUNDLE_GUI_IDENTIFIER ${APP_BUNDLE_IDENTIFIER})
  set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_BUNDLE_IDENTIFIER})
  set(MACOSX_BUNDLE_ICON_FILE "")
  set(MACOSX_BUNDLE_LONG_VERSION_STRING "1.0")
  set(MACOSX_BUNDLE_SHORT_VERSION_STRING "1.0")
  set(MACOSX_BUNDLE_BUNDLE_VERSION "1.0")
  set(MACOSX_BUNDLE_COPYRIGHT "Copyright Zac Brannelly")
  set(MACOSX_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET})

  set(APP_HEADER_FILES
    ./ios/AppDelegate.h
    ./ios/ViewController.h
    ./ios/EngineInterface.h
  )

  set(APP_SOURCE_FILES
    ./ios/AppDelegate.m
    ./ios/ViewController.m
    ./ios/EngineInterface.mm
    ./ios/main.m
  )

  set(RESOURCES
    ./ios/Main.storyboard
    ./ios/LaunchScreen.storyboard
  )

  add_executable (
    ${APP_NAME}
    MACOSX_BUNDLE
    ${APP_HEADER_FILES}
    ${APP_SOURCE_FILES}
    ${RESOURCES}
  )

  add_dependencies(${APP_NAME} ZEngine-Core ZEngine-Interop-Dotnet ZEngine-Interop)

  find_library(UIKIT UIKit REQUIRED)
  find_library(FOUNDATION Foundation REQUIRED)
  find_library(MOBILECORESERVICES MobileCoreServices REQUIRED)
  find_library(CFNETWORK CFNetwork REQUIRED)
  find_library(SYSTEMCONFIGURATION SystemConfiguration REQUIRED)
  find_library(METAL Metal REQUIRED)
  find_library(METALKIT MetalKit REQUIRED)
  find_library(QUATZCORE QuartzCore REQUIRED)

  target_link_libraries(${APP_NAME} PRIVATE ${UIKIT})
  target_link_libraries(${APP_NAME} PRIVATE ${FOUNDATION})
  target_link_libraries(${APP_NAME} PRIVATE ${MOBILECORESERVICES})
  target_link_libraries(${APP_NAME} PRIVATE ${CFNETWORK})
  target_link_libraries(${APP_NAME} PRIVATE ${SYSTEMCONFIGURATION})
  target_link_libraries(${APP_NAME} PRIVATE ${METAL})
  target_link_libraries(${APP_NAME} PRIVATE ${METALKIT})
  target_link_libraries(${APP_NAME} PRIVATE ${QUATZCORE})

  # Link the framework to the app (I wonder if we need this part)
  set_target_properties(${APP_NAME} PROPERTIES
    XCODE_ATTRIBUTE_OTHER_LDFLAGS "${XCODE_ATTRIBUTE_OTHER_LDFLAGS} -framework ZEngine-Core -framework ZEngine-Interop"
  )

  # Turn on ARC
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fobjc-arc")

  # Create the app target
  set_target_properties(${APP_NAME} PROPERTIES
    XCODE_ATTRIBUTE_DEBUG_INFORMATION_FORMAT "dwarf-with-dsym"
    RESOURCE "${RESOURCES}"
    XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER "NO"
    XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET ${DEPLOYMENT_TARGET}
    XCODE_ATTRIBUTE_DEVELOPMENT_TEAM ${DEVELOPMENT_TEAM_ID}
    XCODE_ATTRIBUTE_TARGETED_DEVICE_FAMILY ${DEVICE_FAMILY}
    MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/ios/plist.in
    XCODE_ATTRIBUTE_CLANG_ENABLE_OBJC_ARC YES
    XCODE_ATTRIBUTE_COMBINE_HIDPI_IMAGES NO
    XCODE_ATTRIBUTE_INSTALL_PATH "$(LOCAL_APPS_DIR)"
    XCODE_ATTRIBUTE_ENABLE_TESTABILITY YES
    XCODE_ATTRIBUTE_GCC_SYMBOLS_PRIVATE_EXTERN YES
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Automatic"
  )

  # Include framework headers, needed to make "Build" Xcode action work.
  # "Archive" works fine just relying on default search paths as it has different
  # build product output directory.
  target_include_directories(${APP_NAME} PUBLIC 
    "$<TARGET_FILE_DIR:ZEngine-Core>"
  )

  # Include ZEngine-Core's include directories too.
  target_include_directories(${APP_NAME} PUBLIC
    "$<TARGET_PROPERTY:ZEngine-Core,INCLUDE_DIRECTORIES>"
  )

  # Set the app's linker search path to the default location on iOS
  # Plus search the Frameworks directory in the ZEngine-Core framework (we will put the ZEngine-Core-Interop dylib in there)
  set_target_properties(
    ${APP_NAME}
    PROPERTIES
    XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS
    "@executable_path/Frameworks"
  )

  # Copy the ZEngine-Core framework into the app bundle
  add_custom_command(
    TARGET ${APP_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "$<TARGET_FILE_DIR:ZEngine-Core>"
    "$<TARGET_FILE_DIR:${APP_NAME}>/Frameworks/ZEngine-Core.framework"
  )

  # Copy the ZEngine-Interop framework into the app bundle
  add_custom_command(
    TARGET ${APP_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "$<TARGET_FILE_DIR:ZEngine-Interop>"
    "$<TARGET_FILE_DIR:${APP_NAME}>/Frameworks/ZEngine-Interop.framework"
  )

  # Copy the ZEngine-Interop dat files into the app bundle
  set(ZENGINE_INTEROP_PROJECT_BUILD_PATH ${CMAKE_CURRENT_BINARY_DIR}/../ZEngine-Core/projects/ZEngine.Interop)
  add_custom_command(
    TARGET ${APP_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${ZENGINE_INTEROP_PROJECT_BUILD_PATH}/bin/Release/net8.0/ios-arm64/icudt.dat"
    "$<TARGET_FILE_DIR:${APP_NAME}>/icudt.dat"
  )

  # Copy the game assets into the app bundle
  add_custom_command(
    TARGET ${APP_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/Assets"
    "$<TARGET_FILE_DIR:${APP_NAME}>/Assets"
  )
  add_custom_command(
    TARGET ${APP_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    "${CMAKE_CURRENT_SOURCE_DIR}/catalog.zengine"
    "$<TARGET_FILE_DIR:${APP_NAME}>/catalog.zengine"
  )
endif()

################################################################################
# Includes
################################################################################
target_include_directories(ZEngine-Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/bgfx.cmake/bgfx/include)
target_include_directories(ZEngine-Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stb)
target_include_directories(ZEngine-Player PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../3rdparty/stduuid/include)
target_include_directories(ZEngine-Player PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(ZEngine-Player PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})
target_include_directories(ZEngine-Player PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_include_directories(ZEngine-Player PRIVATE ${GLM_INCLUDE_DIRS})
target_include_directories(ZEngine-Player PRIVATE ${GLFW_INCLUDE_DIRS})

################################################################################
# Linking
################################################################################
target_link_libraries (ZEngine-Player 
  PRIVATE ZEngine-Core
  PRIVATE ${SDL2_MIXER_LIBRARIES}
)

if (NOT IOS)
  target_link_libraries (ZEngine-Player 
    PRIVATE glfw
  )
endif()
