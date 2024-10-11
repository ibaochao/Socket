#include <iostream>
#include <string>
#include <cstring>  // 使用strerror函数
#include <sys/socket.h>  // Linux socket头文件
#include <arpa/inet.h>  // 地址相关 sockaddr_in and inet_ntoa
#include <unistd.h>  // close函数


#define PORT 9010  // 定义端口号
#define BUFFER_SIZE 1024  // 定义缓冲区大小
#define IP "xxx.xxx.xxx.xxx"  // 定义服务器IP地址


int main() {
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);  // 地址长度参数
	
	// 创建UDP Socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0 ){
		std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
		return 1;
	}
	
	// 配置服务器地址
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	server_addr.sin_addr.s_addr = inet_addr(IP);
	
	// 连接到服务器
	if (connect(sockfd, (struct sockaddr*)&server_addr, addr_len) < 0){
		std::cerr <<"Connection failed!" << std::endl;
		close(sockfd);
		return 1;
	}
	
	while(true) {
		
		// 发送消息到服务器
		std::string message;
		std::cout << "Enter message: ";
		std::getline(std::cin, message);
		
		// 发送消息
		int send_result = send(sockfd, message.c_str(), message.size(), 0);
		if (send_result < 0){
			std::cerr << "send failed: " << strerror(errno) << std::endl;
			break;
		}
		
		// Check if the user wants to quit
        if (message == "quit")
        {
            break;
        }
		
		// 接收服务器的响应
		int n = recv(sockfd, buffer, BUFFER_SIZE, 0);
		if(n<0){
			std::cerr << "recv failed:"  << strerror(errno) << std::endl;
			break;
		}
		
		buffer[n]='\0';  // 添加字符串结束符
		std::cout <<"server:"<< buffer << std::endl;  // 输出服务器响应
	}
	
	//关闭socket
	close(sockfd);
	
	return 0;
}
