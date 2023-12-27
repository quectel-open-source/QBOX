#pragma once

#include <vector>

#include "json/json.h"

#include "ObjectType.hpp"

int InitPedestrian(void*& handle, const std::string& json_path);
int InitPedestrian(void*& handle, const Json::Value& config);
int UnInitPedestrian(void*& handle);

int DetPedestrian(void* handle, unsigned char* data, int width, int height, int channel, std::vector<ObjectResult>& result_list);
