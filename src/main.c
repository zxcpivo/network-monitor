
#include <stdio.h>
#include <sys/socket.h>     // General socket API
#include <linux/if_packet.h> // Linux packet sockets
#include <linux/if_ether.h>  // Ethernet protocol definitions
#include <arpa/inet.h>       // Network byte order conversion
#include <unistd.h>          // Close socket

int main(void)
{
    printf("Network Monitor starting...\n\n");

    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    unsigned char buffer[65536];

    if (sockfd == -1) {
        perror("socket");
        return 1;
    }

    printf("Socket file descriptor: %d\n", sockfd);
    printf("Listening for network packets...\n\n");

    unsigned long packetCount = 0;

    while (1) {

        // Receive a network packet
        ssize_t received = recvfrom(
            sockfd, buffer, sizeof(buffer), 0, NULL, NULL
        );

        if (received == -1) {
            perror("recvfrom");
            close(sockfd);
            return 1;
        }

        // Verify that the Ethernet header is complete
        if (received < ETH_HLEN) {
            continue;
        }

        packetCount++;

        printf("========================================\n");
        printf("PACKET #%lu\n", packetCount);
        printf("========================================\n");

        printf("Frame size: %zd bytes\n\n", received);

        // Ethernet header
        printf("ETHERNET HEADER\n");
        printf("----------------------------------------\n");

        // Destination MAC address (bytes 0-5)
        printf("Destination MAC: ");

        for (int i = 0; i < 6; i++) {
            printf("%02X", buffer[i]);

            if (i < 5) {
                printf(":");
            }
        }

        printf("\n");

        // Source MAC address (bytes 6-11)
        printf("Source MAC:      ");

        for (int i = 6; i < 12; i++) {
            printf("%02X", buffer[i]);

            if (i < 11) {
                printf(":");
            }
        }

        printf("\n");

        // EtherType (bytes 12-13)
        unsigned int etherType =
            (buffer[12] << 8) | buffer[13];

        printf("EtherType:       0x%04X", etherType);

        switch (etherType) {
            case ETH_P_IP:
                printf(" (IPv4)");
                break;

            case ETH_P_IPV6:
                printf(" (IPv6)");
                break;

            case ETH_P_ARP:
                printf(" (ARP)");
                break;

            default:
                printf(" (Other)");
                break;
        }

        printf("\n\n");

        // Print raw packet data in hexadecimal
        printf("RAW HEX DATA\n");
        printf("----------------------------------------\n");

        for (ssize_t i = 0; i < received; i++) {
            printf("%02X ", buffer[i]);

            // Display 16 bytes per line
            if ((i + 1) % 16 == 0) {
                printf("\n");
            }
        }

        printf("\n\n");
    }

    close(sockfd);

    return 0;
}
