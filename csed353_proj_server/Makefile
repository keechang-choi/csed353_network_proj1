CC=g++

TARGET = server.exe

all: $(TARGET)

server.exe: server.cpp
	$(CC) -std=c++11 server.cpp -o server.exe -lwsock32

clean:
	rm $(TARGET)