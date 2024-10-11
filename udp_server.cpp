#include <iostream>
#include <string>
#include <cstring>  // memset函数
#include <sys/socket.h>  // Linux socket头文件
#include <arpa/inet.h>  // 地址相关 sockaddr_in and inet_ntoa
#include <unistd.h>  // close函数


#define PORT 9020
#define BUFFER_SIZE 1024


int main(){
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	
	// 创建UDP Socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0){
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	// 配置服务器地址
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;  // IPV4
	server_addr.sin_addr.s_addr = INADDR_ANY;  // 绑定到本地所有IP地址, 可能多块网卡
	server_addr.sin_port = htons(PORT);  // 指定端口号，转大端序
	
	// 绑定Socket到地址
	if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	std::cout << "UDP server is listening on port " << PORT << std::endl;
	
	while (true){
		//接收消息
		int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
		buffer[n]= '\0';  // 将接收到的数据转换为字符串格式
		std::cout << "Client: " << buffer << std::endl;

		//响应消息
		std::string temp = "Message received: " + std::string(buffer, n);
		const char *response = temp.c_str();
		sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&client_addr, addr_len);
	}
	
	close(sockfd);
	return 0;
}