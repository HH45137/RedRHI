#pragma once

#include <vector>

#include "RedRHIBuffer.h"
#include "RedRHIPipeline.h"
#include "RedRHIResourcePool.h"
#include "RedRHIShader.h"


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
        void *_data
    ) = 0;

    virtual void DestroyBuffer(RedRHIBuffer *_buffer) = 0;

    virtual void BindBuffer(RedRHIBuffer *_buffer) = 0;

    virtual RedRHITexture *CreateTexture(
        RedRHITextureFormat _usage,
        RedRHITextureSamplerType _sampler_type,
        RedRHITextureAddressType _address_type,
        int32_t _width,
        int32_t _height,
        int32_t _mip_levels,
        unsigned char *_data
    ) = 0;

    virtual RedRHIShader *CreateShader(std::string &_src, RedRHIShaderStage _stage) = 0;

    virtual RedRHIShader *CreateShader(RedRHIShader *_vertex_shader, RedRHIShader *_fragment_shader) = 0;

    virtual void DestroyShader(RedRHIShader *_shader) = 0;

    virtual RedRHIPipeline *CreatePipeline(RedRHIPipelineDesc _desc) = 0;

    virtual void DestroyPipeline(RedRHIPipeline *_pipeline) = 0;

    virtual void BindPipeline(RedRHIPipeline *_pipeline) = 0;

    virtual void ClearColor(float _red, float _green, float _blue) = 0;

    virtual void ClearFrameBuffer() = 0;

    virtual void Draw(int32_t _element_count) = 0;

protected:
    RedRHIResourcePool resource_poll;
};
