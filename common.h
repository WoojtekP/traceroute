#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <strings.h>
#include <assert.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>

#ifndef common_h
#define common_h

#define handle_error(msg) do { fprintf(stderr, msg); exit(EXIT_FAILURE); } while (0)
#define MAX_IP_LEN 16

typedef struct packet {
    struct timeval duration;
    char IP[MAX_IP_LEN];
    int id;
    int seq;
} Packet;

void validateArguments(int argc, const char* argv[]);
u_int16_t compute_icmp_checksum (const void *buff, int length);
void printPacketsInfo(const Packet *packets, int size);
int isDestinationReached(const Packet *packets, int counter, const char *IP);

#endif