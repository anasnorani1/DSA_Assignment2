#include "network_monitor.h"
#include <iostream>
#include <thread>
#include <chrono>

//Run with make and sudo not with g++ for root access
int main() {
    printf("Network Monitor Starting...\n");
    
    NetworkMonitor monitor("ens3"); //Interface specifically for VMWare Ubuntu
    
    if (!monitor.startCapture()) {
        printf("Failed to start packet capture\n");
        return 1;
    }
    
    printf("Packet capture started. Running for 1 minute...\n");
    
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + std::chrono::minutes(1);
    
    while (std::chrono::steady_clock::now() < endTime) {
        monitor.processPackets();
        
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        
        static int displayCount = 0;
        if (displayCount % 50 == 0) {
            monitor.displayPackets();
        }
        displayCount++;
    }
    
    printf("Capture completed. Testing filtering...\n");
    
    monitor.filterPackets("192.168.1.1", "192.168.1.2");
    
    printf("Filtered packets:\n");
    monitor.displayFilteredPackets();
    
    printf("Backup packets:\n");
    monitor.displayBackupPackets();
    
    printf("Testing replay...\n");
    monitor.replayPackets();
    
    printf("Replay completed. Testing error handling...\n");
    
    monitor.displayBackupPackets();
    
    monitor.stopCapture();
    
    printf("Network Monitor Test Completed\n");
    
    return 0;
}
