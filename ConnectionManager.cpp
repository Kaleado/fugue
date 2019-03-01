//
// Created by kaleado on 3/02/19.
//

#include "ConnectionManager.hpp"

Fugue::ConnectionManager::ConnectionManager(Fugue::ServerConfiguration conf, Fugue::AbstractKeyValueStore<std::string>& kvs, Fugue::ExpirationManager<std::string>& expirationManager)
: _tcpEndpoint{io::ip::tcp::endpoint{io::ip::tcp::v4(), conf.listenPort}},
  _tcpAcceptor{io::ip::tcp::acceptor{_ioService, _tcpEndpoint}},
  _tcpSocket{io::ip::tcp::socket{_ioService}},
  _dynamicBuffer{conf.maxValueSize},
  _kvs{kvs},
  _expirationManager{expirationManager} {
    _state.maxValueSize = conf.maxValueSize;
}

void Fugue::ConnectionManager::_writeResponseText(std::string resp) {
    boost::system::error_code err;
    io::write(_tcpSocket, io::buffer(resp), io::transfer_all(), err);
    if(err){
        throw std::runtime_error("Error on write: " + err.message() + "\n");
    }
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
    _acceptConnection();
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
    try {
        auto cmd = _parser.parse(line);
        DataItem buffer;
        auto lck = _kvs.getUniqueLock();
        cmd->execute(_kvs, _expirationManager, _state, buffer);
        if(buffer.raw && buffer.length){
            std::cout << "Value: " << buffer.get<std::string>() << "\n";
            _writeResponseText(buffer.get<std::string>());
        }
        else
            std::cout << "Buffer was null.\n";
    }
    catch(std::exception& e){
        std::cerr << "Exception whilst executing command.\n";
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
//        _acceptConnection();
        return;
    }
    auto* data = _fixedBuffer;
    //is.get(data, _state.incomingBinaryLength);
    //TODO: fix heap overflow here.
    auto* dataString = new std::string(data, _state.incomingBinaryLength);
    if(_state.status == ServerState::READY_SET_BINARY){
        // Insert the received string in to the store.
        auto lck = _kvs.getUniqueLock();
        auto* item = new DataItem(dataString, sizeof(dataString));
        _kvs.insert(_state.settingKey, item);
    }
    else if(_state.status == ServerState::READY_APPEND_BINARY){
        // Append the received string to the stored string.
        auto lck = _kvs.getUniqueLock();
        auto item = static_cast<DataItem*>(_kvs.get(_state.settingKey));
        auto* contents = static_cast<std::string*>(item->raw);
        *contents += (*dataString);
        std::cout << "Appended, new value: " << contents << "\n";
    }
    _state.status = ServerState::READY_TEXT;
    _writeResponseText("success");
    _readIfAvailable();
}

void Fugue::ConnectionManager::_acceptConnection(){
    auto acceptHnd = std::bind(&ConnectionManager::_handleAccept, this, std::placeholders::_1);
    _tcpAcceptor.async_accept(_tcpSocket, acceptHnd);
}

void Fugue::ConnectionManager::_readIfAvailable() {
    switch(_state.status){
        case ServerState::READY_TEXT:
            {
                auto readHnd = std::bind(&ConnectionManager::_handleReadText, this, std::placeholders::_1);
                io::async_read_until(_tcpSocket, _dynamicBuffer, '\n', readHnd);
            }
            break;
        case ServerState::READY_SET_BINARY:
        case ServerState::READY_APPEND_BINARY:
            {
                _fixedBuffer = new char[_state.incomingBinaryLength];
                auto readHnd = std::bind(&ConnectionManager::_handleReadBinary, this, std::placeholders::_1);
                io::async_read(_tcpSocket, io::buffer(_fixedBuffer, _state.incomingBinaryLength), readHnd);
            }
            break;
        default:
            std::cerr << "Error: invalid server state.\n";
            break;
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
