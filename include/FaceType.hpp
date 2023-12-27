#pragma once


#include <iostream>

struct v7Mat {
	unsigned char *data;
	int height;
	int width;
	int channel;
};

struct v7Point {
	int x;
	int y;
};

struct v7Rect {
	int x;
	int y;
	int width;
	int height;
};

struct v7Box {
	v7Rect rect;
	int cls;
	float conf;
	int index;
};

enum ImageFormat : uint32_t {
	PIX_FORMAT_BGR = 0,
	PIX_FORMAT_NV21 = 1,
	PIX_FORMAT_GRAY = 2,
};

typedef struct tagDeepFaceResult
{
	float facefeature[512];
	float facefeature_mask[512];
	v7Rect face_rt;
	float face_conf;
	float fyaw;
	float fpitch;
	float froll;
	float face_conf_l;
	float f_mask;
	v7Point face_point[21];
	unsigned char face_image[16384];
	unsigned char face_show_image[196608];
} DeepFaceResult;

