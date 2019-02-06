//
// Created by kaleado on 3/02/19.
//

#include "ConnectionManager.hpp"

Fugue::ConnectionManager::ConnectionManager(Fugue::ServerConfiguration conf, Fugue::AbstractKeyValueStore<std::string>& kvs)
: _tcpEndpoint{io::ip::tcp::endpoint{io::ip::tcp::v4(), conf.listenPort}},
  _tcpAcceptor{io::ip::tcp::acceptor{_ioService, _tcpEndpoint}},
  _tcpSocket{io::ip::tcp::socket{_ioService}},
  _dynamicBuffer{conf.maxValueSize},
  _kvs{kvs} {
    _state.maxValueSize = conf.maxValueSize;
}

void Fugue::ConnectionManager::_handleWrite(const boost::system::error_code &errorCode, std::size_t bytesTransferred) {

}

void Fugue::ConnectionManager::_handleAccept(const boost::system::error_code &errorCode) {
    std::cout << "Accepted a connection\n";
    if(errorCode){
        std::cerr << "Error on accept: " << errorCode.message() << "\n";
        return;
    }
    else{
        _state.activeConnections++;
        _readIfAvailable();
    }
}

void Fugue::ConnectionManager::_handleReadText(const boost::system::error_code &errorCode) {
#ifdef DEBUG
    std::cout << "Reading text from a connection.\n";
#endif
    if(errorCode){
        std::cerr << errorCode.message() << "\n";
        _tcpSocket.close();
        _acceptConnection();
        return;
    }
    std::istream is{&_dynamicBuffer};
    std::string line;
    std::getline(is, line);
    // TODO: service request here.
    try {
        auto cmd = _parser.parse(line);
        DataItem buffer;
        cmd->execute(_kvs, _state, buffer);
        if(buffer.raw && buffer.length)
            std::cout << "Value: " << buffer.get<std::string>() << "\n";
        else
            std::cout << "Buffer was null.\n";
    }
    catch(std::exception& e){
        std::cerr << "Exception during parse.\n";
    }
    _readIfAvailable();
}

void Fugue::ConnectionManager::_handleReadBinary(const boost::system::error_code &errorCode) {
#ifdef DEBUG
    std::cout << "Reading binary from a connection.\n";
#endif
    if(errorCode){
        std::cerr << errorCode.message() << "\n";
        _tcpSocket.close();
        _acceptConnection();
        return;
    }
    auto* data = _fixedBuffer;
    //is.get(data, _state.incomingBinaryLength);
    auto* dataString = new std::string(data);
    auto* item = new DataItem(dataString, sizeof(dataString));
    _kvs.insert(_state.settingKey, item);
    _state.status = ServerState::READY_TEXT;
    _readIfAvailable();
}

void Fugue::ConnectionManager::_acceptConnection(){
    auto acceptHnd = std::bind(&ConnectionManager::_handleAccept, this, std::placeholders::_1);
    _tcpAcceptor.async_accept(_tcpSocket, acceptHnd);
}

void Fugue::ConnectionManager::_readIfAvailable() {
    if(_state.status == ServerState::READY_TEXT){
        auto readHnd = std::bind(&ConnectionManager::_handleReadText, this, std::placeholders::_1);
        io::async_read_until(_tcpSocket, _dynamicBuffer, '\n', readHnd);
    }
    else if(_state.status == ServerState::READY_BINARY){
        _fixedBuffer = new char[_state.incomingBinaryLength];
        auto readHnd = std::bind(&ConnectionManager::_handleReadBinary, this, std::placeholders::_1);
        io::async_read(_tcpSocket, io::buffer(_fixedBuffer, _state.incomingBinaryLength), readHnd);
    }
}

void Fugue::ConnectionManager::startListening() {
    _state.status = ServerState::READY_TEXT;
    _tcpAcceptor.listen(8);
    _acceptConnection();
    _ioService.run();
}

void Fugue::ConnectionManager::shutdown() {
    _tcpSocket.shutdown(io::ip::tcp::socket::shutdown_send);
}
