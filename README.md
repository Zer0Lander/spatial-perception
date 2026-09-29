# Spatial Perception

A C++ spatial perception stack for ZED-based visual-inertial odometry (VIO) and terrain traversability, designed for NVIDIA Jetson with Python and future ROS 2 integration.

## Current Status

Currently implemented:

- ZED 2 integration using the ZED SDK
- Visual-inertial odometry
- Position and quaternion orientation
- Linear and angular velocity
- Tracking state
- ZeroMQ transport from C++ to Python
- JSON-based VIO messages

## Target Architecture

    ZED 2
     ├── VIO
     │    └── position, orientation, velocity
     │
     └── Depth / Point Cloud
          └── terrain traversability

                 ↓

           Robot system
           Python / ROS 2

## Dependencies

- ZED SDK
- CUDA
- ZeroMQ
- nlohmann/json
- Python 3
- pyzmq

Additional packages:

    sudo apt install build-essential cmake pkg-config libzmq3-dev nlohmann-json3-dev python3-zmq

## Build

    cd vio
    mkdir build
    cd build
    cmake ..
    make -j$(nproc)

## Run

Start the VIO publisher:

    ./vio/build/vio

Start the Python subscriber:

    python3 vio/read_vio.py

## VIO Message

VIO data is published as JSON and contains:

- timestamp
- position `(x, y, z)`
- orientation quaternion `(x, y, z, w)`
- linear velocity `(x, y, z)`
- angular velocity `(x, y, z)`
- tracking state

## Planned

- Terrain traversability using ZED depth / point clouds
- ROS 2 integration
- Optional VSLAM / global localization