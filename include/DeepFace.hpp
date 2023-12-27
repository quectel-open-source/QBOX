#ifndef DEEPFACE_Y
#define DEEPFACE_Y

#include "json/json.h"

#include "FaceType.hpp"

typedef void *YHandle;

/**
 * 功能：初始化人脸操作handle（由APP调用）
 * 参数描述：
 *  【IN】
 *      handle : 人脸操作的handle
 *      json_path : json配置文件路径
 *      model_ptr : 模型起始地址
 *      model_size: 模型长度
 * 返回值：:
 *   0: 初始化成功
 *   -1: 模型路径不存在
 *   -2: handle初始化失败
 */
int InitDeepFace(YHandle &handle, const std::string& json_path);
int InitDeepFace(YHandle &handle, const Json::Value& config);

int UnInitDeepFace(YHandle &handle);

/**
 * 功能：执行人脸检测和lms检测，并做人脸质量判定（由APP调用）
 * 参数描述：
 *  【IN】
 *      handle : 人脸操作的handle
 *      img_data ：输入图像data，原图
 *      image_height ：图像高度
 *      image_width  ：图像宽度
 *      image_channel ：图像通道数
 *      format ：图像格式：[PIX_FORMAT_BGR, PIX_FORMAT_NV21, PIX_FORMAT_GRAY]
 *  【OUT】
 *      deepfaceResult ：人脸信息，包含所有模块的检测结果
 * 返回值：:
 *   0: 未检测到人脸
 *   1: 检测到有人脸
 *   2: 人脸角度异常
 *   3: 人脸大小异常
 *   4: 人脸检测置信度异常
 *   5: 人脸出界异常
 *   6: 图像质量异常
 */
int DetFace(YHandle handle, unsigned char* img_data, const int image_height, const int image_weight, const int image_channel, DeepFaceResult &deepfaceResult);

/**
 * 功能：提取人脸特征，该方法在DetFace后调用（由APP调用）
 * 参数描述：
 *  【IN】
 *      handle : 人脸操作的handle
 *  【OUT】
 *      deepfaceResult ：人脸信息，包含所有模块的检测结果，内含人脸特征
 * 返回值：: [0: 成功, -1: 失败]
 */
int GetFaceFeature(YHandle handle, DeepFaceResult& deepfaceResult);

/**
 * 功能：人脸1:N特征比对（由APP调用）
 * 参数描述：
 *  【IN】
 *      feature_gallery : 特征底库，N个人脸特征拼接而成
 *      length          : 底库数量
 *      feature_probe   : 待匹配人脸
 *      threshold       : 匹配阈值
 *  【OUT】
 *      face_id         : 匹配到的id
 *      score           : 匹配置信度
 * 返回值：: [0: 成功, -1: 失败]
 */
int FeatureCompare_org(float* feature_gallery, int length, float* feature_probe, float threshold, int& face_id, float& score);

const char* getDeepFaceVersion();

#endif
