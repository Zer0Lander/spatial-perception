#pragma once

#include <sl/Camera.hpp>
#include "vio_state.hpp"

class ZedVio {
public:
    bool open();
    bool read(VioState& state);

private:
    sl::Camera zed_;
};