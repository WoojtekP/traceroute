#ifndef send_h
#define send_h

#include "common.h"

struct icmphdr setHeader(int id, int seqNumber);
void sendPacket(int sockfd, int ttl, int id, int seqNumber, const char* IPaddr);

#endif