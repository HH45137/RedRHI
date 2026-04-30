#include "RedOpenGLDevice.h"
#include "RedOpenGLBuffer.h"

#include <iostream>

#include "SDL3/SDL_video.h"


bool RedOpenGLDevice::Init() {
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    std::cout << "The OpenGL backend has been initialized." << std::endl;
    return true;
}

void RedOpenGLDevice::Destroy() {
    resource_poll.ForceReleaseAll();

    std::cout << "The OpenGL backend has been destroyed." << std::endl;
}

RedRHIAdapterInfo *RedOpenGLDevice::GetAdapterInfo() {
    auto adapter_info = new RedRHIAdapterInfo{};

    adapter_info->vendor = reinterpret_cast<const char *>(glGetString(GL_VENDOR));
    adapter_info->gpu = reinterpret_cast<const char *>(glGetString(GL_RENDERER));
    adapter_info->gl_version = reinterpret_cast<const char *>(glGetString(GL_VERSION));
    adapter_info->glsl_version = reinterpret_cast<const char *>(glGetString(GL_SHADING_LANGUAGE_VERSION));

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &adapter_info->max_texture_2d_size);
    glGetIntegerv(GL_MAX_3D_TEXTURE_SIZE, &adapter_info->max_texture_3d_size);
    glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &adapter_info->max_uniform_buffer_bindings);
    glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &adapter_info->max_uniform_block_size);
    glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT, &adapter_info->max_uniform_buffer_offset_alignment);

    // Get all supported extensions
    GLint num_extensions = 0;
    std::vector<std::string> extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &num_extensions);
    for (int i = 0; i < num_extensions; ++i) {
        extensions.emplace_back(reinterpret_cast<const char *>(glGetStringi(GL_EXTENSIONS, i)));
    }

    return adapter_info;
}

RedRHIBuffer *RedOpenGLDevice::CreateBuffer(
    RedRHIBufferUsage _usage,
    RedRHIMemoryType _memory_type,
    size_t _size,
    void *_data
) {
    auto buffer = new RedOpenGLBuffer{};

    buffer->usage = _usage;
    buffer->memory_type = _memory_type;
    buffer->size = _size;

    glGenBuffers(1, &buffer->gl_buffer);

    switch (_usage) {
        case RED_RHI_BUFFER_USAGE_VERTEX:
            glBindBuffer(GL_ARRAY_BUFFER, buffer->gl_buffer);
            glBufferData(GL_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            break;

        case RED_RHI_BUFFER_USAGE_INDEX:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->gl_buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, _size, _data, GL_STATIC_DRAW);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
            break;

        case RED_RHI_BUFFER_USAGE_UNIFORM:
            break;

        case RED_RHI_BUFFER_USAGE_STORAGE:
            break;

        case RED_RHI_BUFFER_USAGE_STAGING:
            break;

        default:
            throw std::runtime_error("Invalid buffer usage");
    }

    resource_poll.Register(buffer);

    return buffer;
}

void RedOpenGLDevice::DestroyBuffer(RedRHIBuffer *_buffer) {
    glDeleteBuffers(1, &static_cast<RedOpenGLBuffer *>(_buffer)->gl_buffer);
    resource_poll.SafeRelease(_buffer);
}

void RedOpenGLDevice::BindBuffer(RedRHIBuffer *_buffer) {
    auto buffer = static_cast<RedOpenGLBuffer *>(_buffer);

    switch (buffer->usage) {
        case RED_RHI_BUFFER_USAGE_VERTEX: {
            if (!bound_pipeline) { break; }
            auto &attrs = bound_pipeline->desc.vertex_input_desc.attributes;

            for (auto &attr: attrs) {
                if (attr.size > 0) {
                    glBindVertexBuffer(
                        attr.index,
                        buffer->gl_buffer,
                        0,
                        attr.stride
                    );
                    break;
                }
            }
            break;
        }

        case RED_RHI_BUFFER_USAGE_INDEX:
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->gl_buffer);
            break;

        case RED_RHI_BUFFER_USAGE_UNIFORM:
            break;

        case RED_RHI_BUFFER_USAGE_STORAGE:
            break;

        case RED_RHI_BUFFER_USAGE_STAGING:
            break;

        default:
            throw std::runtime_error("Invalid buffer usage");
    }
}

RedRHITexture *RedOpenGLDevice::CreateTexture(
    RedRHITextureFormat _usage,
    RedRHITextureSamplerType _sampler_type,
    RedRHITextureAddressType _address_type,
    int32_t _width,
    int32_t _height,
    int32_t _mip_levels
) {
    auto texture = new RedRHITexture{};

    texture->usage = _usage;
    texture->address_type = _address_type;
    texture->width = _width;
    texture->height = _height;
    texture->mip_levels = _mip_levels;

    resource_poll.Register(texture);

    return texture;
}

RedRHIShader *RedOpenGLDevice::CreateShader(std::string &_src, RedRHIShaderStage _stage) {
    auto shader = new RedOpenGLShader();

    shader->stage = _stage;
    shader->src = _src;

    unsigned int temp_shader = 0;
    switch (_stage) {
        case RED_RHI_SHADER_STAGE_VERTEX:
            temp_shader = glCreateShader(GL_VERTEX_SHADER);
            break;

        case RED_RHI_SHADER_STAGE_FRAGMENT:
            temp_shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;

        case RED_RHI_SHADER_STAGE_COMPUTE:
            temp_shader = glCreateShader(GL_COMPUTE_SHADER);
            break;

        default:
            break;
    }

    const char *temp_shader_source = shader->src.c_str();
    glShaderSource(temp_shader, 1, &temp_shader_source, nullptr);
    glCompileShader(temp_shader);

    int success;
    char info_log[512];
    glGetShaderiv(temp_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(temp_shader, 512, nullptr, info_log);
        std::cout << "Error compile shader" << info_log << std::endl;
    }

    shader->gl_shader = temp_shader;

    shader->is_final_program = false;

    return shader;
}

RedRHIShader *RedOpenGLDevice::CreateShader(RedRHIShader *_vertex_shader, RedRHIShader *_fragment_shader) {
    auto shader = new RedOpenGLShader();

    shader->gl_shader = glCreateProgram();

    glAttachShader(shader->gl_shader, ((RedOpenGLShader *) _vertex_shader)->gl_shader);
    glAttachShader(shader->gl_shader, ((RedOpenGLShader *) _fragment_shader)->gl_shader);

    glLinkProgram(shader->gl_shader);

    int success;
    char info_log[512];
    glGetProgramiv(shader->gl_shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader->gl_shader, 512, nullptr, info_log);
        std::cout << "Error linking program" << info_log << std::endl;
    }

    DestroyShader(_vertex_shader);
    DestroyShader(_fragment_shader);

    shader->is_final_program = true;

    resource_poll.Register(shader);

    return shader;
}

void RedOpenGLDevice::DestroyShader(RedRHIShader *_shader) {
    glDeleteShader(static_cast<RedOpenGLShader *>(_shader)->gl_shader);
    resource_poll.SafeRelease(_shader);
}

RedRHIPipeline *RedOpenGLDevice::CreatePipeline(RedRHIPipelineDesc _desc) {
    auto pipeline = new RedOpenGLPipeline();

    pipeline->desc = _desc;

    glGenVertexArrays(1, &pipeline->gl_vao);
    glBindVertexArray(pipeline->gl_vao);
    for (auto &item: _desc.vertex_input_desc.attributes) {
        glEnableVertexAttribArray(item.index);
        GLenum gl_type = MapFormatType(item.type);
        /*glVertexAttribPointer(
            item.index,
            item.size,
            gl_type,
            item.normalized,
            item.stride,
            reinterpret_cast<void *>(item.offset)
        );*/

        glVertexAttribFormat(
            item.index,
            item.size,
            gl_type,
            item.normalized,
            item.offset
        );
        glVertexAttribBinding(
            item.index,
            item.index
        );
        glEnableVertexAttribArray(item.index);
    }
    glBindVertexArray(0);

    resource_poll.Register(pipeline);

    return pipeline;
}

void RedOpenGLDevice::DestroyPipeline(RedRHIPipeline *_pipeline) {
    auto pipeline = static_cast<RedOpenGLPipeline *>(_pipeline);
    DestroyShader(pipeline->desc.shader);
    glDeleteVertexArrays(1, &pipeline->gl_vao);
    resource_poll.SafeRelease(_pipeline);
}

void RedOpenGLDevice::BindPipeline(RedRHIPipeline *_pipeline) {
    if (_pipeline == nullptr) {
        bound_pipeline = nullptr;
        glBindVertexArray(0);
        return;
    }
    bound_pipeline = static_cast<RedOpenGLPipeline *>(_pipeline);

    auto pipeline = static_cast<RedOpenGLPipeline *>(_pipeline);
    auto desc = pipeline->desc;
    auto shader = static_cast<RedOpenGLShader *>(desc.shader);

    glUseProgram(shader->gl_shader);

    glBindVertexArray(pipeline->gl_vao);

    switch (desc.rasterizer_desc.cull_mode) {
        case RED_RHI_CULL_MODE_NONE:
            glDisable(GL_CULL_FACE);
            break;

        case RED_RHI_CULL_MODE_FRONT:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT);
            break;

        case RED_RHI_CULL_MODE_BACK:
            glEnable(GL_CULL_FACE);
            glCullFace(GL_BACK);
            break;

        default:
            throw std::runtime_error("Undefined cull mode");
    }

    if (desc.depth_stencil_desc.depth_test) {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(desc.depth_stencil_desc.depth_write ? GL_TRUE : GL_FALSE);
        glDepthFunc(MapCompareOp(desc.depth_stencil_desc.compare_op));
    } else {
        glDisable(GL_DEPTH_TEST);
    }

    if (desc.blend_desc.blend_enable) {
        glEnable(GL_BLEND);
        glBlendFuncSeparate(
            MapBlendFactor(desc.blend_desc.src_color_blend_factor),
            MapBlendFactor(desc.blend_desc.dst_color_blend_factor),
            MapBlendFactor(desc.blend_desc.src_alpha_blend_factor),
            MapBlendFactor(desc.blend_desc.dst_color_blend_factor)
        );
    } else {
        glDisable(GL_BLEND);
    }
}

void RedOpenGLDevice::ClearColor(float _red, float _green, float _blue) {
    glClearColor(_red, _green, _blue, 1.0);
}

void RedOpenGLDevice::ClearFrameBuffer() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLenum RedOpenGLDevice::MapFormatType(RedRHIFormatType _type) {
    GLenum gl_type{};

    switch (_type) {
        case RED_RHI_FORMAT_TYPE_FLOAT:
            gl_type = GL_FLOAT;
            break;

        case RED_RHI_FORMAT_TYPE_DOUBLE:
            gl_type = GL_DOUBLE;
            break;

        case RED_RHI_FORMAT_TYPE_INT32:
            gl_type = GL_INT;
            break;

        case RED_RHI_FORMAT_TYPE_INT16:
            gl_type = GL_SHORT;
            break;

        case RED_RHI_FORMAT_TYPE_UINT32:
            gl_type = GL_UNSIGNED_INT;
            break;

        case RED_RHI_FORMAT_TYPE_UINT16:
            gl_type = GL_UNSIGNED_SHORT;
            break;

        default:
            throw std::runtime_error("Unknown format type");
    }

    return gl_type;
}

GLenum RedOpenGLDevice::MapCompareOp(RedRHICompareOp _compare_op) {
    GLenum op_type{};

    switch (_compare_op) {
        case RED_RHI_COMPARE_OP_NEVER:
            op_type = GL_NEVER;
            break;

        case RED_RHI_COMPARE_OP_LESS:
            op_type = GL_LESS;
            break;

        case RED_RHI_COMPARE_OP_EQUAL:
            op_type = GL_EQUAL;
            break;

        case RED_RHI_COMPARE_OP_GREATER:
            op_type = GL_GREATER;
            break;

        case RED_RHI_COMPARE_OP_NOT_EQUAL:
            op_type = GL_NOTEQUAL;
            break;

        case RED_RHI_COMPARE_OP_ALWAYS:
            op_type = GL_ALWAYS;
            break;

        default:
            throw std::runtime_error("Unknown compare operate");
    }

    return op_type;
}

GLenum RedOpenGLDevice::MapBlendFactor(RedRHIBlendFactor _blend_factor) {
    GLenum blend_factor{};

    switch (_blend_factor) {
        case RED_RHI_BLEND_FACTOR_ZERO:
            blend_factor = GL_ZERO;
            break;

        case RED_RHI_BLEND_FACTOR_ONE:
            blend_factor = GL_ONE;

            break;

        case RED_RHI_BLEND_FACTOR_SRC_ALPHA:
            blend_factor = GL_SRC_ALPHA;
            break;

        case RED_RHI_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA:
            blend_factor = GL_ONE_MINUS_SRC_ALPHA;
            break;

        default:
            throw std::runtime_error("Unknown blend factor");
    }

    return blend_factor;
}
