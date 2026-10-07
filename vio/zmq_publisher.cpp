#include "zmq_publisher.hpp"

#include <zmq.h>

ZmqPublisher::ZmqPublisher() {
    context_ = zmq_ctx_new();
    if (!context_) {
        setZmqError("Failed to create ZeroMQ context");
    }
}

ZmqPublisher::~ZmqPublisher() {
    closeSocket();

    if (context_) {
        zmq_ctx_term(context_);
    }
}

bool ZmqPublisher::open(const std::string& endpoint) {
    if (!context_) {
        return false;
    }

    if (socket_) {
        last_error_ = "ZeroMQ publisher is already open";
        return false;
    }

    socket_ = zmq_socket(context_, ZMQ_PUB);

    if (!socket_) {
        setZmqError("Failed to create ZeroMQ publisher socket");
        return false;
    }

    constexpr int linger_ms = 0;
    if (zmq_setsockopt(socket_, ZMQ_LINGER, &linger_ms, sizeof(linger_ms)) != 0) {
        setZmqError("Failed to set ZeroMQ publisher linger");
        closeSocket();
        return false;
    }

    if (zmq_bind(socket_, endpoint.c_str()) != 0) {
        setZmqError("Failed to bind ZeroMQ publisher");
        closeSocket();
        return false;
    }

    last_error_.clear();
    return true;
}

bool ZmqPublisher::publish(const std::string& message) {
    if (!socket_) {
        last_error_ = "ZeroMQ publisher is not open";
        return false;
    }

    const int rc = zmq_send(
        socket_,
        message.data(),
        message.size(),
        0
    );

    if (rc < 0) {
        setZmqError("Failed to send ZeroMQ message");
        return false;
    }

    last_error_.clear();
    return true;
}

const std::string& ZmqPublisher::lastError() const {
    return last_error_;
}

void ZmqPublisher::closeSocket() {
    if (socket_) {
        zmq_close(socket_);
        socket_ = nullptr;
    }
}

void ZmqPublisher::setZmqError(const std::string& operation) {
    last_error_ = operation;
    last_error_ += ": ";
    last_error_ += zmq_strerror(zmq_errno());
}
