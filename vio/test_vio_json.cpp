#include "vio_json.hpp"

#include <nlohmann/json.hpp>

int main() {
    VioState state;
    state.timestamp_ns = 1234567890123456789ULL;
    state.position = {1.0, 2.0, 3.0};
    state.orientation = {0.1, 0.2, 0.3, 0.9};
    state.linear_velocity = {4.0, 5.0, 6.0};
    state.angular_velocity = {0.4, 0.5, 0.6};
    state.pose_valid = true;
    state.pose_confidence = 87;
    state.tracking_state = "OK";

    const auto message = nlohmann::json::parse(serializeVioState(state));

    const bool matches =
        message.at("schema_version") == 1 &&
        message.at("timestamp_ns") == state.timestamp_ns &&
        message.at("pose_valid") == state.pose_valid &&
        message.at("pose_confidence") == state.pose_confidence &&
        message.at("pose_frame") == "zed_world" &&
        message.at("twist_frame") == "zed_left_camera" &&
        message.at("tracking_state") == state.tracking_state &&
        message.at("position").at("x") == state.position.x &&
        message.at("position").at("y") == state.position.y &&
        message.at("position").at("z") == state.position.z &&
        message.at("orientation").at("x") == state.orientation.x &&
        message.at("orientation").at("y") == state.orientation.y &&
        message.at("orientation").at("z") == state.orientation.z &&
        message.at("orientation").at("w") == state.orientation.w &&
        message.at("linear_velocity").at("x") == state.linear_velocity.x &&
        message.at("linear_velocity").at("y") == state.linear_velocity.y &&
        message.at("linear_velocity").at("z") == state.linear_velocity.z &&
        message.at("angular_velocity").at("x") == state.angular_velocity.x &&
        message.at("angular_velocity").at("y") == state.angular_velocity.y &&
        message.at("angular_velocity").at("z") == state.angular_velocity.z;

    return matches ? 0 : 1;
}
