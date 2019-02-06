//
// Created by kaleado on 3/02/19.
//

#ifndef FUGUE_SERVERCONFIGURATION_HPP
#define FUGUE_SERVERCONFIGURATION_HPP

namespace Fugue {

    //! Encapsulates various configuration parameters for the Fugue server.
    //! Designed for easily serialisation, so everything is public.
    class ServerConfiguration {
    private:
    public:
        unsigned short listenPort = 11311;
    };

}

#endif //FUGUE_SERVERCONFIGURATION_HPP