#pragma once

#include "../RedRHI.h"


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

    RedRHITexture *CreateTexture(
        RedRHITextureFormat _usage,
        RedRHITextureSamplerType _sampler_type,
        RedRHITextureAddressType _address_type,
        int32_t _width,
        int32_t _height,
        int32_t _mip_levels
    ) override;

    void ClearColor(float _red, float _green, float _blue) override;

    void ClearFrameBuffer() override;

private:
    unsigned int gl_vao{};
};
