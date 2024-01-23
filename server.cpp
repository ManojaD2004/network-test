#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>


int main() {
    // Create a UDP socket
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    // Bind the socket to a specific address and port
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(12345);
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        perror("Error binding socket");
        close(serverSocket);
        return -1;
    }

    char buffer[1024];
    sockaddr_in clientAddress;
    socklen_t clientAddrLen = sizeof(clientAddress);

    // Receive data from clients
    while (true) {
        ssize_t bytesRead = recvfrom(serverSocket, buffer, sizeof(buffer), 0,
                                     (struct sockaddr*)&clientAddress, &clientAddrLen);

        if (bytesRead == -1) {
            perror("Error receiving data");
            break;
        }

        // Process the received data (in this case, just printing to console)
        buffer[bytesRead] = '\0';
        std::cout << "Received from client: " << (struct sockaddr*)&clientAddress << " " 
        << clientAddrLen << " " << buffer << std::endl;

        // Send a response back to the client
        const char* response = "Message received!";
        sendto(serverSocket, response, strlen(response), 0,
               (struct sockaddr*)&clientAddress, clientAddrLen);
    }

    // Close the socket
    close(serverSocket);

    return 0;
}
