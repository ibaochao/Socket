#include <iostream>
#include <string>
#include <cstring>  // 使用strerror函数
#include <sys/socket.h>  // Linux socket头文件
#include <arpa/inet.h>  // 地址相关 sockaddr_in and inet_ntoa
#include <unistd.h>  // close函数


#define PORT 9020  // 定义端口号
#define BUFFER_SIZE 1024  // 定义缓冲区大小
#define IP "xxx.xxx.xxx.xxx"  // 定义服务器IP地址


int main() {
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);  // 地址长度参数
	
	// 创建UDP Socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0 ){
		std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
		return 1;
	}
	
	// 配置服务器地址
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	
	// 使用inetpton将IP地址从字符串转换为网络字节顺序
	if (inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address or Address not supported" << std::endl;
		close(sockfd);
		return 1;
	}
	
	while(true) {
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
		if (send_result < 0){
			std::cerr << "sendto failed: " << strerror(errno) << std::endl;
			break;
		}
		
		// 接收服务器的响应
		int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_len);
		if(n<0){
			std::cerr << "recvfrom failed:"  << strerror(errno) << std::endl;
			break;
		}
		
		buffer[n]='\0';  // 添加字符串结束符
		std::cout <<"server:"<< buffer << std::endl;  // 输出服务器响应
	}
	
	//关闭socket
	close(sockfd);
	
	return 0;
}
