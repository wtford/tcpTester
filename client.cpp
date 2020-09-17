#include <iostream>
#include <string>
#include <cstring>

#include <sys/socket.h> // socket(), connect()
#include <netinet/in.h> // sockaddr_in
#include <unistd.h> // read(), send()
#include <arpa/inet.h> // inet_pton()

#include "config.h"

int main(int argCount, char *argValues[])
{
    if (argCount < 2)
    {
        std::cerr << "usage: " << argValues[0] << " address" << std::endl;
        return 0;
    }

    // Create socketAddress
    struct sockaddr_in socketAddress;
    memset(&socketAddress, 0, sizeof(socketAddress));
    socketAddress.sin_family = AF_INET;
    socketAddress.sin_port = htons(portNumber);

    // Parse ip address
    int ipAddressResult = inet_pton(
        AF_INET,
        argValues[1],
        &socketAddress.sin_addr);
    
    if (-1 == ipAddressResult)
    {
        // Handle inet_pton() error
        std::cerr << "inet_pton() error" << std::endl;
        return -1;
    }

    // Create socketDescriptor
    int socketDescriptor = socket(
        AF_INET,
        SOCK_STREAM,
        0
    );

    if (-1 == socketDescriptor)
    {
        // Handle socket() error
        std::cerr << "socket() error" << std::endl;
        return -1;
    }

    int connectResult = connect(
        socketDescriptor,
        reinterpret_cast<sockaddr*>(&socketAddress),
        sizeof(socketAddress));

    if (-1 == connectResult)
    {
        // Handle connect() error
        std::cerr << "connect() error" << std::endl;
        return -1;
    }

    char message[] = "walker";

    send(
        socketDescriptor,
        message,
        strlen(message),
        0);
    
    std::cout << "Message sent: " << message << std::endl;

    sleep(5);

    return 0;
}
