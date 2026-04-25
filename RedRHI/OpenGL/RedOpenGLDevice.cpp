#include "RedOpenGLDevice.h"


RedRHIDevice *RedOpenGLDevice::Init(void *window_handle) {
    return this;
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
    RedRHIBufferUsage usage,
    RedRHIMemoryType memory_type,
    int32_t size,
    int32_t stride
) {
    auto buffer = new RedRHIBuffer{};

    buffer->usage = usage;
    buffer->memory_type = memory_type;
    buffer->size = size;
    buffer->stride = stride;

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
    RedRHITextureFormat usage,
    RedRHITextureSamplerType sampler_type,
    RedRHITextureAddressType address_type,
    int32_t width,
    int32_t height,
    int32_t mip_levels
) {
    auto texture = new RedRHITexture{};

    texture->usage = usage;
    texture->address_type = address_type;
    texture->width = width;
    texture->height = height;
    texture->mip_levels = mip_levels;

    {
        texture->is_destroy = false;
        texture->state = RED_RHI_RESOURCE_STATE_INITIALIZED;
        texture->debug_name = "This is a Texture";
        texture->ref_count = 0;
        texture->ref_count++;
    }

    return texture;
}
