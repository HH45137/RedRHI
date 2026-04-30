#pragma once

#include <vector>

#include "RedRHIBuffer.h"
#include "RedRHIShader.h"


enum RedRHIPrimitiveTopology {
    RED_RHI_PRIMITIVE_TOPOLOGY_POINT_LIST,
    RED_RHI_PRIMITIVE_TOPOLOGY_LINE_LIST,
    RED_RHI_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    RED_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    RED_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,

    RED_RHI_PRIMITIVE_TOPOLOGY_COUNT
};

enum RedRHICullMode {
    RED_RHI_CULL_MODE_NONE,
    RED_RHI_CULL_MODE_FRONT,
    RED_RHI_CULL_MODE_BACK,

    RED_RHI_CULL_MODE_COUNT
};

enum RedRHICompareOp {
    RED_RHI_COMPARE_OP_NEVER,
    RED_RHI_COMPARE_OP_LESS,
    RED_RHI_COMPARE_OP_EQUAL,
    RED_RHI_COMPARE_OP_LESS_OR_EQUAL,
    RED_RHI_COMPARE_OP_GREATER,
    RED_RHI_COMPARE_OP_NOT_EQUAL,
    RED_RHI_COMPARE_OP_GREATER_OR_EQUAL,
    RED_RHI_COMPARE_OP_ALWAYS,

    RED_RHI_COMPARE_OP_COUNT
};

enum RedRHIBlendFactor {
    RED_RHI_BLEND_FACTOR_ZERO,
    RED_RHI_BLEND_FACTOR_ONE,
    RED_RHI_BLEND_FACTOR_SRC_ALPHA,
    RED_RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,

    RED_RHI_BLEND_FACTOR_COUNT
};


struct RedRHIVertexAttributeDesc {
    int32_t index{};
    int32_t size{};
    RedRHIFormatType type = RED_RHI_FORMAT_TYPE_FLOAT;
    bool normalized{};
    int32_t stride{};
    int32_t offset{};
};

struct RedRHIVertexInputDesc {
    RedRHIVertexAttributeDesc attributes[8]{};
    RedRHIPrimitiveTopology topology = RED_RHI_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
};

struct RedRHIRasterizerDesc {
    RedRHICullMode cull_mode = RED_RHI_CULL_MODE_BACK;
};

struct RedRHIDepthStencilDesc {
    bool depth_test = true;
    bool depth_write = true;
    RedRHICompareOp compare_op = RED_RHI_COMPARE_OP_LESS;
};

struct RedRHIBlendDesc {
    bool blend_enable = false;
    RedRHIBlendFactor src_color_blend_factor = RED_RHI_BLEND_FACTOR_ONE;
    RedRHIBlendFactor dst_color_blend_factor = RED_RHI_BLEND_FACTOR_ZERO;
    RedRHIBlendFactor src_alpha_blend_factor = RED_RHI_BLEND_FACTOR_ONE;
    RedRHIBlendFactor dst_alpha_blend_factor = RED_RHI_BLEND_FACTOR_ZERO;
};

struct RedRHIPipelineDesc {
    RedRHIShader *shader{};
    RedRHIVertexInputDesc vertex_input_desc;
    RedRHIRasterizerDesc rasterizer_desc;
    RedRHIDepthStencilDesc depth_stencil_desc;
    RedRHIBlendDesc blend_desc;
};

struct RedRHIPipeline : RedRHIResource {
    RedRHIPipelineDesc desc;
};
