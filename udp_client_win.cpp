#include <iostream>
#include <string>
#include <winsock2.h>  // Windows socket头文件
#include <ws2tcpip.h>  // Windows socket相关TCPIP 头文件

#pragma comment(lib，"Ws2_32.lib")  // 链接Ws2_32.lib库

#define PORT 9020  //定义端口号
#define BUFFER_SIZE 1024  // 定义缓冲区大小
#define IP "xxx.xxx.xxx.xxx"  // 定义服务器IP地址，注意需要加上双引号


int main() {
	WSADATA wsaData;
	SOCKET sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);  // 地址长度参数

	// 初始化winsock
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed." << std::endl;
		WSACleanup();
		return 1;
	}

	// 创建UDP Socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd == INVALID_SOCKET) {
		std::cerr << "Socket creation failed:" << WSAGetLastError() << std::endl;
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}

	// 配置服务器地址
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	// 使用inet_pton将IP地址从字符串转换为网络字节顺序
	if (inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address or Address not supported" << std::endl;
		closesocket(sockfd);
		WSACleanup();
		return 1;
	}

	while (true) {
		// 发送消息到服务器
		std::string message;
		std::cout << "Enter message: ";
		std::getline(std::cin, message);
		
		// Check if the user wants to quit
        if (message == "quit")
        {
            break;
        }

		// 发送消息
		int send_result = sendto(sockfd, message.c_str(), message.size(), 0, (const struct sockaddr*)&server_addr, addr_len);
		if (send_result == SOCKET_ERROR) {
			std::cerr << "sendto failed:" << WSAGetLastError() << std::endl;
			closesocket(clientsocket);
			WSACleanup();
			break;
		}

		// 接收服务器的响应
		int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_len);
		if (n == SOCKET_ERROR) {
			std::cerr << "recvfrom failed: " << WSAGetLastError() << std::endl;
			closesocket(clientsocket);
			WSACleanup();
			break;
		}

		buffer[n] = '\0';  // 添加字符串结束符
		std::cout << "Server: " << buffer << std::endl;  // 输出服务器响应
	}

	// 关闭socket
	closesocket(sockfd);

	// 清理winsock
	WSACleanup();

	return 0;
}
