#include <stdio.h>
#include <sys/socket.h> //general socket API
#include <linux/if_packet.h> // Linux packet-socket-specific definitions
#include <linux/if_ether.h> // protocols (?)
#include <arpa/inet.h> // idk

int main(void)
{
    printf("Network Monitor starting...\n");

   int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

   //Pick the buffer of this size because of the max size of IPv4 packet (65,535 bytes).
   unsigned char buffer[65536];

    if(sockfd == -1) {
        perror("socket");
    }

    printf("%d\n", sockfd);

    ssize_t received = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

    printf("%zd\n", received);

}