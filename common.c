/*
 * Wojciech Pawlik
 * nr indeksu: 298992
 */

#include "common.h"

void validateArguments(int argc, const char* argv[]) {
    if (argc != 2)
        handle_error("Invalid input format");
	struct sockaddr_in test;
	if (inet_pton(AF_INET, argv[1], &test.sin_addr) != 1)
        handle_error("Invalid input format");
}

u_int16_t compute_icmp_checksum (const void *buff, int length) {
	u_int32_t sum;
	const u_int16_t* ptr = buff;
	assert (length % 2 == 0);
	for (sum = 0; length > 0; length -= 2)
		sum += *ptr++;
	sum = (sum >> 16) + (sum & 0xffff);
	return (u_int16_t)(~(sum + (sum >> 16)));
}

void printPacketsInfo(const Packet *packets, int size) {
    if (size == 0) {
        printf("*\n");
        return;
    }
    printf("%s", packets[0].IP);
    if (size > 1 && strcmp(packets[1].IP, packets[0].IP) != 0)
        printf(" %s", packets[1].IP);
    if (size > 2 && strcmp(packets[2].IP, packets[0].IP) != 0 && strcmp(packets[2].IP, packets[1].IP) != 0)
        printf(" %s", packets[1].IP);
    if (size == 3){
        int averageTime = 0;
        for (int i = 0; i < 3; i++)
            averageTime += packets[i].duration.tv_usec;
        averageTime /= 3000;
        printf(" %dms\n", averageTime);
    }
    else 
        printf(" ???\n");
}

int isDestinationReached(const Packet *packets, int counter, const char *IP) {
    if (counter < 3)
        return 0;
    if (strcmp(packets[0].IP, IP) == 0 && strcmp(packets[1].IP, IP) == 0 && strcmp(packets[2].IP, IP) == 0)
        return 1;
    return 0;
}