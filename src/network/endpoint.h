#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__

#include <string>
#include <netinet/in.h>

#include "types.h"

namespace TinyNet {
    class Address {
    public:
        Address(const std::string& ip, const std::string& port);
        explicit Address(struct sockaddr_in sin) : addr_(sin) {}
        virtual ~Address() = default;

    public:
        uint32_t ip() const;
        uint16_t port() const;
        struct sockaddr_in& addr();

    private:
        struct sockaddr_in addr_;
    };

    class Endpoint {
    public:
        static const int INVALID_SOCKET = -1;

    public:
        explicit Endpoint(int fd = INVALID_SOCKET);
        virtual ~Endpoint();

    public:
        void socket(int type);

        int bind(uint16_t port = 0, uint32_t addr = INADDR_ANY);
        int listen(int backlog = 5);
        EndpointPtr accept();

        int connect(uint16_t port, uint32_t addr);

        int close();
        int detach();

    public:
        Address local_address();
        Address remote_address();

    public:
        void fd(int fd);
        int fd() const { return socket_; }
        bool valid() const { return socket_ != INVALID_SOCKET; }

        int set_non_blocking(bool non_blocking);
        int set_reuse_addr(bool reuse);
        int set_reuse_port(bool reuse);
        int set_keepalive(bool keepalive);

        int add_fd_flag(int flag);

    private:
        int get_local_addr(uint16_t* port, uint32_t* addr);
        int get_remote_addr(uint16_t* port, uint32_t* addr);

    private:
        int socket_ = INVALID_SOCKET;
    };
}

#endif//__ENDPOINT_H__