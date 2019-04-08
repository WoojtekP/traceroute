#ifndef receive_h
#define receive_h

#include "common.h"

Packet receivePacket(int sockfd);
int isReceivedPacketValid(const Packet* newPacket, int pid, int ttl);

#endif