#pragma once

#include <fstream>
#include <uuid.h>

#define GEN_UUID_STR uuids::to_string(uuids::uuid_system_generator{}())


inline std::string ReadTextFile(const std::string &_filename) {
    std::ifstream file(_filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file " + _filename);
    }

    std::ostringstream oss;
    oss << file.rdbuf();

    return oss.str();
}

