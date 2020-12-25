#ifndef __ENDPOINT_H__
#define __ENDPOINT_H__


#include <sys/fcntl.h>
#include <netinet/in.h>

#include <unistd.h>

#include <memory>

#include "basictypes.h"

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
        /// @name Construction/Destruction
        //@{
        Endpoint();
        virtual ~Endpoint();
        //@}

        /// @name File descriptor access
        //@{
        explicit operator int() const;
        void set_file_descriptor(int fd);
        bool good() const;
        //@}

        /// @name General Socket Methods
        //@{
        void socket( int type );

        int close();
        int detach();

        int bind(uint16_t port = 0, uint32_t addr = INADDR_ANY);

        int set_non_blocking(bool non_blocking);
        int set_reuse_addr(bool reuseaddr);
        int set_keepalive(bool keepalive);

        int get_local_addr(uint16_t* port, uint32_t* addr) const;
        int get_remote_addr(uint16_t* port, uint32_t* addr) const;
        //@}

        /// @name Connecting Socket Methods
        //@{
        int listen(int backlog = 5);
        int connect(uint16_t port, uint32_t addr);
        Endpoint* accept(uint16_t* port = nullptr, uint32_t* addr = nullptr);

        int send(const void* msg, int32_t len);
        int recv(void* msg, int32_t len);
        bool recv_all(void* msg, int32_t len);
        //@}

    private:
        /// This is internal socket representation of the Endpoint.
        int	socket_;
    };
}

#endif//__ENDPOINT_H__
