#include "network/endpoint.h"

int main() {
    auto* ep = new TinyNet::Endpoint();
    ep->set_file_descriptor(-1);

    return 0;
}