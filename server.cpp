#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <ctime> // time_t, time(), tm, strftime
#include <sys/socket.h> // socket(), listen(), accept()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h> // read()

#include "config.h"

int main(int argCount, char *argValues[])
{
    time_t timeNow = time(0);
    tm *localTimeNow = localtime(&timeNow);
    char logFileName[sizeof("tcpServer_2020-09-17_2221.log")];
    size_t timeSize = strftime(
        logFileName,
        sizeof(logFileName),
        "tcpServer_%F_%H%M.log",
        localTimeNow);
    std::ofstream logFile;
    logFile.open(logFileName);
    
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

    int optionValue= 1;
    if (setsockopt(socketDescriptor, SOL_SOCKET, SO_REUSEADDR, &optionValue, sizeof(int)) < 0)
    {
	    std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
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
            char tcpBuffer[1024];

            // Read in data
            byteCount = read(
                clientDescriptor,
                tcpBuffer,
                sizeof(tcpBuffer));

            if (byteCount <= 0)
            {
                // Handle read() error
                std::cerr << "read() error" << std::endl;
                break;
            }

            // Handle data
            std::cout << "Received: " << tcpBuffer << std::endl;
            timeNow = time(0);
            localTimeNow = localtime(&timeNow);
            char timeStampBuffer[sizeof("2020-09-17_22:34:00")];
            timeSize = strftime(
                timeStampBuffer,
                sizeof(timeStampBuffer),
                "%F %H:%M:%S",
                localTimeNow);
            logFile << timeStampBuffer << " " << tcpBuffer << std::endl;
        }
    }

    logFile.close();
}
