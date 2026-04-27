#pragma once

#include "../RedRHI.h"


struct RedOpenGLBuffer : RedRHIBuffer {
    uint32_t gl_buffer;
};

struct RedOpenGLTexture : RedRHITexture {
    uint32_t gl_texture;
};
