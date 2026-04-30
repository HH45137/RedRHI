#pragma once

#include "../RedRHI.h"
#include "glad/glad.h"

struct RedOpenGLPipeline;

class RedOpenGLDevice : public RedRHIDevice {
public:
    bool Init() override;

    void Destroy() override;

    RedRHIAdapterInfo *GetAdapterInfo() override;

    RedRHIBuffer *CreateBuffer(
        RedRHIBufferUsage _usage,
        RedRHIMemoryType _memory_type,
        size_t _size,
        void *_data
    ) override;

    void DestroyBuffer(RedRHIBuffer *_buffer) override;

    void BindBuffer(RedRHIBuffer *_buffer) override;

    RedRHITexture *CreateTexture(
        RedRHITextureFormat _usage,
        RedRHITextureSamplerType _sampler_type,
        RedRHITextureAddressType _address_type,
        int32_t _width,
        int32_t _height,
        int32_t _mip_levels
    ) override;

    RedRHIShader *CreateShader(std::string &_src, RedRHIShaderStage _stage) override;

    RedRHIShader *CreateShader(RedRHIShader *_vertex_shader, RedRHIShader *_fragment_shader) override;

    void DestroyShader(RedRHIShader *_shader) override;

    RedRHIPipeline *CreatePipeline(RedRHIPipelineDesc _desc) override;

    void DestroyPipeline(RedRHIPipeline *_pipeline) override;

    void BindPipeline(RedRHIPipeline *_pipeline) override;

    void ClearColor(float _red, float _green, float _blue) override;

    void ClearFrameBuffer() override;

private:
    RedOpenGLPipeline *bound_pipeline{};

    static GLenum MapFormatType(RedRHIFormatType _type);

    static GLenum MapCompareOp(RedRHICompareOp _compare_op);

    static GLenum MapBlendFactor(RedRHIBlendFactor _blend_factor);
};
