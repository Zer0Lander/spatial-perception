#pragma once

#include <string>

class ZmqPublisher {
public:
    ZmqPublisher();
    ~ZmqPublisher();

    ZmqPublisher(const ZmqPublisher&) = delete;
    ZmqPublisher& operator=(const ZmqPublisher&) = delete;

    bool open(const std::string& endpoint);
    bool publish(const std::string& message);

    const std::string& lastError() const;

private:
    void closeSocket();
    void setZmqError(const std::string& operation);

    void* context_ = nullptr;
    void* socket_ = nullptr;
    std::string last_error_;
};
