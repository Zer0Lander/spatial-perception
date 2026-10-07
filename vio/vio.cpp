#include <chrono>
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

#include "vio_json.hpp"
#include "zed_vio.hpp"
#include "zmq_publisher.hpp"

namespace {

volatile std::sig_atomic_t stop_requested = 0;

void requestStop(int) {
    stop_requested = 1;
}

}  // namespace

int main() {
    if (std::signal(SIGINT, requestStop) == SIG_ERR ||
        std::signal(SIGTERM, requestStop) == SIG_ERR) {
        std::cerr << "Failed to install signal handlers" << std::endl;
        return 1;
    }

    ZedVio vio;

    if (!vio.open()) {
        std::cerr << vio.lastError() << std::endl;
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
    std::string previous_tracking_state;
    auto next_grab_error_log = std::chrono::steady_clock::time_point::min();

    while (!stop_requested) {
        if (vio.read(state)) {
            if (state.tracking_state != previous_tracking_state) {
                std::cerr
                    << "ZED tracking state: " << state.tracking_state
                    << ", pose_valid=" << std::boolalpha << state.pose_valid
                    << ", confidence=" << state.pose_confidence
                    << std::endl;
                previous_tracking_state = state.tracking_state;
            }

            if (!publisher.publish(serializeVioState(state))) {
                std::cerr
                    << "Failed to publish VIO message: "
                    << publisher.lastError()
                    << std::endl;
            }
        } else if (!stop_requested) {
            const auto now = std::chrono::steady_clock::now();
            if (now >= next_grab_error_log) {
                std::cerr << vio.lastError() << std::endl;
                next_grab_error_log = now + std::chrono::seconds(1);
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    std::cerr << "Stopping VIO publisher" << std::endl;
    return 0;
}
