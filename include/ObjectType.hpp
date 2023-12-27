#pragma once

#include <string>

typedef struct ObjectResult {
	int label_id;       // 模型预测的类别
	std::string label;  // 模型预测的类别名称
	double score;       // 模型预测的概率值
	double angle;       // 最小外接矩形的角度
	double cx;          // 最小外接矩形的中心点 x
	double cy;          // 最小外接矩形的中心点 y
	int width;          // 最小外接矩形的长
	int height;         // 最小外接矩形的宽
	double area;        // 面积
} ObjectResult;

typedef struct DeepPlateResult {
	ObjectResult obj;
	std::string id;
} DeepPlateResult;