#pragma once

#include <cstdint>
#include <string>

struct Vec3 {
    double x;
    double y;
    double z;
};

struct Quaternion {
    double x;
    double y;
    double z;
    double w;
};

struct VioState {
    std::uint64_t timestamp_ns;

    Vec3 position;
    Quaternion orientation;

    Vec3 linear_velocity;
    Vec3 angular_velocity;

    std::string tracking_state;
};