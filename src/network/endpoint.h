/** @brief   A network event lib using epoll.
 *  @file    endpoint.h
 *  @author  zlecky
 *  @version v0.0.1
 *  @date    2020/12/25
 */

#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__


#include <sys/fcntl.h>
#include <netinet/in.h>

#include <unistd.h>

#include <memory>

#include "basictypes.h"

/**
 * @brief
 * The lib's only namespace is TinyNet.
 */
namespace TinyNet {
    /**
     * @brief
     * This class provides a wrapper around a socket.
     */
    class Endpoint
    {
    public:
        static const int NO_SOCKET = -1;

    public:
         /**
          * Constructor
          */
        Endpoint();
         /**
          * Destructor
          */
        virtual ~Endpoint();

    public:
        /**
         *
         * @return Inner socket fd
         */
        explicit operator int() const;
        /**
         *
         * @param fd Socket file descriptor
         */
        void set_file_descriptor(int fd);
        /**
         *
         * @return If socket_ is NO_SOCKET, return false, or return true
         */
        bool good() const;

        /**
         *
         * @param type Socket's address family type.
         */
        void socket( int type );

        /**
         * Close socket fd
         * @return If successful, return 0, or return -1
         */
        int close();
        /**
         * Detach socket fd from Endpoint
         * @return Return detached socket fd
         */
        int detach();

        int bind(uint16_t port = 0, uint32_t addr = INADDR_ANY);

        int set_non_blocking(bool non_blocking);
        int set_reuse_addr(bool reuseaddr);
        int set_keepalive(bool keepalive);

        int get_local_addr(uint16_t* port, uint32_t* addr) const;
        int get_remote_addr(uint16_t* port, uint32_t* addr) const;

        int listen(int backlog = 5);
        int connect(uint16_t port, uint32_t addr);
        Endpoint* accept(uint16_t* port = nullptr, uint32_t* addr = nullptr);

        int send(const void* msg, int32_t len);
        int recv(void* msg, int32_t len);
        bool recv_all(void* msg, int32_t len);

    private:
        int	socket_; ///< This is internal socket representation of the Endpoint.
    };
}

#endif//__ENDPOINT_H__
