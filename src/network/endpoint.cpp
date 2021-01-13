#include "endpoint.h"

#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <arpa/inet.h>

namespace TinyNet {
    Address::Address(const std::string& ip, const std::string& port) {
        memset(&addr_, 0, sizeof(addr_));

        addr_.sin_family = AF_INET;
        addr_.sin_port   = htons(std::atoi(port.data()));

        if (!ip.empty()) {
            addr_.sin_addr.s_addr = inet_addr(ip.data());
        } else {
            addr_.sin_addr.s_addr = INADDR_ANY;
        }
    }

    uint32_t Address::ip() const {
        return addr_.sin_addr.s_addr;
    }

    uint16_t Address::port() const {
        return addr_.sin_port;
    }

    struct sockaddr_in& Address::addr() {
        return addr_;
    }

    Endpoint::Endpoint(int fd/* = INVALID_SOCKET*/) {
        this->fd(fd);
    }

    Endpoint::~Endpoint() {

    }

    void Endpoint::socket(int type) {
        this->fd(::socket(AF_INET, type, 0));
    }

    int Endpoint::bind(uint16_t port/* = 0*/, uint32_t addr/* = INADDR_ANY*/) {
        struct sockaddr_in sin;
        sin.sin_family      = AF_INET;
        sin.sin_port        = port;
        sin.sin_addr.s_addr = addr;
        return ::bind(socket_, (struct sockaddr*)&sin, sizeof(sin));
    }

    int Endpoint::listen(int backlog/* = 5*/) {
        return ::listen(socket_, backlog);
    }

    EndpointPtr Endpoint::accept() {
        struct sockaddr_in sin;
        socklen_t sin_len = sizeof(sin);

        int ret = ::accept(socket_, (struct sockaddr*)&sin, &sin_len);
        if (ret < 0)
            return nullptr;

        return std::make_shared<Endpoint>(ret);
    }

    int Endpoint::connect(uint16_t port, uint32_t addr) {
        struct sockaddr_in sin;
        sin.sin_family      = AF_INET;
        sin.sin_port        = port;
        sin.sin_addr.s_addr = addr;
        return ::connect(socket_, (struct sockaddr*)&sin, sizeof(sin));
    }

    int Endpoint::close() {
        if (!this->valid())
            return 0;

        int ret = ::close(socket_);
        if (0 == ret) {
            this->fd(INVALID_SOCKET);
        }
        return ret;
    }

    int Endpoint::detach() {
        int ret = socket_;
        this->fd(INVALID_SOCKET);
        return ret;
    }

    Address Endpoint::local_address() {
        struct sockaddr_in sin;
        this->get_local_addr(&sin.sin_port, &sin.sin_addr.s_addr);

        return Address(sin);
    }

    Address Endpoint::remote_address() {
        struct sockaddr_in sin;
        this->get_remote_addr(&sin.sin_port, &sin.sin_addr.s_addr);

        return Address(sin);
    }

    void Endpoint::fd(int fd) {
        socket_ = fd;
    }

    int Endpoint::set_non_blocking(bool non_blocking) {
        return ::fcntl(socket_, F_SETFL, (non_blocking ? O_NONBLOCK : 0));
    }

    int Endpoint::set_reuse_addr(bool reuse) {
        int val = reuse ? 1 : 0;
        return ::setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, (void*)&val, sizeof(val));
    }

    int Endpoint::set_reuse_port(bool reuse) {
        int val = reuse ? 1 : 0;
        return ::setsockopt(socket_, SOL_SOCKET, SO_REUSEPORT, (void*)&val, sizeof(val));
    }

    int Endpoint::set_keepalive(bool keepalive) {
        int val = keepalive ? 1 : 0;
        return ::setsockopt(socket_, SOL_SOCKET, SO_KEEPALIVE, (void*)&val, sizeof(val));
    }

    int Endpoint::add_fd_flag(int flag) {
        int ret = ::fcntl(socket_, F_GETFD);
        return ::fcntl(socket_, F_SETFL, ret | flag);
    }

    int Endpoint::get_local_addr(uint16_t* port, uint32_t* addr) {
        struct sockaddr_in sin;
        socklen_t sin_len;

        int ret = ::getsockname(socket_, (struct sockaddr*)&sin, &sin_len);
        if (0 == ret) {
            if (port != nullptr)
                *port = sin.sin_port;

            if (addr != nullptr)
                *addr = sin.sin_addr.s_addr;
        }

        return ret;
    }

    int Endpoint::get_remote_addr(uint16_t* port, uint32_t* addr) {
        struct sockaddr_in sin;
        socklen_t sin_len;

        int ret = ::getpeername(socket_, (struct sockaddr*)&sin, &sin_len);
        if (0 == ret) {
            if (port != nullptr)
                *port = sin.sin_port;

            if (addr != nullptr)
                *addr = sin.sin_addr.s_addr;
        }

        return ret;
    }
}