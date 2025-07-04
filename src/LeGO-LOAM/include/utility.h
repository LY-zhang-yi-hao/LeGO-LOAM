/**
 * @file utility.h
 * @brief LeGO-LOAM 工具类头文件 - 包含所有全局配置参数和数据结构定义
 * @author LeGO-LOAM Team (修改版本)
 * @date 2025
 *
 * 本文件定义了LeGO-LOAM系统的所有核心参数，包括：
 * - 雷达传感器配置参数
 * - 特征提取参数
 * - 建图优化参数
 * - 自定义点云数据结构
 */

#pragma once

// ========== ROS相关头文件 ==========
#include <ros/ros.h>                    // ROS核心功能
#include <sensor_msgs/Imu.h>            // IMU消息类型
#include <sensor_msgs/PointCloud2.h>    // 点云消息类型
#include <nav_msgs/Odometry.h>          // 里程计消息类型

#include "cloud_msgs/cloud_info.h"      // 自定义点云信息消息

// ========== OpenCV相关头文件 ==========
#include <opencv2/imgproc.hpp>          // 图像处理功能

// ========== PCL点云库相关头文件 ==========
#include <pcl/point_cloud.h>            // 点云基础类
#include <pcl/point_types.h>            // 点云数据类型
#include <pcl_ros/point_cloud.h>        // ROS-PCL转换
#include <pcl_conversions/pcl_conversions.h>  // PCL消息转换
#include <pcl/range_image/range_image.h>      // 距离图像
#include <pcl/filters/filter.h>              // 点云滤波
#include <pcl/filters/voxel_grid.h>          // 体素网格滤波
#include <pcl/kdtree/kdtree_flann.h>         // KD树搜索
#include <pcl/common/common.h>               // PCL通用功能
#include <pcl/registration/icp.h>            // ICP配准算法

// ========== TF变换相关头文件 ==========
#include <tf/transform_broadcaster.h>    // TF变换发布
#include <tf/transform_datatypes.h>      // TF数据类型

// ========== C++标准库头文件 ==========
#include <vector>       // 动态数组
#include <cmath>        // 数学函数
#include <algorithm>    // 算法库
#include <queue>        // 队列容器
#include <deque>        // 双端队列
#include <iostream>     // 输入输出流
#include <fstream>      // 文件流
#include <ctime>        // 时间处理
#include <cfloat>       // 浮点数限制
#include <iterator>     // 迭代器
#include <sstream>      // 字符串流
#include <string>       // 字符串类
#include <limits>       // 数值限制
#include <iomanip>      // 输入输出格式控制
#include <array>        // 固定数组
#include <thread>       // 多线程
#include <mutex>        // 互斥锁

// ========== 数学常量定义 ==========
#define PI 3.14159265                   // 圆周率常量

using namespace std;

// ========== 点云数据类型定义 ==========
typedef pcl::PointXYZI PointType;       // 定义基础点云类型：包含XYZ坐标和强度信息

// ========== ROS话题名称配置 ==========
extern const string pointCloudTopic = "/velodyne_points";  // 雷达点云话题名称
extern const string imuTopic = "/imu/data";                // IMU数据话题名称

// ========== 文件保存路径配置 ==========
extern const string fileDirectory = "/home/zyh/Desktop/LeGO-LOAM/results/";  // PCD文件保存目录

// ========== 雷达数据处理配置 ==========
// 使用Velodyne点云的"ring"通道进行图像投影（其他雷达可能有不同的通道名称，需要修改下面的"PointXYZIR"）
extern const bool useCloudRing = true;           // 如果为true，则不使用ang_res_y和ang_bottom参数

// ========== VLP-16 雷达传感器参数配置 ==========
extern const int N_SCAN = 16;                    // 雷达扫描线数（垂直方向）
extern const int Horizon_SCAN = 1800;            // 水平扫描点数（每条扫描线的点数）
extern const float ang_res_x = 0.2;              // 水平角度分辨率（度）
extern const float ang_res_y = 2.0;              // 垂直角度分辨率（度）
extern const float ang_bottom = 15.0 + 0.1;      // 雷达最低扫描角度（度）
extern const int groundScanInd = 7;              // 地面点扫描线索引（用于地面分割）

// ========== 其他雷达传感器参数配置（注释掉的备选配置） ==========

// HDL-32E 雷达配置
// extern const int N_SCAN = 32;                    // 32线雷达
// extern const int Horizon_SCAN = 1800;            // 水平扫描点数
// extern const float ang_res_x = 360.0/float(Horizon_SCAN);  // 水平角度分辨率
// extern const float ang_res_y = 41.33/float(N_SCAN-1);      // 垂直角度分辨率
// extern const float ang_bottom = 30.67;           // 最低扫描角度
// extern const int groundScanInd = 20;             // 地面点扫描线索引

// VLS-128 雷达配置
// extern const int N_SCAN = 128;                   // 128线雷达
// extern const int Horizon_SCAN = 1800;            // 水平扫描点数
// extern const float ang_res_x = 0.2;              // 水平角度分辨率
// extern const float ang_res_y = 0.3;              // 垂直角度分辨率
// extern const float ang_bottom = 25.0;            // 最低扫描角度
// extern const int groundScanInd = 10;             // 地面点扫描线索引

// Ouster雷达用户可能需要取消注释imageProjection.cpp中的第159行
// Ouster IMU数据的使用尚未完全支持（LeGO-LOAM需要9自由度IMU），请仅发布点云数据

// Ouster OS1-16 雷达配置
// extern const int N_SCAN = 16;                    // 16线雷达
// extern const int Horizon_SCAN = 1024;            // 水平扫描点数
// extern const float ang_res_x = 360.0/float(Horizon_SCAN);  // 水平角度分辨率
// extern const float ang_res_y = 33.2/float(N_SCAN-1);       // 垂直角度分辨率
// extern const float ang_bottom = 16.6+0.1;        // 最低扫描角度
// extern const int groundScanInd = 7;              // 地面点扫描线索引

// Ouster OS1-64 雷达配置
// extern const int N_SCAN = 64;                    // 64线雷达
// extern const int Horizon_SCAN = 1024;            // 水平扫描点数
// extern const float ang_res_x = 360.0/float(Horizon_SCAN);  // 水平角度分辨率
// extern const float ang_res_y = 33.2/float(N_SCAN-1);       // 垂直角度分辨率
// extern const float ang_bottom = 16.6+0.1;        // 最低扫描角度
// extern const int groundScanInd = 15;             // 地面点扫描线索引

// ========== SLAM算法核心参数配置 ==========
extern const bool loopClosureEnableFlag = false;     // 回环检测使能标志（false=关闭回环检测）
extern const double mappingProcessInterval = 0.01;  // 建图处理间隔（秒）- 处理每一帧（从0.3降低）

// ========== 传感器时序参数配置 ==========
extern const float scanPeriod = 0.1;                 // 雷达扫描周期（秒）- 10Hz
extern const int systemDelay = 0;                    // 系统延迟（帧数）
extern const int imuQueLength = 200;                 // IMU数据队列长度

// ========== 传感器数据过滤参数 ==========
extern const float sensorMinimumRange = 1.0;         // 传感器最小有效距离（米）
extern const float sensorMountAngle = 0.0;               // 传感器安装角度（弧度）
extern const float segmentTheta = 60.0 / 180.0 * M_PI;  // 地面分割角度阈值（降低此值可能提高精度）
extern const int segmentValidPointNum = 5;              // 有效分割点数量阈值
extern const int segmentValidLineNum = 3;               // 有效分割线数量阈值
extern const float segmentAlphaX = ang_res_x / 180.0 * M_PI;  // X方向分割角度分辨率（弧度）
extern const float segmentAlphaY = ang_res_y / 180.0 * M_PI;  // Y方向分割角度分辨率（弧度）

// ========== 特征提取参数配置 ==========
extern const int edgeFeatureNum = 2;                    // 每个扇区提取的边缘特征点数量
extern const int surfFeatureNum = 4;                    // 每个扇区提取的平面特征点数量
extern const int sectionsTotal = 6;                     // 扫描线分割的扇区总数
extern const float edgeThreshold = 0.1;                 // 边缘特征阈值
extern const float surfThreshold = 0.1;                 // 平面特征阈值
extern const float nearestFeatureSearchSqDist = 25;     // 最近特征搜索距离的平方（米²）

// ========== 建图优化参数配置 ==========
extern const float surroundingKeyframeSearchRadius = 50.0;  // 周围关键帧搜索半径（米）- 用于scan-to-map优化（回环关闭时）
extern const int surroundingKeyframeSearchNum = 50;         // 子地图大小（回环开启时）
// 历史关键帧参数（用于回环检测的历史子地图）
extern const float historyKeyframeSearchRadius = 7.0;       // 历史关键帧搜索半径（米）- 用于回环检测
extern const int historyKeyframeSearchNum = 25;             // 2n+1个历史关键帧将融合成子地图用于回环检测
extern const float historyKeyframeFitnessScore = 0.3;       // 历史关键帧匹配得分阈值（越小对齐越好）

// ========== 可视化参数配置 ==========
extern const float globalMapVisualizationSearchRadius = 500.0;  // 全局地图可视化搜索半径（米）

// ========== 自定义数据结构定义 ==========

/**
 * @brief 平滑度结构体 - 用于特征提取中的点云平滑度计算
 */
struct smoothness_t
{
    float value;        // 平滑度数值
    size_t ind;         // 点的索引
};

/**
 * @brief 平滑度比较器 - 用于按平滑度值排序
 */
struct by_value
{
    bool operator()(smoothness_t const &left, smoothness_t const &right)
    {
        return left.value < right.value;  // 按平滑度值升序排列
    }
};

/**
 * @brief 带有"ring"通道的点云类型
 * 用于存储包含扫描线信息的点云数据（Velodyne等雷达特有）
 */
struct PointXYZIR
{
    PCL_ADD_POINT4D                    // 添加x,y,z,w坐标（w通常为1）
    PCL_ADD_INTENSITY;                 // 添加强度信息
    uint16_t ring;                     // 扫描线编号（ring channel）
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW    // Eigen内存对齐
} EIGEN_ALIGN16;                       // 16字节对齐

// 向PCL注册自定义点云类型
POINT_CLOUD_REGISTER_POINT_STRUCT(PointXYZIR,
                                  (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)(uint16_t, ring, ring))

/**
 * @brief 包含6D位姿信息的点云类型
 * 用于存储机器人的完整位姿信息：[x,y,z,roll,pitch,yaw]
 * intensity字段用作时间戳或索引
 */
struct PointXYZIRPYT
{
    PCL_ADD_POINT4D                    // 添加x,y,z,w坐标
    PCL_ADD_INTENSITY;                 // 添加强度信息（用作时间戳或索引）
    float roll;                        // 翻滚角（绕X轴旋转）
    float pitch;                       // 俯仰角（绕Y轴旋转）
    float yaw;                         // 偏航角（绕Z轴旋转）
    double time;                       // 精确时间戳
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW    // Eigen内存对齐
} EIGEN_ALIGN16;                       // 16字节对齐

// 向PCL注册6D位姿点云类型
POINT_CLOUD_REGISTER_POINT_STRUCT(PointXYZIRPYT,
                                  (float, x, x)(float, y, y)(float, z, z)(float, intensity, intensity)
                                  (float, roll, roll)(float, pitch, pitch)(float, yaw, yaw)(double, time, time))

// ========== 类型别名定义 ==========
typedef PointXYZIRPYT PointTypePose;   // 位姿点类型别名，用于轨迹和关键帧存储

/**
 * @note 重要配置说明：
 * 1. mappingProcessInterval = 0.001 - 已修改为处理每一帧雷达数据（原值0.3秒）
 * 2. 当前配置适用于VLP-16雷达，如需使用其他雷达请取消对应注释并注释掉VLP-16配置
 * 3. 文件保存路径为 /home/zyh/Desktop/LeGO-LOAM/results/
 * 4. 回环检测已关闭（loopClosureEnableFlag = false）
 * 5. 自定义点云类型支持带ring信息的雷达数据和6D位姿存储
 */
