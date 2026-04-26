#pragma once

#include <cstdint>
#include <string>

#include "RedRHIBuffer.h"
#include "RedRHIUtility.h"


class RedRHIResourcePool {
public:
    template<typename T>
    T *Register(T *_resource) {
        std::lock_guard lock(this->mutex);

        resources[_resource->uuid] = _resource;
        ++_resource->ref_count;
        return _resource;
    }

    RedRHIResource *GetByUUID(const std::string& _uuid) {
        std::lock_guard lock(this->mutex);

        auto item = resources.find(_uuid);

        return item != resources.end() ? item->second : nullptr;
    }

    void AddRef(RedRHIResource *_resource) {
        std::lock_guard lock(this->mutex);

        ++_resource->ref_count;
    }

    void SafeRelease(RedRHIResource *_resource) {
        std::lock_guard lock(this->mutex);

        if (_resource->ref_count == 0) {
            resources.erase(_resource->uuid);
            delete _resource;
        }
    }

    void ForceRelease(RedRHIResource *_resource) {
        std::lock_guard lock(this->mutex);

        resources.erase(_resource->uuid);
        delete _resource;
    }

    void SafeReleaseAll() {
        for (const auto &item: resources) {
            SafeRelease(item.second);
        }
        resources.clear();
    }

    void ForceReleaseAll() {
        for (const auto &item: resources) {
            SafeRelease(item.second);
        }
        resources.clear();
    }

private:
    std::unordered_map<std::string, RedRHIResource *> resources;
    std::mutex mutex;
};
