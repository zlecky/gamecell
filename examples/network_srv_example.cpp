#include "network/tinynet.h"

#include <iostream>

using namespace TinyNet;

int main() {
    EventContext ev_ct;
    Signal::signal(SIGINT, [&ev_ct]() { ev_ct.exit(); });

    auto srv = TcpAcceptor::create_tcp_acceptor(&ev_ct, "127.0.0.1", "9010");
    if (srv != nullptr) {
        srv->on_create_cb([]() {
            auto conn = std::make_shared<TcpConn>();

            conn->on_msg(new LengthCodec, [](const TcpConnPtr& conn, Slice msg) {
                std::cout << "recv msg:" << msg.size() << std::endl;
            });

            return conn;
        });
    }

    ev_ct.loop();
    return 0;
}