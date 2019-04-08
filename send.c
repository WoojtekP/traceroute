/*
 * Wojciech Pawlik
 * nr indeksu: 298992
 */

#include "send.h"

struct icmphdr setHeader(int id, int seqNumber) {
    struct icmphdr icmp_header;
    icmp_header.type = ICMP_ECHO;
    icmp_header.code = 0;
    icmp_header.un.echo.id = id;
    icmp_header.un.echo.sequence = seqNumber;
    icmp_header.checksum = 0;
    icmp_header.checksum = compute_icmp_checksum((u_int16_t *)&icmp_header, sizeof(icmp_header));
    return icmp_header;
}

void sendPacket(int sockfd, int ttl, int id, int seqNumber, const char* IPaddr) {
    struct icmphdr ICMPheader = setHeader(id, seqNumber);
    struct sockaddr_in recipient;
    bzero(&recipient, sizeof(recipient));
    recipient.sin_family = AF_INET;
    
    if (inet_pton(AF_INET, IPaddr, &recipient.sin_addr) != 1)
        handle_error("Error: Sending packet failure.");
    
    if (setsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)) != 0)
        handle_error("Error: Sending packet failure.");
    
    ssize_t bytes_sent = sendto(
        sockfd, 
        &ICMPheader, 
        sizeof(ICMPheader), 
        0, 
        (struct sockaddr *)&recipient, 
        sizeof(recipient)
    );

    if (bytes_sent <= 0)
        handle_error("Error: Sending packet failure.");
}