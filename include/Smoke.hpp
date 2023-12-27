#pragma once

#include <vector>

#include "ObjectType.hpp"

extern "C" {

typedef void* YHandle;

int InitSmoke(YHandle &handle, const std::string json_path, const uint8_t* model_buffer, const size_t model_size);
int UnInitSmoke(YHandle &handle);

int DetSmoke(YHandle handle, unsigned char *srcData, int srcWidth, int srcHeight, int channel,
                std::vector<ObjectResult>& objResults);
}
