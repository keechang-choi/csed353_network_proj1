#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#include <dirent.h>
#include <iostream>
#pragma comment(lib, "Ws2_32.lib")
#include <tchar.h>
#include <cstring>

using namespace std;

#define PACKET_SIZE 128
#define FILE_MAX 64

int main(int argc, char* argv[]) {
    char* file_list[PACKET_SIZE];
    const std::string folderPath = "./test/";
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

    bind(s, (LPSOCKADDR)&addr, sizeof(addr));

    while (1) {
        if (listen(s, 5) == -1)
            return 0;

        SOCKET client_s;
        SOCKADDR_IN client = {};
        int client_size = sizeof(client);
        ZeroMemory(&client, client_size);
        client_s = accept(s, (SOCKADDR*)&client, &client_size);

        int totalBufferNum;
        int BufferNum = 0;
        int totalReadBytes = 0;
        int readBytes;
        int file_size;
        std::string filenames = "";
        std::string temp_filename;

        char buf[PACKET_SIZE]; 
        char file_name[PACKET_SIZE];
        int rcv_case;
        recv(client_s, buf, PACKET_SIZE, 0);
        rcv_case = atoi(buf);
        switch (rcv_case) {
            case (0):
                recv(client_s, buf, PACKET_SIZE, 0);
                strcpy(file_name, buf);
                FILE* fp;

                temp_filename = folderPath + file_name;
                fp = fopen(temp_filename.c_str(), "wb");  //파일열고
                recv(client_s, buf, PACKET_SIZE, 0); //파일사이즈받기
                file_size = atol(buf);  //char->long변환
                totalBufferNum = file_size / PACKET_SIZE + 1;
                //전체사이즈 = 파일전체사이즈 / 받고있는데이터
                while (BufferNum != totalBufferNum) {
                    readBytes = recv(client_s, buf, PACKET_SIZE, 0);
                    totalReadBytes += readBytes;
                    //데이터와 데이터의크기 받기
                    BufferNum++;
                    fwrite(buf, sizeof(char), readBytes, fp);
                    printf("Task Status: %d/%dByte(s) [%d%%]\n", totalReadBytes, file_size, ((BufferNum * 100) / totalBufferNum));
                    //데이터와 데이터의크기만큼 쓰기
                }
                fclose(fp);
                cout << "received : " << file_name << endl;
                break;
            case (1):
                DIR *dir;
                struct dirent *ent;
                if ((dir = opendir ("./test/")) != NULL) {
                    while ((ent = readdir (dir)) != NULL) {
                        if ( !strcmp( ent->d_name, "."   )) continue;
                        if ( !strcmp( ent->d_name, ".."  )) continue;
                        filenames += ent->d_name;
                        filenames += " ";
                    }
                    closedir (dir);
                } else {
                /* could not open directory */
                    perror ("");
                    return EXIT_FAILURE;
                }
                
                snprintf(buf, sizeof(buf), "%s", filenames.c_str());  //사이즈값을 buf에다가넣기
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
