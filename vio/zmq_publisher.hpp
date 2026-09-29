#pragma once

#include <zmq.h>
#include <string>

class ZmqPublisher {
public:
    ZmqPublisher();
    ~ZmqPublisher();

    bool open(const std::string& endpoint);
    bool publish(const std::string& message);

    std::string lastError() const;

private:
    void* context_ = nullptr;
    void* socket_ = nullptr;
};