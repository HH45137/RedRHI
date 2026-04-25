cmake_minimum_required(VERSION 4.2)
project(RedRHI)

set(CMAKE_CXX_STANDARD 20)

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