//
// Created by dungtda on 23/05/2024.
//

#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <cstdio>
#include <csignal>
#include <arpa/inet.h>
#include <cstdlib>

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void packi16_be(unsigned char *buf, unsigned int i) {
    *buf++ = i>>8;
    *buf++ = i;
}

void packi32_le(unsigned char *buf, unsigned long i) {
    *buf++ = i;
    *buf++ = i>>8;
    *buf++ = i>>16;
    *buf++ = i>>24;
}

unsigned int unpacku16_be(unsigned char *buf) {
    return ((unsigned int) buf[0]<<8 | buf[1]);
}

unsigned long int unpacku32_le(unsigned char *buf) {
    return ((unsigned long int) buf[3]<<24) |
            ((unsigned long int) buf[2]<<16)|
            ((unsigned long int) buf[1]<<8) |
            ((unsigned long int) buf[0]);
}

unsigned int pack(unsigned char *buf, char *s) {
    unsigned int size = 0;
    packi32_le(buf, strlen(s));
    buf += 4;
    size +=4;
    printf("pack length\n");

    packi16_be(buf, 2);
    buf += 2;
    size += 2;
    printf("pack service\n");

    memcpy(buf, s, strlen(s));
    buf += strlen(s);
    size += strlen(s);
    printf("pack data\n");
    return size;
}



int main() {

    struct addrinfo hints, *servinfo, *p;
    int sockfd;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo("45.119.83.244", "4444", &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 2;
    }

    for (p = servinfo; p != NULL; p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family,
              get_in_addr((struct sockaddr *)p->ai_addr),
                      s,
                      sizeof s);
    printf("client: connecting to %s\n", s);

    char *data = "{\"requestId\": \"1\",\"username\": \"username\",\"password\": \"password\",\"module_name\": \"client\",\"dc_id\": \"dc_id\",\"ip\": \"127.0.0.1\",\"port\": \"9999\"}";
//    char *data = "Great unmitigated Zot! You've found the Runestaff!";
    unsigned char buf[4 + 2 + strlen(data)];
    unsigned int size;
    size = pack(buf, data);
    printf("Send data %d \n", size);
    if (send(sockfd, buf, size, 0) == -1) {
        perror("send");
        _exit(1);
    }

    printf("Send data done\n");
    printf("start wait...\n");


    while (true) {
        printf("waiting...\n");
        unsigned char lengthbuf[4];
        recv(sockfd, lengthbuf, 4, 0);
        unsigned long length = unpacku32_le(lengthbuf);
        printf("Length Data: %ld. ", length);
        unsigned char servicebuf[2];
        recv(sockfd, servicebuf, 2, 0);
        unsigned int service = unpacku16_be(servicebuf);
        printf("Service: %d. ", service);
        unsigned char databuf[length];
        recv(sockfd, databuf, length, 0);
        printf("Data: %s. \n", databuf);
    }


}
