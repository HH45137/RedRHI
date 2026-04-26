cmake_minimum_required(VERSION 4.2)
project(RedRHI)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

add_subdirectory(Thirdpart/SDL EXCLUDE_FROM_ALL)

set(RHI_SRC
        RedRHI/RedRHIBuffer.h
        RedRHI/RedRHI.h
        RedRHI/RedRHISync.h
        RedRHI/RedRHICommandList.h
        RedRHI/RedRHIDescriptor.h
        RedRHI/RedRHIDevice.h
        RedRHI/RedRHIPipeline.h
        RedRHI/RedRHIResourcePool.h
        RedRHI/RedRHIShader.h
        RedRHI/RedRHISwapChain.h

        RedRHI/OpenGL/RedOpenGLDevice.cpp
        RedRHI/OpenGL/RedOpenGLDevice.h
        RedRHI/OpenGL/RedOpenGL.h
        RedRHI/RedRHIUtility.h
)

add_executable(RedRHI main.cpp ${RHI_SRC})

add_definitions(-DUUID_SYSTEM_GENERATOR)
include_directories(Thirdpart/)

target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)
