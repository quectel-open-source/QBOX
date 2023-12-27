#pragma once

#include <vector>

#include "json/json.h"

#include "ObjectType.hpp"

int InitDeepPlate(void*& handle, const std::string& path);
int InitDeepPlate(void*& handle, const Json::Value& config);
int UnInitDeepPlate(void*& handle);

int DetPlate(void* handle, unsigned char* data, const int width, const int height, const int channel, std::vector<DeepPlateResult>& result_list);
