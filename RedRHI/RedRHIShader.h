#pragma once

#include "RedRHIBuffer.h"
#include "RedRHIUtility.h"


enum RedRHIShaderStage {
    RED_RHI_SHADER_STAGE_VERTEX,
    RED_RHI_SHADER_STAGE_FRAGMENT,
    RED_RHI_SHADER_STAGE_COMPUTE,

    RED_RHI_SHADER_STAGE_COUNT
};

struct RedRHIShader : RedRHIResource {
    std::string src{};
    std::string entry_point = "main";
    RedRHIShaderStage stage = RED_RHI_SHADER_STAGE_VERTEX;
};
