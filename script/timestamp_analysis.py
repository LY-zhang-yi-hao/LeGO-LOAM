#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
分析LeGO-LOAM时间戳与原始雷达时间戳的差异
"""

from datetime import datetime
import pytz

def unix_to_readable(unix_timestamp):
    """将Unix时间戳转换为可读格式"""
    dt = datetime.fromtimestamp(unix_timestamp)
    return dt.strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]  # 保留毫秒

def readable_to_unix(readable_time):
    """将可读时间转换为Unix时间戳"""
    dt = datetime.strptime(readable_time, '%Y-%m-%d %H:%M:%S.%f')
    return dt.timestamp()

print("🕐 时间戳分析工具")
print("=" * 60)

# LeGO-LOAM的时间戳
# 读取原始文件的雷达时间戳
lego_timestamps = []
UnboundLocalErrorego_timestamp = []
try:
    with open('results/all_poses.txt', 'r') as f:
        for line in f:
            line = line.strip()
            # 跳过第一行（注释行）
            if line.startswith('#'):
                continue
            if line:  # 跳过空行
                timestamp = float(line.split()[0])  # 获取第一列时间戳
                lego_timestamps.append(timestamp)
    print(f"成功读取 {len(lego_timestamps)} 个时间戳")
except FileNotFoundError:
    print("错误: 找不到文件 results/all_poses.txt")
    lego_timestamps = []
except Exception as e:
    print(f"读取文件时出错: {e}")
    lego_timestamps = []

# 你提取的原始时间戳
original_timestamps = [
    "2025-07-01 11:05:59.502408",
    "2025-07-01 11:05:59.602346", 
    "2025-07-01 11:05:59.804363",
    "2025-07-01 11:05:59.905646",
    "2025-07-01 11:06:00.106494"
]

print("📊 LeGO-LOAM时间戳转换:")
print("-" * 40)
for i, ts in enumerate(lego_timestamps):
    readable = unix_to_readable(ts)
    print(f"{i+1}. {ts:.9f} -> {readable}")

print("\n📊 原始雷达时间戳转换:")
print("-" * 40)
for i, ts in enumerate(original_timestamps):
    unix_ts = readable_to_unix(ts)
    print(f"{i+1}. {ts} -> {unix_ts:.9f}")

print("\n🔍 差异分析:")
print("-" * 40)

# 检查LeGO-LOAM时间戳是否合理
lego_year = datetime.fromtimestamp(lego_timestamps[0]).year
print(f"LeGO-LOAM时间戳对应年份: {lego_year}")

# 计算时间间隔
print("\n⏱️  时间间隔分析:")
print("LeGO-LOAM时间间隔:")
for i in range(1, len(lego_timestamps)):
    interval = lego_timestamps[i] - lego_timestamps[i-1]
    print(f"  间隔 {i}: {interval:.3f}秒")

print("\n原始雷达时间间隔:")
for i in range(1, len(original_timestamps)):
    ts1 = readable_to_unix(original_timestamps[i-1])
    ts2 = readable_to_unix(original_timestamps[i])
    interval = ts2 - ts1
    print(f"  间隔 {i}: {interval:.3f}秒")

# 检查是否是ROS时间问题
print("\n🤔 可能的原因分析:")
print("1. ROS时间 vs 系统时间")
print("2. rosbag录制时的时间戳设置")
print("3. LeGO-LOAM使用的时间源")

# 检查2025年的合理性
current_year = datetime.now().year
if lego_year > current_year + 10:
    print(f"\n⚠️  警告: LeGO-LOAM时间戳显示{lego_year}年，这可能不正确！")
    print("   建议检查ROS时间设置和rosbag播放参数")
