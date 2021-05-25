#include "tcp_connection.h"

#include "buffer.h"
#include "codec.h"
#include "channel.h"
#include "event_poller.h"

namespace TinyNet {
    TcpConn::TcpConn() {

    }

    TcpConn::~TcpConn() {

    }

    void TcpConn::attach(EventContextPtr ct, EndpointPtr ep) {
        context_  = ct;
        endpoint_ = ep;
        state_    = State::handshaking;

        if (channel_ != nullptr)
            delete channel_;
        channel_ = new Channel(ct, endpoint_->fd(), kReadEvent | kWriteEvent);

        auto self = shared_from_this();
        self->channel_->on_read_cb([self]() { self->handle_read(self); });
        self->channel_->on_write_cb([self]() { self->handle_write(self); });
    }

    void TcpConn::on_read(const TcpCallback& cb) {
        read_cb_ = cb;
    }

    void TcpConn::on_state(const TcpCallback& cb) {
        state_cb_ = cb;
    }

    void TcpConn::on_msg(CodecBasePtr codec, const MsgCallback& cb) {
        codec_.reset(codec);

        this->on_read([cb](const TcpConnPtr& conn) {
            int ret = 0;

            do {
                Slice msg;
                ret = conn->codec_->tryDecode(conn->in(), msg);
                if (ret < 0) {
                    conn->channel_->close();
                    break;
                } else if (ret > 0) {
                    cb(conn, msg);
                    conn->in().consume(ret);
                }

            } while (ret > 0);
        });
    }

    void TcpConn::handle_read(const TcpConnPtr conn) {

    }

    void TcpConn::handle_write(const TcpConnPtr conn) {

    }
}