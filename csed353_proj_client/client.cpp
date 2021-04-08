#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#include <string.h>


#define PACKET_SIZE 1024

using namespace std;

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
	
	int conn;
	conn = connect(s, (LPSOCKADDR)&addr, sizeof(addr));
	// 성공 0
	char file_name[PACKET_SIZE] = "";

	int send_case;
	/*cout << "You have entered " << argc
		<< " arguments:" << "\n";

	for (int i = 0; i < argc; ++i)
		cout << argv[i] << "\n";
	*/
	
	if (argc < 2) {
		cout << "option error" << endl;
		return -1;
	}
	else {
		if (strcmp(argv[1], "-t") == 0) {
			send_case = 0;
			strcpy(file_name, argv[2]);
		}
		else if (strcmp(argv[1], "-l") == 0) {
			send_case = 1;
		}
		else {
			cout << "option error" << endl;
			return -1;
		}
	}
	int sendBytes;
	long file_size;
	char buf[PACKET_SIZE];
	FILE* fp;
	if (!conn) {
		switch (send_case) {
		case(0):
			snprintf(buf, sizeof(buf), "%d", 0);
			send(s, buf, sizeof(buf), 0);

			
			snprintf(buf, sizeof(buf), "%s", file_name);
			send(s, buf, sizeof(buf), 0);
	
			fp = fopen(file_name, "rb"); //파일열기 
			fseek(fp, 0, SEEK_END); //끝으로가서 
			file_size = ftell(fp); //사이즈재고 
			fseek(fp, 0, SEEK_SET); //처음으로와서 
			snprintf(buf, sizeof(buf), "%d", file_size); //사이즈값을 buf에다가넣기 
			send(s, buf, sizeof(buf), 0); //사이즈값전송 
			while ((sendBytes = fread(buf, sizeof(char), sizeof(buf), fp)) > 0) send(s, buf, sendBytes, 0);
			//파일이끝날때까지 읽고 보내고를 반복 
			fclose(fp);
			cout << "sended file : " << file_name << endl;
			break;
		case(1):
			snprintf(buf, sizeof(buf), "%d", 1);
			send(s, buf, sizeof(buf), 0);
			char file_names[PACKET_SIZE];
			recv(s, buf, PACKET_SIZE, 0);
			strcpy(file_names, buf);
			cout << "file_list : " << file_names << endl;
			break;
		}
	}
	else {
		cout << "Server closed " << endl;
		return -1;
	}


	closesocket(s);
	WSACleanup();
	cin.get();
	return 0;
}