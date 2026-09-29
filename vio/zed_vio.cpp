#include "zed_vio.hpp"

bool ZedVio::open() {
    sl::InitParameters init;
    init.coordinate_units = sl::UNIT::METER;
    init.coordinate_system = sl::COORDINATE_SYSTEM::RIGHT_HANDED_Z_UP_X_FWD;

    auto err = zed_.open(init);

    if (err != sl::ERROR_CODE::SUCCESS) {
        return false;
    }

    sl::PositionalTrackingParameters tracking;

    err = zed_.enablePositionalTracking(tracking);

    return err == sl::ERROR_CODE::SUCCESS;
}

bool ZedVio::read(VioState& state) {
    if (zed_.grab() != sl::ERROR_CODE::SUCCESS) {
        return false;
    }

    sl::Pose pose;

    auto tracking_state =
        zed_.getPosition(pose, sl::REFERENCE_FRAME::WORLD);

    auto p = pose.getTranslation();
    auto q = pose.getOrientation();

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