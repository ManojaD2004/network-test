#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <chrono>
#include <ctime>   

int main() {
    // Create a UDP socket
    int clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        return -1;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(12345);
    inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr);

    // Send data to the server
    const char* message = "Hello, server!";
    auto start = std::chrono::system_clock::now();
    sendto(clientSocket, message, strlen(message), 0,
           (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    char buffer[1024];

    // Receive response from the server
    ssize_t bytesRead = recvfrom(clientSocket, buffer, sizeof(buffer), 0, nullptr, nullptr);
    if (bytesRead == -1) {
        perror("Error receiving data");
    } else {
        buffer[bytesRead] = '\0';
        std::cout << "Server response: " << buffer << std::endl;

        auto end = std::chrono::system_clock::now();
 
        std::chrono::duration<double> elapsed_seconds = end-start;
        std::cout<<elapsed_seconds.count()<<" Seconds Took this much time!!"<<std::endl;
    }

    // Close the socket
    close(clientSocket);

    return 0;
}
