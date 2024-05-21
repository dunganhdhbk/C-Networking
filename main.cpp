#include <string>
#include <arpa/inet.h>

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



}