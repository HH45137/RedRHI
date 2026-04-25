#pragma once

#include <uuid.h>

#define GEN_UUID_STR uuids::to_string(uuids::uuid_system_generator{}())
