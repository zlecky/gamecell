#include "endpoint.h"

#include "spdlog/spdlog.h"

TinyNet::Endpoint::Endpoint() {

}

TinyNet::Endpoint::~Endpoint() {
    this->close();
}

/*
 * Return the file descriptor for the endpoint.
 */
TinyNet::Endpoint::operator int() const {
    return socket_;
}

/*
 * Set the file descriptor used by the endpoint.
 */
void TinyNet::Endpoint::set_file_descriptor(int fd) {
    socket_ = fd;
}

/*
 * The method returns if the endpoint is a valid socket.
 */
bool TinyNet::Endpoint::good() const {
    return socket_ != NO_SOCKET;
}

/*
 * Create the socket of the request type.
 */
void TinyNet::Endpoint::socket(int type) {
    this->set_file_descriptor(::socket(AF_INET, type, 0));
}

/*
 * This method closes the socket associated with this endpoint.
 */
int TinyNet::Endpoint::close() {
    if (NO_SOCKET == socket_)
        return 0;

    int ret = ::close(socket_);
    if (0 == ret)
        this->set_file_descriptor(NO_SOCKET);

    return ret;
}

/*
 * This method detaches the socket from this endpoint.
 * When the destructor is called, the socket will not be automatically closed.
 */
int TinyNet::Endpoint::detach() {
    int ret = socket_;
    this->set_file_descriptor(NO_SOCKET);

    return ret;
}

/*
 * This method binds the socket to a given address and port.
 */
int TinyNet::Endpoint::bind(uint16_t port, uint32_t addr) {
    sockaddr_in sin;
    bzero(&sin, sizeof(sin));

    sin.sin_family      = AF_INET;
    sin.sin_port        = port;
    sin.sin_addr.s_addr = addr;

    return ::bind(socket_, (struct sockaddr*)&sin, sizeof(sin));
}

/*
 * This method controls the blocking mode of the socket.
 * When a socket is set to non-blocking mode, socket calls
 * will return immediately.
 */
int TinyNet::Endpoint::set_non_blocking(bool non_blocking) {
    return ::fcntl(socket_, F_SETFL, (non_blocking ? O_NONBLOCK : 0));
}

/*
 * This method toggles the reuse address mode of the socket.
 */
int TinyNet::Endpoint::set_reuse_addr(bool reuseaddr) {
    int val = reuseaddr ? 1 : 0;
    return ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (char*)val, sizeof(val));
}

/*
 * Set keepalive of the socket.
 */
int TinyNet::Endpoint::set_keepalive(bool keepalive) {
    int val = keepalive ? 1 : 0;
    return ::setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, (char*)val, sizeof(val));
}

/*
 * This method returns the local address and port to which this endpoint is bound.
 */
int TinyNet::Endpoint::get_local_addr(uint16_t *port, uint32_t *addr) const {
    sockaddr_in sin;
    socklen_t sin_len = sizeof(sin);
    bzero(&sin, sin_len);

    int ret = ::getsockname(socket_, (struct sockaddr*)&sin, &sin_len);
    if (0 == ret) {
        if (port != nullptr)
            *port = sin.sin_port;

        if (addr != nullptr)
            *addr = sin.sin_addr.s_addr;
    }

    return ret;
}

/*
 * This method returns the remote address and port to which this endpoint is connected.
 */
int TinyNet::Endpoint::get_remote_addr(uint16_t *port, uint32_t *addr) const {
    sockaddr_in sin;
    socklen_t sin_len = sizeof(sin);
    bzero(&sin, sin_len);

    int ret = ::getpeername(socket_, (struct sockaddr*)&sin, &sin_len);
    if (0 == ret) {
        if (port != nullptr)
            *port = sin.sin_port;

        if (addr != nullptr)
            *addr = sin.sin_addr.s_addr;
    }

    return ret;
}

/*
 * This method instructs this endpoint to listen for incoming connections.
 */
int TinyNet::Endpoint::listen(int backlog/* = 5*/) {
    return ::listen(socket_, backlog);
}

/*
 * This method connections this endpoint to a destination address.
 */
int TinyNet::Endpoint::connect(uint16_t port, uint32_t addr) {
    sockaddr_in sin;
    bzero(&sin, sizeof(sin));

    sin.sin_family      = AF_INET;
    sin.sin_port        = port;
    sin.sin_addr.s_addr = addr;

    return ::connect(socket_, (struct sockaddr*)&sin, sizeof(sin));
}

/*
 * This method accepts a connection on the socket listen queue, returning
 * a new endpoint if successful, or nullptr if not. The remote port and
 * address are set into the pointers passed in if not nullptr.
 */
TinyNet::Endpoint* TinyNet::Endpoint::accept(uint16_t* port/* = nullptr*/, uint32_t* addr/* = nullptr*/) {
    sockaddr_in sin;
    socklen_t sin_len = sizeof(sin);
    bzero(&sin, sin_len);

    int ret = ::accept(socket_, (struct sockaddr*)&sin, &sin_len);
    if (ret < 0)
        return nullptr;

    auto* ep = new Endpoint();
    ep->set_file_descriptor(ret);

    if (port != nullptr)
        *port = sin.sin_port;
    if (addr != nullptr)
        *addr = sin.sin_addr.s_addr;

    return ep;
}

/*
 * This method sends some data to the given address.
 */
int TinyNet::Endpoint::send(const void* msg, int32_t len) {
    return ::send(socket_, msg, len, 0);
}

/*
 * This method attempts to receive some data.
 */
int TinyNet::Endpoint::recv(void *msg, int32_t len) {
    return ::recv(socket_, msg, len, 0);
}

/*
 * This helper method wait until exactly gramSize data has been read.
 * True if gramSize was read, otherwise false (usually indicating the
 * connection was lost.
 */
bool TinyNet::Endpoint::recv_all(void* msg, int32_t len) {
    while (len > 0) {
        int ret = this->recv(msg, len);

        if (ret <= 0) {
            if (0 == ret) {
                spdlog::error("Endpoint::recv_all: Connection lost");
            } else {
                spdlog::error("Endpoint::recv_all: Got error %", strerror(errno));
            }

            return false;
        }

        len -= ret;
        msg = (char*)msg + len;
    }

    return false;
}

