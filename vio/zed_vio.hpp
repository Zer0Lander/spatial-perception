#pragma once

#include <string>

#include <sl/Camera.hpp>
#include "vio_state.hpp"

class ZedVio {
public:
    bool open();
    bool read(VioState& state);
    const std::string& lastError() const;

private:
    sl::Camera zed_;
    std::string last_error_;
};
