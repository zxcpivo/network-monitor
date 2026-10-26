#include <stdio.h>
#include <sys/socket.h> //general socket API
#include <linux/if_packet.h> // Linux packet-socket-specific definitions
#include <linux/if_ether.h> // protocols (?)
#include <arpa/inet.h> // idk
#include <unistd.h> //close socket

int main(void)
{
    printf("Network Monitor starting...\n\n");

   int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

   //Pick the buffer of this size because of the max size of IPv4 packet (65,535 bytes).
   unsigned char buffer[65536];

    if(sockfd == -1) {
        perror("socket");
        return 1;
    }

    //print socket file descriptor (socket identifier)
    printf("Socket file descriptor: %d\n", sockfd);

    ssize_t received = recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);

    if(received == -1) {
        perror("revfrom");
        close(sockfd);
        return 1;
    }

    //The amount of bytes received
    printf("Amount of bytes recieved: %zd\n\n", received);

    //prints in hex
    printf("Raw hex:\n");
    for(int i = 0; i < received; i++) {
        printf("%02X ", buffer[i]);
    }
    printf("\n\n");



    printf("ETHERNET HEADER: \n");

    printf("Destination MAC address: ");
    for(int i = 0; i < 4; i++) {
        printf("%02X ", buffer[i]);
    }




    printf("\n");
    close(sockfd);

}