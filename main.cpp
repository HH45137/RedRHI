#include <iostream>
#include "RedRHI/RedRHI.h"

int main() {
    RedRHIDevice *device = new RedOpenGLDevice();

    auto device_rhi = device->Init(0);
    auto adapter_info = device->GetAdapterInfo();
    auto texture_01 = device->CreateTexture(
        RED_RHI_TEXTURE_FORMAT_RGB_8,
        RED_RHI_TEXTURE_SAMPLER_TYPE_LINEAR,
        RED_RHI_TEXTURE_ADDRESS_TYPE_REPEAT,
        512,
        512,
        4
    );
    auto buffer_01 = device->CreateBuffer(
        RED_RHI_BUFFER_USAGE_VERTEX,
        RED_RHI_MEMORY_TYPE_DEVICE,
        1024,
        8
    );

    return 0;
}
