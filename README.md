# 🧩 Network Monitor – Assignment 2  
### **Course:** CS250 – Data Structures and Algorithms  
**Student:** Anas Norani  
**CMS:** 501231  
**Semester:** Fall 2025  
**Environment:** Kali Linux  

## **📘 Overview**  
This project implements a **Network Monitor System** in **C++** using **custom stacks and queues** for real-time network packet capture, dissection, filtering, and replay.  
It leverages **raw sockets** on Linux and demonstrates the integration of data structures with network-level programming.  


## **⚙️ Compilation and Execution**

### **Compilation**
```bash
make
```

### **Running**
```bash
sudo ./network_monitor
```


## **🧩 Requirements**
- Linux system (tested on **Kali Linux**)  
- Root privileges for raw socket access  
- Network interface (default: **eth0**)  
  - *(Change the interface name in `main.cpp` if your system uses a different one)*  


## **🛠️ Features**

### **1. Continuous Packet Capture**
- Captures packets live from a single network interface using raw sockets.  
- Stores packets with **unique ID, timestamp, source/destination IP, and size**.  

### **2. Packet Dissection**
- Supports detailed dissection for the following protocols:  
  - **Ethernet**  
  - **IPv4**  
  - **IPv6**  
  - **TCP**  
  - **UDP**  
- Extracts key header fields such as MAC addresses, IPs, and ports.  

### **3. Filtering System**
- Filters packets by **source** and **destination IP addresses**.  
- Skips oversized packets (>1500 bytes) after exceeding a threshold.  

### **4. Replay and Delay Estimation**
- Replays filtered packets with estimated delay:  
  Total Delay = Packet Size / 1000 (ms)
- Maintains a continuous replay loop for matched packets.  

### **5. Error Handling and Backup Queue**
- Implements a **retry mechanism (max 2 retries)** for failed packet replays.  
- Stores failed packets in a **backup queue** for future attempts.  

## **🧪 Test Cases**

### ✅ **1-Minute Continuous Capture**
Captures packets continuously for at least **1 minute** without packet loss.  

### ✅ **Packet Dissection**
Dissects all captured packets layer by layer (Ethernet → IP → TCP/UDP).  

### ✅ **Filtering**
Filters and isolates packets between user-specified IP addresses.  

### ✅ **Replay Functionality**
Successfully replays filtered packets with delay estimation.  

### ✅ **Error Handling**
Handles replay errors gracefully and uses backup queue to reattempt failed packets.  


## **📂 Project Structure**

```
Network_Monitor/
│
├── main.cpp                  # Main implementation file
├── network_monitor.cpp        # Network logic and packet management
├── Makefile                   # For easy compilation
├── README.md                  # Project documentation
└── report.pdf                 # Detailed academic report (for LMS submission)
```


## **🧠 Concepts Demonstrated**
- **Stacks:** Used for protocol layer parsing (LIFO behavior).  
- **Queues:** Used for packet management and continuous buffering (FIFO).  
- **Raw Socket Programming:** Direct interaction with network layers.  
- **Algorithmic Efficiency:** Smooth handling of real-time packet flow.  


## **📈 Sample Output**
```
[Capture Started on eth0]
Captured Packet ID: 12
Timestamp: 2025-10-25 20:32:19
Source IP: 192.168.1.5
Destination IP: 192.168.1.9
Protocol: TCP
Size: 1400 bytes

Replaying Packet ID 12... Delay = 1.4 ms
Replay Successful.
```


## **📚 Learning Outcomes**
Through this project, I:
- Strengthened my understanding of **data structures** and their real-time applications.  
- Applied **stack and queue logic** to real-world **network monitoring**.  
- Learned to use **raw sockets** for low-level packet manipulation.  
- Gained experience handling **network errors, delays, and replay mechanisms**.  



**© 2025 – Anas Norani | NUST BS Data Science**
