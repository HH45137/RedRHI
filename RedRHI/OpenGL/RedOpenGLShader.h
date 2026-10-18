#pragma once

#include "../RedRHI.h"


struct RedOpenGLShader : RedRHIShader {
    unsigned int gl_shader = 0;
    bool is_final_program = false;
};
