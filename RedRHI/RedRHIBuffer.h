#pragma once

#include <cstdint>
#include <string>

#include "RedRHIUtility.h"


enum RedRHIBufferUsage {
    RED_RHI_BUFFER_USAGE_VERTEX,
    RED_RHI_BUFFER_USAGE_INDEX,
    RED_RHI_BUFFER_USAGE_UNIFORM,
    RED_RHI_BUFFER_USAGE_STORAGE,
    RED_RHI_BUFFER_USAGE_STAGING,

    RED_RHI_BUFFER_USAGE_COUNT
};

enum RedRHIMemoryType {
    RED_RHI_MEMORY_TYPE_HOST,
    RED_RHI_MEMORY_TYPE_DEVICE,
    RED_RHI_MEMORY_TYPE_SHARED,

    RED_RHI_MEMORY_TYPE_COUNT
};

enum RedRHITextureSamplerType {
    RED_RHI_TEXTURE_SAMPLER_TYPE_NEAREST,
    RED_RHI_TEXTURE_SAMPLER_TYPE_LINEAR,

    RED_RHI_TEXTURE_SAMPLER_TYPE_COUNT
};

enum RedRHITextureAddressType {
    RED_RHI_TEXTURE_ADDRESS_TYPE_REPEAT,
    RED_RHI_TEXTURE_ADDRESS_TYPE_MIRROR_REPEAT,
    RED_RHI_TEXTURE_ADDRESS_TYPE_CLAMP_TO_EDGE,
    RED_RHI_TEXTURE_ADDRESS_TYPE_CLAMP_TO_BORDER,

    RED_RHI_TEXTURE_ADDRESS_TYPE_COUNT
};

enum RedRHITextureFormat {
    RED_RHI_TEXTURE_FORMAT_R_8,
    RED_RHI_TEXTURE_FORMAT_RG_8,
    RED_RHI_TEXTURE_FORMAT_RGB_8,
    RED_RHI_TEXTURE_FORMAT_RGBA_8,
    RED_RHI_TEXTURE_FORMAT_UNKNOWN,

    RED_RHI_TEXTURE_FORMAT_COUNT
};

struct RedRHIResource {
    std::string uuid = GEN_UUID_STR;
    size_t ref_count;
    std::string debug_name;
};

struct RedRHIBuffer : RedRHIResource {
    RedRHIBufferUsage usage;
    RedRHIMemoryType memory_type;
    size_t size;
};

struct RedRHITexture : RedRHIResource {
    RedRHITextureFormat usage;
    RedRHITextureSamplerType sampler_type;
    RedRHITextureAddressType address_type;
    int32_t width, height;
    int32_t mip_levels;
};
