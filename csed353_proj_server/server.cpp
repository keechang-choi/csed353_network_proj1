#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#if defined(_WIN32)
#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#pragma comment(lib, "Ws2_32.lib")
#endif

#include <tchar.h>


using namespace std;

#define PACKET_SIZE 1024

int main(int argc, char* argv[])
{
	//socket 정보, 버전, ...
	WSADATA wsa;
	// winsock 관련 라이브러리 초기화, wsa에 저장
	WSAStartup(MAKEWORD(2, 2), &wsa);


	SOCKET s;
	// addr famlily,  socket type,  protocol
	s = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(7777);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
//	InetPton(AF_INET, "127.0.0.1", &(addr.sin_addr.s_addr));

	bind(s, (LPSOCKADDR)& addr, sizeof(addr));

	while (1) {
		if (listen(s, 5) == -1)
			return 0;

		SOCKET client_s;
		SOCKADDR_IN client = {};
		int client_size = sizeof(client);
		ZeroMemory(&client, client_size);
		client_s = accept(s, (SOCKADDR*)& client, &client_size);

		int totalBufferNum;
		int BufferNum = 0;
		int readBytes;
		long file_size;

		char buf[PACKET_SIZE];
		char file_name[PACKET_SIZE];
		int rcv_case;
		recv(client_s, buf, PACKET_SIZE, 0);
		rcv_case = atoi(buf);
		switch (rcv_case) {
		case(0):
			recv(client_s, buf, PACKET_SIZE, 0);
			strcpy_s(file_name, buf);
			FILE* fp;
			fp = fopen(file_name, "wb"); //파일열고 
			recv(client_s, buf, PACKET_SIZE, 0); //파일사이즈받기 
			file_size = atol(buf); //char->long변환 
			totalBufferNum = file_size / PACKET_SIZE + 1;
			//전체사이즈 = 파일전체사이즈 / 받고있는데이터
			while (BufferNum != totalBufferNum) {
				readBytes = recv(client_s, buf, PACKET_SIZE, 0);
				//데이터와 데이터의크기 받기 
				BufferNum++;
				fwrite(buf, sizeof(char), readBytes, fp);
				//데이터와 데이터의크기만큼 쓰기 
			}
			fclose(fp);
			cout << "received : " << file_name << endl;
			break;
		case(1):
			snprintf(buf, sizeof(buf), "%s", "filename1 filename2"); //사이즈값을 buf에다가넣기 
			send(client_s, buf, sizeof(buf), 0);
			cout << "filenames returned" << endl;
			break;
		}
		

		closesocket(client_s);

	}

	
	closesocket(s);
	WSACleanup();
	return 0;
}

