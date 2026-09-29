#include "zmq_publisher.hpp"

ZmqPublisher::ZmqPublisher() {
    context_ = zmq_ctx_new();
}

ZmqPublisher::~ZmqPublisher() {
    if (socket_) {
        zmq_close(socket_);
    }

    if (context_) {
        zmq_ctx_term(context_);
    }
}

bool ZmqPublisher::open(const std::string& endpoint) {
    socket_ = zmq_socket(context_, ZMQ_PUB);

    if (!socket_) {
        return false;
    }

    return zmq_bind(socket_, endpoint.c_str()) == 0;
}

bool ZmqPublisher::publish(const std::string& message) {
    if (!socket_) {
        return false;
    }

    int rc = zmq_send(
        socket_,
        message.data(),
        message.size(),
        0
    );

    return rc >= 0;
}

std::string ZmqPublisher::lastError() const {
    return zmq_strerror(zmq_errno());
}