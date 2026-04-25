#pragma once

#include <vector>

#include "RedRHIBuffer.h"


enum RedRHIFeature {
    RED_RHI_FEATURE_RAY_TRACING,
    RED_RHI_FEATURE_MESH_SHADER,
    RED_RHI_FEATURE_VRS,

    RED_RHI_FEATURE_COUNT
};

struct RedRHIAdapterInfo {
    std::string name;
    size_t ram_size;
    size_t max_texture_size;
    size_t max_uniform_buffer_range;
    std::vector<RedRHITextureFormat> formats;
    std::vector<RedRHIFeature> features;
};

class RedRHIDevice {
public:
    virtual ~RedRHIDevice() = default;

    virtual const RedRHIDevice *Init(void *window_handle) = 0;

    virtual const RedRHIAdapterInfo *GetAdapterInfo() = 0;

    virtual const RedRHIBuffer *CreateBuffer(
        RedRHIBufferUsage usage,
        RedRHIMemoryType memory_type,
        int32_t size,
        int32_t stride
    ) = 0;

    virtual const RedRHITexture *CreateTexture(
        RedRHITextureFormat usage,
        RedRHITextureSamplerType sampler_type,
        RedRHITextureAddressType address_type,
        int32_t width,
        int32_t height,
        int32_t mip_levels
    ) = 0;
};
