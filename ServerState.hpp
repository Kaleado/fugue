//
// Created by kaleado on 5/02/19.
//

#ifndef FUGUE_SERVERSTATE_HPP
#define FUGUE_SERVERSTATE_HPP

#include <string>

namespace Fugue {
    class ServerState {
    private:
    public:
        enum Status { STOPPED, STARTING, READY_TEXT, READY_SET_BINARY, READY_APPEND_BINARY };
        Status status = STOPPED;
        int activeConnections = 0;
        std::string settingKey = "";
        std::size_t incomingBinaryLength = 0;
        std::size_t maxTransferSize = 0;
    };
}

#endif //FUGUE_SERVERSTATE_HPP
