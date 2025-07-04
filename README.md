！ 坐标系的关系   
The cordinate of velodyne LiDAR:   

<img src="https://github.com/wykxwyc/LeGO-LOAM_NOTED/blob/master/velodyne_LiDAR_cordinate.jpg" width = 80% height = 80%/>   


The cordinate of frame-of-integrated_to_init:    

<img src="https://github.com/wykxwyc/LeGO-LOAM_NOTED/blob/master/frame-of-integrated_to_init.jpg" width = 80% height = 80%/>   

详细编译：看相关的教程（这里仅仅更新版本的差异）
- v1 2025年7月3日18：12； 第一次上传为，ros-noteic ubuntu 20.04的 正常编译运行，点云正常显示
    - 若只有坐标系显示，可能为rosbag play 时，最后没有加 --clock 
- v2 2025年7月3日20：57； 第二次上传为，输出ros.bag最后的pcd输出文件；
    - pcd 文件保存在 results，修改保存地址，在src/LeGO-LOAM/include/utility.h中修改:fileDirectory = "/home/zyh/Desktop/LeGO-LOAM/results/";
    - pcd 文件的保存，注意事项：必须在rviz中，订阅map cloud(已经修改text.rviz,默认是订阅状态)
    - 保存的pcd有4个文件：具体含义，问AI 
- v3 2025年7月4日 15：18；第三次上传为，轨迹输出带有时间戳的pcd和txt文件，以及kitti格式的txt文件，方便使用evo的评估
    - 默认代码 为输出 kitti 个时的txt轨迹文件：all_odom_poses_kitti.txt以及 all_poses_kitti.txt
    - odom的txt文件为，前段里程计的每帧雷达轨迹输出；（误差较大）
    - pose的txt文件为，后端优化后的关键帧的轨迹输出；（准确，但是受系统限制，最少的间隔只能0.2s，所以帧数会少一些）关于间隔的时长更改在utility.h中修改：mappingProcessInterval = 0.02;  // 建图处理间隔（秒）
    - 输出的kitti_txt文件,不容易主观分析，自行利用AI修改代码
    - pose.txt的时间戳，为unix时间戳，可以使用分析时间戳的脚本：script/timestamp_analysis.py，转换成可读时间



