#include <iostream>

#include "zed_vio.hpp"
#include "zmq_publisher.hpp"

#include <nlohmann/json.hpp>

int main() {
    ZedVio vio;

    if (!vio.open()) {
        std::cerr << "Failed to initialize ZED VIO" << std::endl;
        return 1;
    }

    ZmqPublisher publisher;

    if (!publisher.open("tcp://*:5555")) {
        std::cerr
            << "Failed to start ZeroMQ publisher: "
            << publisher.lastError()
            << std::endl;

        return 1;
    }

    VioState state;

    while (true) {
        if (vio.read(state)) {
            nlohmann::json msg = {
                {"timestamp_ns", state.timestamp_ns},

                {"position", {
                    {"x", state.position.x},
                    {"y", state.position.y},
                    {"z", state.position.z}
                }},

                {"orientation", {
                    {"x", state.orientation.x},
                    {"y", state.orientation.y},
                    {"z", state.orientation.z},
                    {"w", state.orientation.w}
                }},

                {"linear_velocity", {
                    {"x", state.linear_velocity.x},
                    {"y", state.linear_velocity.y},
                    {"z", state.linear_velocity.z}
                }},

                {"angular_velocity", {
                    {"x", state.angular_velocity.x},
                    {"y", state.angular_velocity.y},
                    {"z", state.angular_velocity.z}
                }},

                {"tracking_state", state.tracking_state}
            };

            if (!publisher.publish(msg.dump())) {
                std::cerr
                    << "Failed to publish VIO message: "
                    << publisher.lastError()
                    << std::endl;
            }
        }
    }
}