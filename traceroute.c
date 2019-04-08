/*
 * Wojciech Pawlik
 * nr indeksu: 298992
 */

#include "common.h"
#include "send.h"
#include "receive.h"


int main(int argc, const char *argv[])
{
    validateArguments(argc, argv);

    int sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sockfd < 0)
        handle_error("Socket error");

    int pid = getpid();
    int seqNum = 0;
    int destReached = 0;
    for (int ttl = 1; ttl <= 30 && !destReached; ttl++)
    {
        // sending 3 packets with current ttl value
        for (int i = 0; i < 3; i++, seqNum++)
            sendPacket(sockfd, ttl, pid, seqNum, argv[1]);

        // start time-keeping
        struct timeval timeBefore, timeAfter, timeResult;
        gettimeofday(&timeBefore, NULL);
            
        // preparing data for receiving packets
        fd_set descriptors;
        FD_ZERO(&descriptors);
        FD_SET(sockfd, &descriptors);
        struct timeval tv;
        tv.tv_sec = 1;  
        tv.tv_usec = 0;

        int receivedPacketsCounter = 0;
        Packet correctPackets[3];
        while (receivedPacketsCounter < 3)
        {
            //waiting for packets
            int ready = select(sockfd + 1, &descriptors, NULL, NULL, &tv);
            if (ready == 0)
                break;
            if (ready < 0)
                handle_error("Error: Reading from socket failure.");
            Packet newPacket = receivePacket(sockfd);
            gettimeofday(&timeAfter, NULL);
            timersub(&timeAfter, &timeBefore, &timeResult);
            if (isReceivedPacketValid(&newPacket, pid, ttl)) {
                newPacket.duration = timeResult;
                correctPackets[receivedPacketsCounter] = newPacket;
                receivedPacketsCounter++;
            }
        }
        printf("%2d. ", ttl);
        printPacketsInfo(correctPackets, receivedPacketsCounter);
        if (isDestinationReached(correctPackets, receivedPacketsCounter, argv[1]))
            destReached = 1;
    }
}
