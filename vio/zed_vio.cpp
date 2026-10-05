#include "zed_vio.hpp"

bool ZedVio::open() {
    sl::InitParameters init;
    init.coordinate_units = sl::UNIT::METER;
    init.coordinate_system = sl::COORDINATE_SYSTEM::RIGHT_HANDED_Z_UP_X_FWD;

    auto err = zed_.open(init);

    if (err != sl::ERROR_CODE::SUCCESS) {
        last_error_ = "Failed to open ZED camera: ";
        last_error_ += sl::toString(err).c_str();
        return false;
    }

    sl::PositionalTrackingParameters tracking;
    tracking.enable_area_memory = false;

    err = zed_.enablePositionalTracking(tracking);

    if (err != sl::ERROR_CODE::SUCCESS) {
        last_error_ = "Failed to enable ZED positional tracking: ";
        last_error_ += sl::toString(err).c_str();
        zed_.close();
        return false;
    }

    last_error_.clear();
    return true;
}

bool ZedVio::read(VioState& state) {
    const auto grab_result = zed_.grab();
    if (grab_result != sl::ERROR_CODE::SUCCESS) {
        last_error_ = "Failed to grab ZED frame: ";
        last_error_ += sl::toString(grab_result).c_str();
        return false;
    }

    last_error_.clear();

    sl::Pose pose;

    const auto tracking_state = zed_.getPosition(pose, sl::REFERENCE_FRAME::WORLD);

    const auto p = pose.getTranslation();
    const auto q = pose.getOrientation();

    state.pose_valid = pose.valid && tracking_state == sl::POSITIONAL_TRACKING_STATE::OK;
    state.pose_confidence = pose.pose_confidence;

    state.timestamp_ns = pose.timestamp.getNanoseconds();

    state.position.x = p.tx;
    state.position.y = p.ty;
    state.position.z = p.tz;

    state.orientation.x = q.ox;
    state.orientation.y = q.oy;
    state.orientation.z = q.oz;
    state.orientation.w = q.ow;

    state.linear_velocity.x = pose.twist[0];
    state.linear_velocity.y = pose.twist[1];
    state.linear_velocity.z = pose.twist[2];

    state.angular_velocity.x = pose.twist[3];
    state.angular_velocity.y = pose.twist[4];
    state.angular_velocity.z = pose.twist[5];

    state.tracking_state = sl::toString(tracking_state).c_str();

    return true;
}

const std::string& ZedVio::lastError() const {
    return last_error_;
}
