### 🗺️ 坐标系关系

为了更好地理解算法，以下是本项目中关键的坐标系定义：

1.  **Velodyne LiDAR 坐标系**:
    <img src="https://github.com/wykxwyc/LeGO-LOAM_NOTED/blob/master/velodyne_LiDAR_cordinate.jpg" width="80%" />

2.  **`integrated_to_init` 坐标系**:
    <img src="https://github.com/wykxwyc/LeGO-LOAM_NOTED/blob/master/frame-of-integrated_to_init.jpg" width="80%" />

---

### 📝 版本更新与修改日志

这里记录了项目的主要修改和注意事项。详细的编译教程请参考其他文档，本节仅关注版本间的差异。

#### V1: 首次成功运行 ✅
*   **时间**: `2025-07-03 18:12`
*   **环境**: ROS Noetic + Ubuntu 20.04
*   **状态**: 成功编译并运行，点云地图在 Rviz 中正常显示。
*   **📌 注意**: 如果 Rviz 中只显示坐标系而没有点云，请检查 `rosbag play` 命令末尾是否添加了 `--clock` 参数。

#### V2: 增加 PCD 地图保存功能 💾
*   **时间**: `2025-07-03 20:57`
*   **新功能**: 程序结束时，可以将最终的点云地图保存为 `.pcd` 文件。
*   **文件路径**:
    *   默认保存位置: `results` 目录下。
    *   修改保存路径: 在 `src/LeGO-LOAM/include/utility.h` 文件中修改 `fileDirectory` 变量。
*   **📌 注意**: 必须在 Rviz 中订阅 `map_cloud` 话题才能触发保存。项目中的 `.rviz` 配置文件已默认订阅。
*   **💡 解读**: LeGO-LOAM 保存的4个PCD文件分别是用于位姿估计的四种不同类型的特征点：尖锐边缘点、次尖锐边缘点、平坦平面点和次平坦平面点。

#### V3: 增加轨迹导出功能 📈
*   **时间**: `2025-07-04 15:18`
*   **新功能**: 可输出带时间戳的轨迹文件（`.txt` 和 `.pcd` 格式），并支持 KITTI 数据集格式，方便使用 `evo` 等工具进行精度评估。
*   **输出文件**:
    *   `all_odom_poses_kitti.txt`: 前端激光雷达里程计的轨迹，帧间匹配，误差会随时间累积。
    *   `all_poses_kitti.txt`: 后端优化后的关键帧轨迹，更准确，但帧数较稀疏。
*   **参数调整**:
    *   后端优化的频率可在 `utility.h` 中修改 `mappingProcessInterval`。
*   **辅助脚本**:
    *   `pose.txt` 中的时间戳为 UNIX 格式，可以使用 `script/timestamp_analysis.py` 脚本将其转换为可读的日期时间格式。



