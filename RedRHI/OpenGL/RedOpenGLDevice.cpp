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
        glVertexAttribPointer(item.index, item.size, gl_type, item.normalized, item.stride, (void *) item.offset);
    }
    glBindVertexArray(0);

    resource_poll.Register(pipeline);

    return pipeline;
}

void RedOpenGLDevice::DestroyPipeline(RedRHIPipeline *_pipeline) {
    auto pipeline = static_cast<RedOpenGLPipeline *>(_pipeline);
    glDeleteVertexArrays(1, &pipeline->gl_vao);
    resource_poll.SafeRelease(_pipeline);
}

void RedOpenGLDevice::BindPipeline(RedRHIPipeline *_pipeline) {
    glBindVertexArray(static_cast<RedOpenGLPipeline *>(_pipeline)->gl_vao);
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
            break;
    }

    return gl_type;
}
