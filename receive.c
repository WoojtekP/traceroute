/*
 * Wojciech Pawlik
 * nr indeksu: 298992
 */

#include "receive.h"

Packet receivePacket(int sockfd) {
    Packet receivedPacked;
    struct sockaddr_in sender;
    socklen_t sender_len = sizeof(sender);
    
    u_int8_t buffer [IP_MAXPACKET];
    bzero(buffer, IP_MAXPACKET);
    
    ssize_t packet_len = recvfrom(sockfd, buffer, IP_MAXPACKET, 0, (struct sockaddr *)&sender, &sender_len);
    if (packet_len <= 0)
        handle_error("Error: Receiving packet failure.");

    if (inet_ntop(AF_INET, &(sender.sin_addr), receivedPacked.IP, sizeof(receivedPacked.IP)) == NULL)
        handle_error("Error: Receiving packet failure.");

    struct iphdr *ip_header = (struct iphdr *)buffer;
    ssize_t ip_header_len = 4 * ip_header->ihl;
    u_int8_t* icmp_packet = buffer + ip_header_len;
    struct icmphdr* icmp_header = (struct icmphdr*) icmp_packet;
    
    if (icmp_header->type == ICMP_ECHOREPLY) {
        receivedPacked.id = icmp_header->un.echo.id;
        receivedPacked.seq = icmp_header->un.echo.sequence;
    }
    else if (icmp_header->type == ICMP_TIME_EXCEEDED) {
        icmp_packet += 8;
        icmp_packet += 4 * ((struct ip*) icmp_packet)->ip_hl;
        struct icmp* icmpTimeEx = (struct icmp*) icmp_packet;

        receivedPacked.id = icmpTimeEx->icmp_id;
        receivedPacked.seq = icmpTimeEx->icmp_seq;
    }
    
    return receivedPacked;
}

int isReceivedPacketValid(const Packet* newPacket, int pid, int ttl) {
    return newPacket->id == pid && (newPacket->seq / 3 + 1) == ttl;
}