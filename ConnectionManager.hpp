//
// Created by kaleado on 3/02/19.
//

#ifndef FUGUE_CONNECTIONMANAGER_HPP
#define FUGUE_CONNECTIONMANAGER_HPP

#include <string>
#include <iostream>

#include <boost/asio.hpp>
#include "ServerConfiguration.hpp"
#include "ServerState.hpp"
#include "CommandParser.hpp"

namespace Fugue {

    namespace io = boost::asio;

    class ConnectionManager {
    private:
        io::io_service _ioService;
        io::ip::tcp::endpoint _tcpEndpoint;
        io::ip::tcp::acceptor _tcpAcceptor;
        io::ip::tcp::socket _tcpSocket;
        char* _fixedBuffer;
        io::streambuf _dynamicBuffer;
        AbstractKeyValueStore<std::string>& _kvs;
        CommandParser<std::string> _parser;
        ServerState _state;

        void _handleWrite(const boost::system::error_code& errorCode, std::size_t bytesTransferred);

        void _handleAccept(const boost::system::error_code& errorCode);

        void _handleReadBinary(const boost::system::error_code &errorCode);

        void _handleReadText(const boost::system::error_code &errorCode);

        void _readIfAvailable();

        void _acceptConnection();

    public:

        void startListening();

        void shutdown();

        explicit ConnectionManager(Fugue::ServerConfiguration conf, Fugue::AbstractKeyValueStore<std::string>& kvs);

    };

}

#endif //FUGUE_CONNECTIONMANAGER_HPP
