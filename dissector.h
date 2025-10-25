#ifndef DISSECTOR_H
#define DISSECTOR_H

#include "packet.h"
#include <arpa/inet.h>

class PacketDissector {
private:
    Stack layerStack;
    
public:
    PacketDissector() : layerStack(10) {}
    
    void dissectPacket(Packet& packet) {
        printf("=== Dissecting Packet %d ===\n", packet.id);
        printf("Packet size: %d bytes\n", packet.size);
        
        layerStack.push(packet);
        
        int offset = 0;
        char* data = packet.data;
        
        if (offset + 14 <= packet.size) {
            parseEthernet(data + offset);
            offset += 14;
        } else {
            printf("Packet too small for Ethernet header\n");
            return;
        }
        
        if (offset + 20 <= packet.size) {
            int ipVersion = (data[offset] >> 4) & 0x0F;
            printf("IP Version: %d\n", ipVersion);
            
            if (ipVersion == 4) {
                offset = parseIPv4(data + offset, packet);
            } else if (ipVersion == 6) {
                offset = parseIPv6(data + offset, packet);
            } else {
                printf("Unknown IP version: %d\n", ipVersion);
                return;
            }
        } else {
            printf("Packet too small for IP header\n");
            return;
        }
        
        if (offset + 8 <= packet.size) {
            int protocol = data[offset + 9];
            printf("Protocol: %d\n", protocol);
            
            if (protocol == 6) {
                parseTCP(data + offset);
            } else if (protocol == 17) {
                parseUDP(data + offset);
            } else {
                printf("Unknown protocol: %d\n", protocol);
            }
        } else {
            printf("Packet too small for transport header\n");
        }
        
        printf("=== End Packet %d ===\n\n", packet.id);
    }
    
private:
    void parseEthernet(char* data) {
        printf("Ethernet Layer\n");
        printf("Destination MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
               data[0], data[1], data[2], data[3], data[4], data[5]);
        printf("Source MAC: %02x:%02x:%02x:%02x:%02x:%02x\n",
               data[6], data[7], data[8], data[9], data[10], data[11]);
    }
    
    int parseIPv4(char* data, Packet& packet) {
        printf("IPv4 Layer\n");
        
        char src[16], dst[16];
        sprintf(src, "%d.%d.%d.%d", data[12], data[13], data[14], data[15]);
        sprintf(dst, "%d.%d.%d.%d", data[16], data[17], data[18], data[19]);
        
        packet.src_ip = std::string(src);
        packet.dst_ip = std::string(dst);
        
        printf("Source IP: %s\n", src);
        printf("Destination IP: %s\n", dst);
        
        return 20;
    }
    
    int parseIPv6(char* data, Packet& packet) {
        printf("IPv6 Layer\n");
        
        char src[40], dst[40];
        inet_ntop(AF_INET6, data + 8, src, 40);
        inet_ntop(AF_INET6, data + 24, dst, 40);
        
        packet.src_ip = std::string(src);
        packet.dst_ip = std::string(dst);
        
        printf("Source IP: %s\n", src);
        printf("Destination IP: %s\n", dst);
        
        return 40;
    }
    
    void parseTCP(char* data) {
        printf("TCP Layer\n");
        int srcPort = (data[0] << 8) | data[1];
        int dstPort = (data[2] << 8) | data[3];
        printf("Source Port: %d\n", srcPort);
        printf("Destination Port: %d\n", dstPort);
    }
    
    void parseUDP(char* data) {
        printf("UDP Layer\n");
        int srcPort = (data[0] << 8) | data[1];
        int dstPort = (data[2] << 8) | data[3];
        printf("Source Port: %d\n", srcPort);
        printf("Destination Port: %d\n", dstPort);
    }
};

#endif
