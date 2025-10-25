CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = network_monitor
SOURCES = main.cpp
HEADERS = packet.h dissector.h network_monitor.h

$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET)

install:
	sudo cp $(TARGET) /usr/local/bin/

.PHONY: clean install
