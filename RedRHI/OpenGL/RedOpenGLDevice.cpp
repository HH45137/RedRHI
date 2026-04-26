#include "RedOpenGLDevice.h"

#include <iostream>

#include "glad/glad.h"
#include "SDL3/SDL_video.h"


bool RedOpenGLDevice::Init() {
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    std::cout << "The OpenGL backend has been initialized." << std::endl;
    return true;
}

void RedOpenGLDevice::Destroy() {
    std::cout << "The OpenGL backend has been destroyed." << std::endl;
}

RedRHIAdapterInfo *RedOpenGLDevice::GetAdapterInfo() {
    auto adapter_info = new RedRHIAdapterInfo{};

    adapter_info->name = "GPU NAME IS HERE!";
    adapter_info->ram_size = 0;
    adapter_info->max_texture_size = 0;
    adapter_info->max_uniform_buffer_range = 0;

    adapter_info->formats.push_back(RED_RHI_TEXTURE_FORMAT_RGB_8);
    adapter_info->features.push_back(RED_RHI_FEATURE_RAY_TRACING);

    return adapter_info;
}

RedRHIBuffer *RedOpenGLDevice::CreateBuffer(
    RedRHIBufferUsage _usage,
    RedRHIMemoryType _memory_type,
    int32_t _size,
    int32_t _stride
) {
    auto buffer = new RedRHIBuffer{};

    buffer->usage = _usage;
    buffer->memory_type = _memory_type;
    buffer->size = _size;
    buffer->stride = _stride;

    {
        buffer->is_destroy = false;
        buffer->state = RED_RHI_RESOURCE_STATE_INITIALIZED;
        buffer->debug_name = "This is a Buffer";
        buffer->ref_count = 0;
        buffer->ref_count++;
    }

    return buffer;
}

RedRHITexture *RedOpenGLDevice::CreateTexture(
    RedRHITextureFormat _usage,
    RedRHITextureSamplerType _sampler_type,
    RedRHITextureAddressType _address_type,
    int32_t _width,
    int32_t _height,
    int32_t _mip_levels
) {
    auto texture = new RedRHITexture{};

    texture->usage = _usage;
    texture->address_type = _address_type;
    texture->width = _width;
    texture->height = _height;
    texture->mip_levels = _mip_levels;

    {
        texture->is_destroy = false;
        texture->state = RED_RHI_RESOURCE_STATE_INITIALIZED;
        texture->debug_name = "This is a Texture";
        texture->ref_count = 0;
        texture->ref_count++;
    }

    return texture;
}

void RedOpenGLDevice::DrawFrame() {
    glClearColor(0.8, 0.0, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
