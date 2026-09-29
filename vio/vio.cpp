#include "zed_vio.hpp"
#include <iostream>

int main() {
    ZedVio vio;

    if (!vio.open()) {
        std::cerr << "Failed to initialize ZED VIO" << std::endl;
        return 1;
    }

    VioState state;

    while (true) {
        if (vio.read(state)) {
            std::cout
                << state.timestamp_ns << ","
                << state.position.x << ","
                << state.position.y << ","
                << state.position.z << ","
                << state.orientation.x << ","
                << state.orientation.y << ","
                << state.orientation.z << ","
                << state.orientation.w << ","
                << state.linear_velocity.x << ","
                << state.linear_velocity.y << ","
                << state.linear_velocity.z << ","
                << state.angular_velocity.x << ","
                << state.angular_velocity.y << ","
                << state.angular_velocity.z << ","
                << state.tracking_state
                << std::endl;
        }
    }
}