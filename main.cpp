#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstring>

int main() {
    printf("Hello World!!!\n");

    struct sockaddr_in sa{}; //IPv4

    int error = inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr);
    if (error == -1) {
        printf("Error while printable to network\n");
    }

    char ipv4[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &sa.sin_addr, ipv4, INET_ADDRSTRLEN);

    printf("IPv4 Address: %s\n", ipv4);


    int status;
    struct addrinfo hints;
    struct addrinfo *servinfo;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ((status = getaddrinfo(NULL, "030000", &hints, &servinfo))  != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
        exit(1);
    }



}