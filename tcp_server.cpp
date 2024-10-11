#include <iostream>  // �������������
#include <sys/socket.h>  // ���� Socket ��
#include <netinet/in.h>  // ���� Internet ��ַ��
#include <unistd.h>  // ����UNIX��׼��
#include <cstring>  // �����ַ��������⣬���� memset()
#include <pthread.h>
#include <string>
#include <vector>

#define PORT 9010  // �����������Ķ˿ں�
#define BUFFER_SIZE 1024

std::vector<int> v0;
std::vector<pthread_t> v1;


void* fun(void* arg){
	int* new_socket = (int*)arg;
	while(true){
		char buffer[BUFFER_SIZE] = {0};  // �������ջ�����
		ssize_t bytes_read = recv(*new_socket, buffer, sizeof(buffer), 0); // ��������
		
		// ����Ƿ�ɹ�����
		if (bytes_read > 0) {
			if(std::string(buffer)=="quit") break;
			std::cout <<"Client: "<< buffer << std::endl;
			std::string temp = "Message received: " + std::string(buffer, bytes_read);
			const char *response = temp.c_str();
			send(*new_socket, response, strlen(response), 0);  // ��������
		}
	}
	close(*new_socket);  // �رտͻ��� Socket
	return nullptr;
}


int main(){
	int server_fd, new_socket;  // ���������� Socket ���µĿͻ��� Socket
	struct sockaddr_in address;  // �������ڴ洢��ַ��Ϣ�Ľṹ��
	int opt = 1;  // ����ѡ��
	int addrlen = sizeof(address);  // ��ַ�ṹ��Ĵ�С 
	
	// ����TCP Socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	// ��� Socket �����Ƿ�ɹ�
	if (server_fd == 0) {
		std::cerr <<"Socket creation failed!"<< std::endl; // ���������Ϣ
		return 1;  // �˳�����
	}
	
	//���� Socket ѡ��������õ�ַ
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	// ���÷�������ַ��Ϣ
	address.sin_family = AF_INET;  // ʹ��IPv4��ַ��
	address.sin_addr.s_addr = INADDR_ANY;  // ������������κ� IP ��ַ������, �˴���ָ���ط�����������IP
	address.sin_port = htons(PORT);  // ���ü����Ķ˿ں�
	
	// �� Socket �󶨵�ָ����ַ�Ͷ˿�
	if(bind(server_fd,(struct sockaddr*)&address, sizeof(address)) < 0){
		std::cerr<<"Bind failed!"<< std::endl;  // �����ʧ�ܵ���Ϣ
		close(server_fd);
		return 1;  // �˳�����
	}
	
	// ��ʼ����������������󣬶��г���Ϊ 3
	if(listen(server_fd,3) < 0) {
		std::cerr <<"Listen failed!"<< std::endl;  // �������ʧ�ܵ���Ϣ
		close(server_fd);
		return 1;  // �˳�����
	}
	
	std::cout << "TCP Server is listening on port " << PORT << std::endl;  // �������������״̬

	// ����ѭ�������ܿͻ�������
	while (true){
		new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		// ��������Ƿ�ɹ�
		if (new_socket < 0){
			std::cerr <<"Accept failed!"<< std::endl; // �����������ʧ�ܵ���Ϣ
			continue;  // ������һ��ѭ��
		}
		v0.push_back(new_socket);
		
		pthread_t thread;
		v1.push_back(thread);
	 
		// �½�һ���߳���˿ͻ���ͨ��
		int ret = pthread_create(&v1[v1.size()-1], NULL, fun, &v0[v0.size()-1]);
		if (ret != 0) {
			perror("�̴߳���ʧ��");
			exit(EXIT_FAILURE);
		}
		
	}
	
	for(auto& t: v1){  // �ȴ������߳�ִ�����
		pthread_join(t, NULL);
	}
	
	close(server_fd);  // �رշ����� Socket
	
	return 0;  // �����˳�����
}
