#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
将LeGO-LOAM的txt轨迹文件转换为CSV格式
"""

import csv
import os
from datetime import datetime

def unix_to_readable(unix_timestamp):
    """将Unix时间戳转换为可读格式"""
    dt = datetime.fromtimestamp(unix_timestamp)
    return dt.strftime('%Y-%m-%d %H:%M:%S.%f')[:-3]  # 保留毫秒

def convert_txt_to_csv(input_file, output_file, add_readable_time=True):
    """
    将txt格式的轨迹文件转换为CSV格式
    
    Args:
        input_file: 输入的txt文件路径
        output_file: 输出的CSV文件路径
        add_readable_time: 是否添加可读时间列
    """
    
    if not os.path.exists(input_file):
        print(f"❌ 错误: 找不到文件 {input_file}")
        return False
    
    try:
        with open(input_file, 'r') as txt_file, open(output_file, 'w', newline='', encoding='utf-8') as csv_file:
            csv_writer = csv.writer(csv_file)
            
            # 写入CSV头部
            if add_readable_time:
                csv_writer.writerow(['timestamp_unix', 'timestamp_readable', 'x', 'y', 'z', 'roll', 'pitch', 'yaw'])
            else:
                csv_writer.writerow(['timestamp', 'x', 'y', 'z', 'roll', 'pitch', 'yaw'])
            
            line_count = 0
            data_count = 0
            
            for line in txt_file:
                line_count += 1
                line = line.strip()
                
                # 跳过注释行和空行
                if line.startswith('#') or not line:
                    continue
                
                # 分割数据
                parts = line.split()
                if len(parts) >= 7:  # 确保有足够的列
                    timestamp = float(parts[0])
                    x = float(parts[1])
                    y = float(parts[2])
                    z = float(parts[3])
                    roll = float(parts[4])
                    pitch = float(parts[5])
                    yaw = float(parts[6])
                    
                    if add_readable_time:
                        readable_time = unix_to_readable(timestamp)
                        csv_writer.writerow([timestamp, readable_time, x, y, z, roll, pitch, yaw])
                    else:
                        csv_writer.writerow([timestamp, x, y, z, roll, pitch, yaw])
                    
                    data_count += 1
                else:
                    print(f"⚠️  警告: 第{line_count}行数据格式不正确: {line}")
        
        print(f"✅ 转换成功!")
        print(f"   输入文件: {input_file}")
        print(f"   输出文件: {output_file}")
        print(f"   处理行数: {line_count}")
        print(f"   数据行数: {data_count}")
        return True
        
    except Exception as e:
        print(f"❌ 转换失败: {e}")
        return False

def main():
    print("🔄 LeGO-LOAM轨迹文件转换工具")
    print("=" * 50)
    
    # 定义文件路径
    files_to_convert = [
        ("results/all_poses.txt", "results/all_poses.csv"),
        ("results/all_odom_poses.txt", "results/all_odom_poses.csv")
    ]
    
    success_count = 0
    
    for input_file, output_file in files_to_convert:
        print(f"\n📄 转换文件: {input_file}")
        if convert_txt_to_csv(input_file, output_file, add_readable_time=True):
            success_count += 1
    
    print(f"\n🎉 转换完成! 成功转换 {success_count}/{len(files_to_convert)} 个文件")
    
    # 显示生成的CSV文件
    print("\n📊 生成的CSV文件:")
    for _, output_file in files_to_convert:
        if os.path.exists(output_file):
            file_size = os.path.getsize(output_file)
            print(f"   ✅ {output_file} ({file_size} bytes)")

if __name__ == "__main__":
    main()
