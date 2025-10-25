#ifndef NETWORK_MONITOR_H
#define NETWORK_MONITOR_H

#include "packet.h"
#include "dissector.h"
#include <sys/socket.h>
#include <netinet/ip.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <unistd.h>
#include <thread>
#include <chrono>

class NetworkMonitor {
private:
    int sockfd;
    Queue packetQueue;
    Queue replayQueue;
    Queue backupQueue;
    PacketDissector dissector;
    int packetId;
    bool running;
    std::string interface;
    
public:
    NetworkMonitor(std::string iface = "eth0") : packetId(1), running(false), interface(iface) {}
    
    bool startCapture() {
        sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        if (sockfd < 0) {
            printf("Error creating socket\n");
            return false;
        }
        
        struct sockaddr_ll sll;
        memset(&sll, 0, sizeof(sll));
        sll.sll_family = AF_PACKET;
        sll.sll_protocol = htons(ETH_P_ALL);
        sll.sll_ifindex = if_nametoindex(interface.c_str());
        
        if (bind(sockfd, (struct sockaddr*)&sll, sizeof(sll)) < 0) {
            printf("Error binding to interface %s\n", interface.c_str());
            close(sockfd);
            return false;
        }
        
        printf("Bound to interface: %s\n", interface.c_str());
        
        running = true;
        std::thread captureThread(&NetworkMonitor::captureLoop, this);
        captureThread.detach();
        
        return true;
    }
    
    void stopCapture() {
        running = false;
        close(sockfd);
    }
    
    void captureLoop() {
        char buffer[2048];
        int packetCount = 0;
        
        printf("Starting packet capture loop...\n");
        
        while (running) {
            int n = recv(sockfd, buffer, sizeof(buffer), 0);
            if (n > 0) {
                packetCount++;
                printf("Captured packet %d, size: %d\n", packetCount, n);
                
                Packet packet;
                packet.id = packetId++;
                packet.timestamp = time(nullptr);
                packet.size = n;
                packet.data = new char[n];
                memcpy(packet.data, buffer, n);
                
                packetQueue.enqueue(packet);
                
                if (packetQueue.size() > 100) {
                    packetQueue.dequeue();
                }
            } else if (n < 0) {
                printf("Error receiving packet: %d\n", n);
            }
        }
        
        printf("Capture loop ended. Total packets: %d\n", packetCount);
    }
    
    void processPackets() {
        if (!packetQueue.isEmpty()) {
            Packet packet = packetQueue.dequeue();
            printf("Processing packet %d, size: %d\n", packet.id, packet.size);
            dissector.dissectPacket(packet);
        }
    }
    
    void filterPackets(std::string srcIP, std::string dstIP) {
        Queue tempQueue;
        
        while (!packetQueue.isEmpty()) {
            Packet packet = packetQueue.dequeue();
            
            if (packet.src_ip == srcIP && packet.dst_ip == dstIP) {
                if (packet.size <= 1500) {
                    replayQueue.enqueue(packet);
                } else {
                    backupQueue.enqueue(packet);
                }
            } else {
                tempQueue.enqueue(packet);
            }
        }
        
        while (!tempQueue.isEmpty()) {
            packetQueue.enqueue(tempQueue.dequeue());
        }
    }
    
    void replayPackets() {
        while (!replayQueue.isEmpty()) {
            Packet packet = replayQueue.dequeue();
            
            int delay = packet.size / 1000;
            if (delay < 1) delay = 1;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
            
            int retries = 0;
            bool success = false;
            
            while (retries < 2 && !success) {
                int result = send(sockfd, packet.data, packet.size, 0);
                if (result > 0) {
                    success = true;
                    printf("Packet %d replayed successfully\n", packet.id);
                } else {
                    retries++;
                    printf("Packet %d replay failed, retry %d\n", packet.id, retries);
                    if (retries >= 2) {
                        backupQueue.enqueue(packet);
                    }
                }
            }
        }
    }
    
    void displayPackets() {
        printf("Current Packets:\n");
        printf("ID\tTimestamp\tSource IP\tDestination IP\n");
        
        Queue tempQueue;
        while (!packetQueue.isEmpty()) {
            Packet packet = packetQueue.dequeue();
            printf("%d\t%ld\t%s\t%s\n", packet.id, packet.timestamp, 
                   packet.src_ip.c_str(), packet.dst_ip.c_str());
            tempQueue.enqueue(packet);
        }
        
        while (!tempQueue.isEmpty()) {
            packetQueue.enqueue(tempQueue.dequeue());
        }
    }
    
    void displayFilteredPackets() {
        printf("Filtered Packets:\n");
        printf("ID\tSize\tDelay\n");
        
        Queue tempQueue;
        while (!replayQueue.isEmpty()) {
            Packet packet = replayQueue.dequeue();
            int delay = packet.size / 1000;
            if (delay < 1) delay = 1;
            printf("%d\t%d\t%d ms\n", packet.id, packet.size, delay);
            tempQueue.enqueue(packet);
        }
        
        while (!tempQueue.isEmpty()) {
            replayQueue.enqueue(tempQueue.dequeue());
        }
    }
    
    void displayBackupPackets() {
        printf("Backup Packets:\n");
        printf("ID\tSize\n");
        
        Queue tempQueue;
        while (!backupQueue.isEmpty()) {
            Packet packet = backupQueue.dequeue();
            printf("%d\t%d\n", packet.id, packet.size);
            tempQueue.enqueue(packet);
        }
        
        while (!tempQueue.isEmpty()) {
            backupQueue.enqueue(tempQueue.dequeue());
        }
    }
};

#endif
