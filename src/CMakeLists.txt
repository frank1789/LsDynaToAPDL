cmake_minimum_required(VERSION 3.16)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/about.h
    ${CMAKE_CURRENT_SOURCE_DIR}/apdlinterpreter.h
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/element.h
    ${CMAKE_CURRENT_SOURCE_DIR}/finitelement.h
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/shell.h
    ${CMAKE_CURRENT_SOURCE_DIR}/writeapdl.h)

include_directories(${HEADERS})

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/about.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/element.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/finitelement.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/lsdynainterpreter.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/shell.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/writeapdl.cpp)

set(UIFORM
    ${CMAKE_CURRENT_SOURCE_DIR}/about.ui 
    ${CMAKE_CURRENT_SOURCE_DIR}/dialog.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/mainwindow.ui)

qt5_add_resources(RESOURCES ${CMAKE_CURRENT_SOURCE_DIR}/resources.qrc)

add_executable(${NAME_PROJECT} ${SOURCES} ${UIFORM} ${RESOURCES})
target_link_libraries(${NAME_PROJECT} Qt5::Core Qt5::Widgets lsdyna logger)
target_compile_options(
  ${NAME_PROJECT}
  PRIVATE -c
          -Wall
          -Wextra
          -fPIC
          -Wunused
          -Wshadow
          -Wredundant-decls
          -Wcast-align
          -Wmissing-declarations
          -Wmissing-include-dirs
          -Winvalid-pch
          -Wredundant-decls
          -Wmissing-format-attribute
          -Wformat-nonliteral
          -fstack-protector-strong
          -fexceptions)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
  if(COVERAGE)
    target_link_libraries(coverage)
  endif()
else()
  target_compile_options(${LIB_NAME} PUBLIC -O2)
endif()
