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
    std::string gpu;
    std::string vendor;
    std::string gl_version;
    std::string glsl_version;
    int32_t ram_size;
    int32_t max_texture_2d_size;
    int32_t max_texture_3d_size;
    int32_t max_uniform_buffer_bindings;
    int32_t max_uniform_block_size;
    int32_t max_uniform_buffer_offset_alignment;
    std::vector<RedRHITextureFormat> formats;
    std::vector<RedRHIFeature> features;
};

class RedRHIDevice {
public:
    virtual ~RedRHIDevice() = default;

    virtual bool Init() = 0;

    virtual void Destroy() = 0;

    virtual RedRHIAdapterInfo *GetAdapterInfo() = 0;

    virtual RedRHIBuffer *CreateBuffer(
        RedRHIBufferUsage _usage,
        RedRHIMemoryType _memory_type,
        size_t _size,
        int32_t _stride,
        void *_data
    ) = 0;

    virtual RedRHITexture *CreateTexture(
        RedRHITextureFormat _usage,
        RedRHITextureSamplerType _sampler_type,
        RedRHITextureAddressType _address_type,
        int32_t _width,
        int32_t _height,
        int32_t _mip_levels
    ) = 0;

    virtual void DrawFrame() = 0;
};
