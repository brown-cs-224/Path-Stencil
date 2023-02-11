cmake_minimum_required(VERSION 3.16)

# Sets project name
project(path LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Sets C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specifies required Qt components
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Xml)

# Specifies .cpp and .h files to be passed to the compiler
add_executable(${PROJECT_NAME}
    main.cpp
    pathtracer.cpp
    scene/scene.cpp
    BVH/BBox.cpp
    BVH/BVH.cpp
    scene/camera.cpp
    scene/basiccamera.cpp
    util/CS123XmlSceneParser.cpp
    scene/shape/mesh.cpp
    scene/shape/triangle.cpp

    pathtracer.h
    scene/scene.h
    BVH/BBox.h
    BVH/BVH.h
    BVH/IntersectionInfo.h
    BVH/Log.h
    BVH/Object.h
    BVH/Ray.h
    BVH/Stopwatch.h
    scene/camera.h
    scene/basiccamera.h
    util/CS123Common.h
    util/CS123ISceneParser.h
    util/CS123SceneData.h
    util/CS123XmlSceneParser.h
    scene/shape/Sphere.h
    scene/shape/mesh.h
    scene/shape/triangle.h
    util/tiny_obj_loader.h
    BVH/vector3.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Xml
)

#include openmp
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(${PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)
endif()

file(GLOB EIGEN_DIR_CONTENTS ${CMAKE_CURRENT_LIST_DIR}/Eigen/*)
list(LENGTH EIGEN_DIR_CONTENTS EIGEN_DIR_SIZE)
if(EIGEN_DIR_SIZE EQUAL 0)
    message(FATAL_ERROR "Eigen dependency not pulled, please run `git submodule update --init --recursive`")
endif()

# This allows you to `#include <Eigen/...>`
target_include_directories(${PROJECT_NAME} PRIVATE
    Eigen
)

# Set this flag to silence warnings on Windows
if (MSVC OR MSYS OR MINGW)
  set(CMAKE_CXX_FLAGS "-Wno-volatile")
endif()

# Set this flag to silence warnings on MacOS
if (APPLE)
  set(CMAKE_CXX_FLAGS "-Wno-deprecated-volatile")
endif()
