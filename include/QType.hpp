#pragma once

struct QMat {
	unsigned char* data;
	int width;
	int height;
	int channel;
};

struct QPoint {
	int x;
	int y;
};

struct QRect {
	int x;
	int y;
	int width;
	int height;
};

struct QBox {
	QRect rect;
	int cls;
	float conf;
	int index;
	float angle;
};

enum ImageFormat : unsigned int {
	PIX_FORMAT_BGR = 0,
	PIX_FORMAT_NV21 = 1,
	PIX_FORMAT_GRAY = 2,
};