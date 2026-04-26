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
    resource_poll.ForceReleaseAll();

    std::cout << "The OpenGL backend has been destroyed." << std::endl;
}

RedRHIAdapterInfo *RedOpenGLDevice::GetAdapterInfo() {
    auto adapter_info = new RedRHIAdapterInfo{};

    adapter_info->vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    adapter_info->gpu = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    adapter_info->gl_version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    adapter_info->glsl_version = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &adapter_info->max_texture_2d_size);
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &adapter_info->max_texture_3d_size);
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &adapter_info->max_uniform_buffer_bindings);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &adapter_info->max_uniform_block_size);
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &adapter_info->max_uniform_buffer_offset_alignment);

    // Get all supported extensions
    GLint num_extensions = 0;
    std::vector<std::string> extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    for (int i = 0; i < num_extensions; ++i) {
        extensions.emplace_back(reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i)));
    }

    return adapter_info;
}

RedRHIBuffer *RedOpenGLDevice::CreateBuffer(
    RedRHIBufferUsage _usage,
    RedRHIMemoryType _memory_type,
    size_t _size,
    int32_t _stride,
    void *_data
) {
    auto buffer = new RedRHIBuffer{};

    buffer->usage = _usage;
    buffer->memory_type = _memory_type;
    buffer->size = _size;

    switch (_usage) {
        case RED_RHI_BUFFER_USAGE_VERTEX:
            break;

        case RED_RHI_BUFFER_USAGE_INDEX:
            break;

        case RED_RHI_BUFFER_USAGE_UNIFORM:
            break;

        case RED_RHI_BUFFER_USAGE_STORAGE:
            break;

        case RED_RHI_BUFFER_USAGE_STAGING:
            break;

        default:
            throw std::runtime_error("Invalid buffer usage");
    }

    resource_poll.Register(buffer);

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

    resource_poll.Register(texture);

    return texture;
}

void RedOpenGLDevice::DrawFrame() {
    glClearColor(0.8, 0.0, 0.6, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
