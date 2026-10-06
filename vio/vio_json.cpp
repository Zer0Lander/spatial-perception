#include "vio_json.hpp"

#include <nlohmann/json.hpp>

std::string serializeVioState(const VioState& state) {
    const nlohmann::json message = {
        {"schema_version", 1},
        {"timestamp_ns", state.timestamp_ns},
        {"pose_valid", state.pose_valid},
        {"pose_confidence", state.pose_confidence},
        {"pose_frame", "zed_world"},
        {"twist_frame", "zed_left_camera"},

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

    return message.dump();
}
