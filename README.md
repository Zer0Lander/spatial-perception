# Spatial Perception

A C++ spatial perception stack for ZED-based visual-inertial odometry (VIO) and terrain traversability, designed to run on NVIDIA Jetson and integrate with Python and ROS 2.

## Current Status

The current implementation provides:

- ZED 2 camera integration using the ZED SDK
- Visual-inertial odometry
- Position and quaternion orientation
- Linear and angular velocity
- Tracking state
- Simple Python access to the VIO output

## Target Architecture

```text
ZED 2
 ├── VIO
 │    └── position, orientation, velocity
 │
 └── Depth / Point Cloud
      └── terrain traversability

             ↓

       Robot system
       Python / ROS 2