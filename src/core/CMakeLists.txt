cmake_minimum_required(VERSION 3.16)
set(TARGET_NAME "core")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.in ${CMAKE_CURRENT_SOURCE_DIR}/version.h)

set(${TARGET_NAME}_HEADERS
  ${CMAKE_CURRENT_SOURCE_DIR}/about.h
  ${CMAKE_CURRENT_SOURCE_DIR}/dialog.h
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
  ${CMAKE_CURRENT_SOURCE_DIR}/version.h
  ${CMAKE_CURRENT_SOURCE_DIR}/parser.h
)

set(${TARGET_NAME}_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/about.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/dialog.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/parser.cc
)

set(${TARGET_NAME}_UIFORM
  ${CMAKE_CURRENT_SOURCE_DIR}/dialog.ui
  ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui
)

file(GLOB_RECURSE ${TARGET_NAME}_QRC ${PROJECT_SOURCE_DIR}/**/*.qrc)
qt5_add_resources(${TARGET_NAME}_RESOURCES ${${TARGET_NAME}_QRC})

set(ICON_RESOURCE "")
if(WIN32) # windows icon resource file, provided from /packaging directory
    set(ICON_RESOURCE "${PROJECT_BINARY_DIR}/exampleApp.icon.rc")
elseif (APPLE) #osx icon ressource file, provided from /packaging directory
    set(ICON_RESOURCE "${PROJECT_SOURCE_DIR}/shared/icons/generic.icns")
endif()



add_executable(${NAME_PROJECT} MACOSX_BUNDLE
  ${ICON_RESOURCE}
  ${${TARGET_NAME}_SOURCES}
  ${${TARGET_NAME}_HEADERS}
  ${${TARGET_NAME}_UIFORM}
  ${${TARGET_NAME}_RESOURCES}
)

target_include_directories(${NAME_PROJECT} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (${NAME_PROJECT} 
  PRIVATE 
  Threads::Threads
  Qt5::Core 
  Qt5::Widgets 
  Qt5::Concurrent 
  lsdyna 
  common 
  apdl
)

target_compile_options(${NAME_PROJECT} PRIVATE -c -fPIC -fstack-protector-strong)

if(APPLE)
    set_target_properties(${NAME_PROJECT} PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "LsDynaToAPDL")
    set_target_properties(${NAME_PROJECT} PROPERTIES MACOSX_BUNDLE_BUNDLE_GUI_IDENTIFIER "com.LsDynaToAPDL.LsDynaToAPDL")
    set_target_properties(${NAME_PROJECT} PROPERTIES MACOSX_BUNDLE_BUNDLE_VERSION "${VERSION}")
    set_target_properties(${NAME_PROJECT} PROPERTIES MACOSX_BUNDLE_SHORT_VERSION_STRING "${VERSION}")
    set_target_properties(${NAME_PROJECT} PROPERTIES MACOSX_BUNDLE_ICON_FILE generic.icns)
    set_target_properties(${NAME_PROJECT} PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${PROJECT_SOURCE_DIR}/shared/macos/MacOSXBundleInfo.plist.in)
    set_source_files_properties(${PROJECT_SOURCE_DIR}/shared/icons/generic.icns PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  if(COVERAGE)
    target_link_libraries(coverage)
  endif()
endif()


# Destination paths below are relative to ${CMAKE_INSTALL_PREFIX}
install(TARGETS ${NAME_PROJECT}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
)

