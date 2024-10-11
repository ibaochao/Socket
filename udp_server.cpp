#include <iostream>
#include <string>
#include <cstring>  // memset����
#include <sys/socket.h>  // Linux socketͷ�ļ�
#include <arpa/inet.h>  // ��ַ��� sockaddr_in and inet_ntoa
#include <unistd.h>  // close����


#define PORT 9020
#define BUFFER_SIZE 1024


int main(){
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);
	
	// ����UDP Socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0){
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	
	// ���÷�������ַ
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;  // IPV4
	server_addr.sin_addr.s_addr = INADDR_ANY;  // �󶨵���������IP��ַ, ���ܶ������
	server_addr.sin_port = htons(PORT);  // ָ���˿ںţ�ת�����
	
	// ��Socket����ַ
	if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("Bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	std::cout << "UDP server is listening on port " << PORT << std::endl;
	
	while (true){
		//������Ϣ
		int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
		buffer[n]= '\0';  // �����յ�������ת��Ϊ�ַ�����ʽ
		std::cout << "Client: " << buffer << std::endl;

		//��Ӧ��Ϣ
		std::string temp = "Message received: " + std::string(buffer, n);
		const char *response = temp.c_str();
		sendto(sockfd, response, strlen(response), 0, (const struct sockaddr *)&client_addr, addr_len);
	}
	
	close(sockfd);
	return 0;
}