# QSF SLS Controller

## Install/Getting Started  
### 1. Install Ubuntu 20.04 and PX4-Autopilot v1.13.2
Install Ubuntu 20.04:
```
wsl --install -d Ubuntu-20.04
```
Install PX4-Autopilot v1.13.2:
```
git clone https://github.com/PX4/PX4-Autopilot.git --branch v1.13.2 --recursive
cd ~/PX4-Autopilot
git submodule update
cd ~
bash ./PX4-Autopilot/Tools/setup/ubuntu.sh
cd ~/PX4-Autopilot
pip uninstall empy
pip install empy==3.3.4
make px4_sitl gazebo
```

### 2. Install QGroundControl
Setup mirror network in your computer first: http://wiki/doku.php?id=wiki:ros:ros2_humble&s[]=wsl2

Then install QGroundControl on windows: https://docs.px4.io/main/en/dev_setup/dev_env_windows_wsl.html#qgroundcontrol

### 3. Install ROS Noetic & MAVROS(Source installation)  
Follow the guide at https://docs.px4.io/main/en/ros/mavros_installation.html  
### Troubleshooting
When install MAVROS, before create a catkin workspace, run
```
sudo apt-get install python3-catkin-tools python3-rosinstall-generator -y
```
If you see catkin build errors in step 5, see http://wiki/doku.php?id=wiki:uav_platforms:multirotor_platform:ancl_drones:px4vision_v1_5&s[]=mavros MAVROS Installation section for help and restart installation from step 1

### 4. Install the project
```
cd catkin_ws/src  
git clone https://github.com/ANCL/ECE664_2025.git 
cd ..
catkin build
source devel/setup.bash  # source the workspace
```
### Troubleshooting

- **Authentication Issues**:  
  If you encounter errors during cloning (e.g., password authentication), generate a [GitHub Personal Access Token](https://github.com/settings/tokens) and use it in place of your password when prompted.

- **Build Errors**:  
  If there are errors related to missing headers or dependencies in the `mrotor_controller` folder:
  1. Open and add the following in `package.xml`:
     ```xml
     <exec_depend>controller_msgs</exec_depend>
     ```

  2. Open and edit `CMakeLists.txt`:
     - Add `controller_msgs` in the `find_package()` section:
       ```cmake
       find_package(catkin REQUIRED COMPONENTS controller_msgs)
       ```
     - Add `controller_msgs` to the `catkin_package()` section:
       ```cmake
       catkin_package(CATKIN_DEPENDS controller_msgs)
       ```

  After making these changes, rebuild the workspace:
  ```bash
  cd catkin_ws
  catkin build
  source devel/setup.bash

### 5. Modify parameters
```
code ~/PX4-Autopilot/build/px4_sitl_default/etc/init.d/airframes/4016_holybro_px4vision
```
Find and comment the following:
```
param set-default SYS_USE_IO 0
param set-default MAV_0_CONFIG 101
param set-default MAV_1_CONFIG 102
param set-default SER_TEL1_BAUD 921600
param set-default MPC_Z_TRAJ_P 0.3
param set-default SENS_CM8JL65_CFG 104
param set-default SENS_EN_PMW3901 1
param set-default BAT1_A_PER_V 36.364
param set-default BAT1_V_DIV 18.182
```
Save the changes. Then rebuild PX4
```
cd ~/PX4-Autopilot
make px4_sitl_default
```

### 6. Modify /.bashrc
```
code ~/.bashrc
```  
add following contents:
```
source /opt/ros/noetic/setup.bash
source ~/catkin_ws/devel/setup.bash
source ~/PX4-Autopilot/Tools/setup_gazebo.bash ~/PX4-Autopilot ~/PX4-Autopilot/build/px4_sitl_default

export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:~/PX4-Autopilot
export ROS_PACKAGE_PATH=$ROS_PACKAGE_PATH:~/PX4-Autopilot/Tools/simulation/gazebo-classic/sitl_gazebo-classic
export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:/usr/lib/x86_64-linux-gnu/gazebo-11/plugins
export GAZEBO_MODEL_PATH=$GAZEBO_MODEL_PATH:~/catkin_ws/src/ECE664_2025/controller_sitl_gazebo/models
```
## Run SLS SITL
### 1. Setup SITL environment
```
# in a new terminal
roslaunch mrotor_controller sitl_sls_empty_world.launch
```  
### 2. Run QGroundControl

### 3. Launch SITL controller launch script
```
# in a new terminal
roslaunch mrotor_controller drone1_sls_controller.launch
```
### 4. Open Dynamic Reconfigure GUI
```
# in a new terminal
rosrun rqt_reconfigure rqt_reconfigure
```
## Plot the Data
Plot the data you obtained using main_ccece25.m file in `MATLAB_Plot` folder by changing the name of the bag file in:
![image](https://github.com/user-attachments/assets/7584aca0-0345-45d9-8889-cc5074fbc41a)
