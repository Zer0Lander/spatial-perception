# Spatial Perception

A C++ spatial perception stack for a ZED 2 on NVIDIA Jetson. The current
component publishes visual-inertial odometry (VIO) to Python over ZeroMQ.
Terrain traversability, ROS 2 integration, and a separate VSLAM layer are
planned work.

## Current Status

The repository currently provides:

- ZED positional tracking with the SDK's default IMU fusion enabled
- area memory disabled for VIO-only operation
- position, quaternion orientation, and linear/angular velocity
- pose validity, pose confidence, and tracking state
- a C++ ZeroMQ PUB socket producing versioned JSON messages
- a validating Python ZeroMQ SUB example

## Target Architecture

```text
ZED 2
 ├── VIO core ── ZeroMQ / future ROS 2
 ├── future VSLAM layer
 └── future depth / point cloud ── terrain traversability
```

VSLAM is intentionally not enabled in the current VIO component. It can be
developed and evaluated separately without changing the VIO transport
contract.

## Dependencies

Install the ZED SDK and its matching CUDA version for the Jetson platform.
The remaining Ubuntu packages are:

```bash
sudo apt install build-essential cmake pkg-config libzmq3-dev \
    nlohmann-json3-dev python3-zmq
```

The build requires CMake 3.16 or newer, ZED SDK 5, and C++17.

## Build and Test

Run these commands from the repository root:

```bash
cmake -S vio -B vio/build -DCMAKE_BUILD_TYPE=Release
cmake --build vio/build --parallel
ctest --test-dir vio/build --output-on-failure
```

Release is the recommended configuration for runtime performance on Jetson.

## Run

Start the Python subscriber first:

```bash
python3 vio/read_vio.py
```

In another terminal, start the VIO publisher:

```bash
./vio/build/vio
```

Press Ctrl-C in either terminal for a clean shutdown.

The publisher binds `tcp://*:5555`; the example subscriber connects to
`tcp://127.0.0.1:5555`. These endpoints are currently fixed in the example
applications.

ZeroMQ PUB/SUB is a live, lossy stream. It provides no acknowledgement or
delivery guarantee, and messages sent before a subscriber finishes connecting
can be dropped. This is intentional for the current real-time VIO feed.

## VIO Message Contract

Each ZeroMQ message contains one JSON object using schema version 1:

| Field | Type | Meaning |
| --- | --- | --- |
| `schema_version` | integer | Currently `1` |
| `timestamp_ns` | non-negative integer | ZED pose timestamp for the grabbed image, in Unix-epoch nanoseconds |
| `pose_valid` | boolean | `true` only when the SDK marks the pose valid and tracking state is `OK` |
| `pose_confidence` | integer | SDK confidence from 0 to 100 |
| `pose_frame` | string | Always `zed_world` in schema version 1 |
| `twist_frame` | string | Always `zed_left_camera` in schema version 1 |
| `position` | object | Position `{x, y, z}` in metres |
| `orientation` | object | Quaternion `{x, y, z, w}` |
| `linear_velocity` | object | Velocity `{x, y, z}` in metres per second |
| `angular_velocity` | object | Angular velocity `{x, y, z}` in radians per second |
| `tracking_state` | string | ZED SDK positional-tracking state |

In schema version 1, `pose_valid` is the downstream validity gate for both pose
and twist. When it is false, consumers must not use those values for control or
estimation. They remain in the message for diagnostics, along with confidence
and tracking state.

### Coordinate and Frame Conventions

The ZED SDK is configured for
`RIGHT_HANDED_Z_UP_X_FWD` coordinates and metre units:

- X points forward, Y points left, and Z points up.
- `zed_world` is created when positional tracking starts. With the current SDK
  defaults, IMU gravity aligns roll and pitch; floor-origin alignment and
  area-memory relocalization are disabled.
- Position is the ZED left-camera origin expressed in `zed_world`.
- Orientation is the left-camera orientation in `zed_world`.
- Quaternion component order is `(x, y, z, w)`.
- Linear and angular velocity are expressed in `zed_left_camera`, following
  the ZED `Pose::twist` convention.

The frame names are transport identifiers. A future ROS 2 adapter will map
them to the chosen TF tree and apply any camera-to-robot extrinsic transform.

## Failure Behavior

- Camera-open and positional-tracking initialization failures terminate the
  publisher with the ZED SDK error text.
- Frame-grab failures are logged at most once per second while retrying.
- Tracking-state changes are logged with pose validity and confidence.
- ZeroMQ context, socket, bind, and send failures are reported.
- SIGINT and SIGTERM stop the main loop and release resources normally.

## Planned

- A separate VSLAM experiment that creates and inspects a small map
- Terrain traversability using ZED depth / point clouds
- ROS 2 integration
