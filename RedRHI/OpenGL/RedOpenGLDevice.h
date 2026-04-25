#pragma once

#include "../RedRHI.h"


class RedOpenGLDevice : public RedRHIDevice {
public:
    RedRHIDevice *Init(void *window_handle) override;

    RedRHIAdapterInfo *GetAdapterInfo() override;

    RedRHIBuffer *CreateBuffer(
        RedRHIBufferUsage usage,
        RedRHIMemoryType memory_type,
        int32_t size,
        int32_t stride
    ) override;

    RedRHITexture *CreateTexture(
        RedRHITextureFormat usage,
        RedRHITextureSamplerType sampler_type,
        RedRHITextureAddressType address_type,
        int32_t width,
        int32_t height,
        int32_t mip_levels
    ) override;
};
