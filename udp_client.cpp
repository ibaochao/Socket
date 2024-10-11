#include <iostream>
#include <string>
#include <cstring>  // ʹ��strerror����
#include <sys/socket.h>  // Linux socketͷ�ļ�
#include <arpa/inet.h>  // ��ַ��� sockaddr_in and inet_ntoa
#include <unistd.h>  // close����


#define PORT 9020  // ����˿ں�
#define BUFFER_SIZE 1024  // ���建������С
#define IP "10.140.32.106"  // ���������IP��ַ


int main() {
	int sockfd;
	char buffer[BUFFER_SIZE];
	struct sockaddr_in server_addr;
	socklen_t addr_len = sizeof(server_addr);  // ��ַ���Ȳ���
	
	// ����UDP Socket
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0 ){
		std::cerr << "Socket creation failed: " << strerror(errno) << std::endl;
		return 1;
	}
	
	// ���÷�������ַ
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);
	
	// ʹ��inetpton��IP��ַ���ַ���ת��Ϊ�����ֽ�˳��
	if (inet_pton(AF_INET, IP, &server_addr.sin_addr) <= 0) {
		std::cerr << "Invalid address or Address not supported" << std::endl;
		close(sockfd);
		return 1;
	}
	
	while(true) {
		// ������Ϣ��������
		std::string message;
		std::cout << "Enter message: ";
		std::getline(std::cin, message);
		
		// Check if the user wants to quit
        if (message == "quit")
        {
            break;
        }
		
		// ������Ϣ
		int send_result = sendto(sockfd, message.c_str(), message.size(), 0, (const struct sockaddr*)&server_addr, addr_len);
		if (send_result < 0){
			std::cerr << "sendto failed: " << strerror(errno) << std::endl;
			break;
		}
		
		// ���շ���������Ӧ
		int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_len);
		if(n<0){
			std::cerr << "recvfrom failed:"  << strerror(errno) << std::endl;
			break;
		}
		
		buffer[n]='\0';  // ����ַ���������
		std::cout <<"server:"<< buffer << std::endl;  // �����������Ӧ
	}
	
	//�ر�socket
	close(sockfd);
	
	return 0;
}