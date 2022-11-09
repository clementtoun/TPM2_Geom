cmake_minimum_required(VERSION 3.22)
project(TPM2_Geom)

set(CMAKE_CXX_STANDARD 17)

set(OpenMesh_DIR ${CMAKE_SOURCE_DIR}/libs/OpenMesh/build/install/share/OpenMesh/cmake)
find_package(OpenMesh REQUIRED)

set(Eigen_DIR ${CMAKE_SOURCE_DIR}/libs/eigen-3.4.0/build/install/share/eigen3/cmake)
find_package(Eigen3 REQUIRED)

find_package(OpenGL REQUIRED)

set(glfw3_DIR ${CMAKE_SOURCE_DIR}/libs/glfw/build/install/lib/cmake/glfw3)
find_package(glfw3 REQUIRED)

FILE(GLOB_RECURSE
        folder_source
        ${CMAKE_SOURCE_DIR}/src/*.cpp
        ${CMAKE_SOURCE_DIR}/libs/glad/glad.c
        )

FILE(GLOB_RECURSE
        folder_header
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/libs/glad/glad.h
        ${CMAKE_SOURCE_DIR}/libs/glm/glm/glm.hpp
        )

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

add_executable(${CMAKE_PROJECT_NAME} ${folder_source} ${folder_header} src/Application.cpp src/Application.h src/Shader.cpp src/Shader.h src/myApp.cpp src/myApp.h src/Drawable.cpp src/Drawable.h src/TriMesh.cpp src/TriMesh.h src/Renderer.cpp src/Renderer.h src/OpenMeshUtils.cpp src/OpenMeshUtils.h)

target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenMeshCore
        OpenMeshTools
        Eigen3::Eigen
        glfw
        )