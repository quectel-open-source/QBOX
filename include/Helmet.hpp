#pragma once

#include <vector>

#include "ObjectType.hpp"

extern "C" {

typedef void* YHandle;

int InitHelmet(YHandle &handle, const std::string json_path, const uint8_t* model_buffer, const size_t model_size);
int UnInitHelmet(YHandle &handle);

int DetHelmet(YHandle handle, unsigned char *srcData, int srcWidth, int srcHeight, int channel,
                std::vector<ObjectResult>& objResults);
}
