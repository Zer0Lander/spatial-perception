#pragma once

#include <cstdint>
#include <string>

struct Vec3 {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
};

struct Quaternion {
    double x = 0.0;
    double y = 0.0;
    double z = 0.0;
    double w = 1.0;
};

struct VioState {
    std::uint64_t timestamp_ns = 0;

    Vec3 position;
    Quaternion orientation;

    Vec3 linear_velocity;
    Vec3 angular_velocity;

    bool pose_valid = false;
    int pose_confidence = 0;

    std::string tracking_state;
};
