#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <string>
#include <ctime>
#include <cstring>

struct Packet {
    int id;
    time_t timestamp;
    char* data;
    int size;
    std::string src_ip;
    std::string dst_ip;
    
    Packet() : id(0), timestamp(0), data(nullptr), size(0) {}
    
    Packet(const Packet& other) : id(other.id), timestamp(other.timestamp), 
                                  size(other.size), src_ip(other.src_ip), dst_ip(other.dst_ip) {
        if (other.data && other.size > 0) {
            data = new char[other.size];
            memcpy(data, other.data, other.size);
        } else {
            data = nullptr;
        }
    }
    
    Packet& operator=(const Packet& other) {
        if (this != &other) {
            if (data) {
                delete[] data;
            }
            id = other.id;
            timestamp = other.timestamp;
            size = other.size;
            src_ip = other.src_ip;
            dst_ip = other.dst_ip;
            if (other.data && other.size > 0) {
                data = new char[other.size];
                memcpy(data, other.data, other.size);
            } else {
                data = nullptr;
            }
        }
        return *this;
    }
    
    ~Packet() {
        if (data) {
            delete[] data;
            data = nullptr;
        }
    }
};

class Stack {
private:
    Packet* items;
    int top;
    int capacity;
    
public:
    Stack(int cap = 1000) : capacity(cap), top(-1) {
        items = new Packet[capacity];
    }
    
    ~Stack() {
        delete[] items;
    }
    
    void push(Packet p) {
        if (top < capacity - 1) {
            top++;
            items[top] = p;
        }
    }
    
    Packet pop() {
        if (top >= 0) {
            Packet p = items[top];
            top--;
            return p;
        }
        return Packet();
    }
    
    bool isEmpty() {
        return top == -1;
    }
    
    Packet peek() {
        if (top >= 0) {
            return items[top];
        }
        return Packet();
    }
};

class Queue {
private:
    Packet* items;
    int front;
    int rear;
    int capacity;
    int count;
    
public:
    Queue(int cap = 1000) : capacity(cap), front(0), rear(-1), count(0) {
        items = new Packet[capacity];
    }
    
    ~Queue() {
        delete[] items;
    }
    
    void enqueue(Packet p) {
        if (count < capacity) {
            rear = (rear + 1) % capacity;
            items[rear] = p;
            count++;
        }
    }
    
    Packet dequeue() {
        if (count > 0) {
            Packet p = items[front];
            front = (front + 1) % capacity;
            count--;
            return p;
        }
        return Packet();
    }
    
    bool isEmpty() {
        return count == 0;
    }
    
    int size() {
        return count;
    }
};

#endif
