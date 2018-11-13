cmake_minimum_required(VERSION 3.1)

if(MSVC)
	add_definitions(-D_SCL_SECURE_NO_WARNINGS) # silence visual studio security warnings
	add_definitions(-D_USE_MATH_DEFINES) # make sure M_PI is in cmath
endif()

# add include folder
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_subdirectory(python)