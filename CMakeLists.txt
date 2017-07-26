cmake_minimum_required (VERSION 3.7)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the root and call cmake from there")
endif()

project (retrieve_spotlight)

#compiler requirement
set (CMAKE_CXX_STANDARD 14)

set(INCLUDE_DIRS 
	"${PROJECT_SOURCE_DIR}")

include_directories(${INCLUDE_DIRS})

set(SOURCES
	"${PROJECT_SOURCE_DIR}/main.cpp"
	"${PROJECT_SOURCE_DIR}/conf_file.hpp"
	"${PROJECT_SOURCE_DIR}/stb_image.h"
	)

add_executable(retrieve_spotlight ${SOURCES})