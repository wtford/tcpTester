#include <iostream>
#include <string>
#include <cstring>

#include <sys/socket.h> // socket(), listen(), accept()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h> // read()

#include "config.h"

int main(int argCount, char *argValues[])
{
    // Create socketDescriptor
    int socketDescriptor = socket(
        AF_INET,
        SOCK_STREAM,
        0);

    if (-1 == socketDescriptor)
    {
        // Handle socket() error
        std::cerr << "socket() error" << std::endl;
        return -1;
    }

    // Create socketAddress
    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_addr.s_addr = INADDR_ANY;
    socketAddress.sin_port = htons(portNumber);

    // Bind address to socket
    int bindResult = bind(
        socketDescriptor,
        reinterpret_cast<sockaddr*>(&socketAddress),
        sizeof(socketAddress));

    if (0 != bindResult)
    {
        // Handle bind() error
        std::cerr << "bind() error" << std::endl;
        return -1;
    }

    // Set socket options
    // int socketOptions = 1;
    // if (setsockopt(
    //     socketDescriptor,
    //     SOL_SOCKET,
    //     SO_REUSEADDR | SO_REUSEPORT,
    //     &socketOptions,
    //     sizeof(socketOptions)))
    // { 
    //     std::cerr << "setsockopt() error" << std::endl; 
    //     return(-1); 
    // } 

    // Listen for connections
    listen(socketDescriptor, SOMAXCONN);

    while (1)
    {
        sockaddr_in clientAddress;
        int clientDescriptor;
        socklen_t clientLength = sizeof(clientAddress);

        // Accept new connection
        clientDescriptor = accept(
            socketDescriptor,
            reinterpret_cast<sockaddr*>(&clientAddress),
            &clientLength);

        if (-1 == clientDescriptor)
        {
            // Handle accept() error
            std::cerr << "accept() error" << std::endl;
            return -1;
        }

        while (1)
        {
            int byteCount;
            char buffer[1024];

            // Read in data
            byteCount = read(
                clientDescriptor,
                buffer,
                sizeof(buffer));

            if (byteCount <= 0)
            {
                // Handle read() error
                std::cerr << "read() error" << std::endl;
                break;
            }

            // Handle data
            std::cout << "Received: " << buffer << std::endl;
        }
    }
}
