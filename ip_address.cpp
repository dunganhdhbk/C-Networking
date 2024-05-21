//
// Created by dungtda on 21/05/2024.
//
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string>


char *produce_packet(std::string basicString, char* buffer) {
    int64_t length;
    length = ntohl(basicString.length());
    std::cout << length << std::endl;
    memcpy(buffer, &length, 4);

    int8_t service = 2;
    service = ntohs(2);
    std::cout << service << std::endl;
    memcpy(buffer + 4, &service, 2);

    memcpy(buffer, basicString.c_str(), basicString.length());

}

int test() {
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(4444);
    serverAddress.sin_addr.s_addr = inet_addr("45.119.83.244");

    int error = connect(clientSocket, (struct sockaddr *) &serverAddress,
                        sizeof(serverAddress));
    std::cout << "Error : " << error << std::endl;

    // sending data
    std::string d = "{\"requestId\": \"1\","
                    "\"username\": \"username\","
                    "\"password\": \"password\","
                    "\"module_name\": \"client\","
                    "\"dc_id\": \"dc_id\","
                    "\"ip\": \"127.0.0.1\","
                    "\"port\": \"9999\"}";

    char* buffer;
    produce_packet(d, buffer);

    std::cout << "Packet: " << strlen(buffer) << std::endl;

    send(clientSocket, buffer, strlen(buffer), 0);

    while (true) {
        std::cout << "Message from client: " << std::endl;

        char* buffer;
        int bytes, buflen; // for bytes written and size of buffer

        /* Read the incoming size */
        bytes = recv(clientSocket, (char*)&buflen, sizeof(buflen), 0);
        if (bytes < 0){
            std::cout << "Error reading size of data" << std::endl;
            return 1;
        }
        buflen = ntohs(buflen);
        buffer = new char[buflen+1]; // +1 for the NUL-terminator
        /* Read the data */

        bytes = recv(clientSocket, buffer, buflen, 0);
        if (bytes < 0){
            std::cout << "Error reading data" << std::endl;
            return 1;
        }

        break;
    }

    // closing socket
    close(clientSocket);

    return 0;
}
