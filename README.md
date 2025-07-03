详细编译：看相关的教程（这里仅仅更新版本的差异）
- v1 第一次上传为，ros-noteic ubuntu 20.04的 正常编译运行，点云正常显示
    - 只有坐标系显示，可能为rosbag play 时，最后没有加 --clock 
- v2 第二次上传为，输出ros.bag最后的pcd输出文件；
    - pcd 文件保存在 results，修改保存地址，在src/LeGO-LOAM/include/utility.h中修改:fileDirectory = "/home/zyh/Desktop/LeGO-LOAM/results/";
    - pcd 文件的保存，注意事项：必须在rviz中，订阅map cloud(已经修改text.rviz,默认是订阅状态)
    - 保存的pcd有4个文件：具体含义，问AI 

