cmake_minimum_required(VERSION 3.16)
set(LIB_NAME "lsdyna")

set(LIBRARY_TARGET_NAME ${LIB_NAME})

set(SOURCES 


${CMAKE_CURRENT_SOURCE_DIR}/converter.cc
            ${CMAKE_CURRENT_SOURCE_DIR}/keywords.cc

            ${CMAKE_CURRENT_SOURCE_DIR}/converterdialog.cc
            
            
            
            
            
            
            )

set(HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/converter.h
            ${CMAKE_CURRENT_SOURCE_DIR}/keywords.h
            
            ${CMAKE_CURRENT_SOURCE_DIR}/converterdialog.h
            
            )

add_library(${LIB_NAME})

target_link_libraries(${LIB_NAME} Qt5::Core Qt5::Widgets logger)

target_sources(
  ${LIB_NAME}
  PRIVATE ${SOURCES}
  PUBLIC ${HEADERS})

target_include_directories(${LIB_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_compile_options(
  ${LIB_NAME}
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
