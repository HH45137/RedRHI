#pragma once

#include "../RedRHI.h"


struct RedOpenGLBuffer : RedRHIBuffer {
    unsigned int gl_buffer = 0;
};

struct RedOpenGLTexture : RedRHITexture {
    unsigned int gl_texture = 0;
};
