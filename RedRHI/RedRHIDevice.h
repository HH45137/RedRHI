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

    virtual bool Init() = 0;

    virtual void Destroy() = 0;

    virtual const RedRHIAdapterInfo *GetAdapterInfo() = 0;

    virtual const RedRHIBuffer *CreateBuffer(
        RedRHIBufferUsage _usage,
        RedRHIMemoryType _memory_type,
        int32_t _size,
        int32_t _stride
    ) = 0;

    virtual const RedRHITexture *CreateTexture(
        RedRHITextureFormat _usage,
        RedRHITextureSamplerType _sampler_type,
        RedRHITextureAddressType _address_type,
        int32_t _width,
        int32_t _height,
        int32_t _mip_levels
    ) = 0;

    virtual void DrawFrame() = 0;
};
