cmake_minimum_required(VERSION 3.22)
project(TPM2_Geom)

set(CMAKE_CXX_STANDARD 17)

set(OpenMesh_DIR ${CMAKE_SOURCE_DIR}/libs/OpenMesh/build/install/share/OpenMesh/cmake)
find_package(OpenMesh REQUIRED)

set(Eigen3_DIR ${CMAKE_SOURCE_DIR}/libs/eigen/build/install/share/eigen3/cmake)
find_package(Eigen3 REQUIRED)

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
        )

SOURCE_GROUP("Source Files" FILES ${folder_source})
SOURCE_GROUP("Header Files" FILES ${folder_header})

include_directories(${CMAKE_SOURCE_DIR}/libs/glm ${CMAKE_SOURCE_DIR}/libs/glad/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${folder_source} ${folder_header} src/AxisMesh.cpp src/AxisMesh.h src/RayUtils.cpp src/RayUtils.h src/mesh_fonction_propagator.cpp src/mesh_fonction_propagator.h)

target_link_libraries(${CMAKE_PROJECT_NAME}
        OpenMeshCore
        OpenMeshTools
        Eigen3::Eigen
        glfw
        )