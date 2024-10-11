#include <iostream>
#include <string>
#include <winsock2.h>  // Windows socketͷ�ļ�
#include <ws2tcpip.h>  // Windows socket���TCPIP ͷ�ļ�

#pragma comment(lib��"Ws2_32.lib")  // ����Ws2_32.lib��

#define PORT 9010  //����˿ں�
#define BUFFER_SIZE 1024  // ���建������С
#define IP "10.140.32.106"  // ���������IP��ַ��ע����Ҫ����˫����


int main() {
	WSADATA wsaData;
	SOCKET sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);  // ��ַ���Ȳ���

	// ��ʼ��winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed." << std::endl;
		WSACleanup();
		return 1;
	}

	// ����TCP Socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == INVALID_SOCKET) {
		std::cerr << "Socket creation failed:" << WSAGetLastError() << std::endl;
		WSACleanup();
		return 1;
	}

	// ���÷�������ַ
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	// ʹ��inet_pton��IP��ַ���ַ���ת��Ϊ�����ֽ�˳��
	if (inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address or Address not supported" << std::endl;
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}
	
	// ���������������
    if (connect(sockfd, (sockaddr*)&server_addr, addr_len) == SOCKET_ERROR)
    {
        std::cerr << "���ӵ���������ʧ��" << std::endl;
        closesocket(clientsocket);
        WSACleanup();
        return 1;
    }

	while (true) {
		// ������Ϣ��������
		std::string message;
		std::cout << "Enter message: ";
		std::getline(std::cin, message);

		// ������Ϣ
		int send_result = send(sockfd, message.c_str(), message.size(), 0);
		if (send_result == SOCKET_ERROR) {
			std::cerr << "send failed:" << WSAGetLastError() << std::endl;
			break;
		}
		
		// Check if the user wants to quit
        if (message == "quit")
        {
            break;
        }

		// ���շ���������Ӧ
		int n = recv(sockfd, buffer, BUFFER_SIZE, 0);
		if (n == SOCKET_ERROR) {
			std::cerr << "recv failed: " << WSAGetLastError() << std::endl;
			break;
		}

		buffer[n] = '\0';  // ����ַ���������
		std::cout << "Server: " << buffer << std::endl;  // �����������Ӧ
	}

	// �ر�socket
	closesocket(sockfd);

	// ����winsock
	WSACleanup();

	return 0;
}