CC=g++

TARGET = client.exe

all: $(TARGET)

client.exe: client.cpp
	$(CC) -std=c++11 client.cpp -o client.exe -lwsock32

clean:
	rm $(TARGET)