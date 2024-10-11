#include <iostream>  // 引入输入输出库
#include <sys/socket.h>  // 引入 Socket 库
#include <netinet/in.h>  // 引入 Internet 地址族
#include <unistd.h>  // 引入UNIX标准库
#include <cstring>  // 引入字符串操作库，用于 memset()
#include <pthread.h>
#include <string>
#include <vector>

#define PORT 9010  // 服务器监听的端口号
#define BUFFER_SIZE 1024

std::vector<int> v0;
std::vector<pthread_t> v1;


void* fun(void* arg){
	int* new_socket = (int*)arg;
	while(true){
		char buffer[BUFFER_SIZE] = {0};  // 创建接收缓冲区
		ssize_t bytes_read = recv(*new_socket, buffer, sizeof(buffer), 0); // 接收数据
		
		// 检查是否成功接收
		if (bytes_read > 0) {
			if(std::string(buffer)=="quit") break;
			std::cout <<"Client: "<< buffer << std::endl;
			std::string temp = "Message received: " + std::string(buffer, bytes_read);
			const char *response = temp.c_str();
			send(*new_socket, response, strlen(response), 0);  // 发送数据
		}
	}
	close(*new_socket);  // 关闭客户端 Socket
	return nullptr;
}


int main(){
	int server_fd, new_socket;  // 声明服务器 Socket 和新的客户端 Socket
	struct sockaddr_in address;  // 声明用于存储地址信息的结构体
	int opt = 1;  // 设置选项
	int addrlen = sizeof(address);  // 地址结构体的大小 
	
	// 创建TCP Socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	// 检查 Socket 创建是否成功
	if (server_fd == 0) {
		std::cerr <<"Socket creation failed!"<< std::endl; // 输出错误信息
		return 1;  // 退出程序
	}
	
	//设置 Socket 选项，允许重用地址
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	
	// 设置服务器地址信息
	address.sin_family = AF_INET;  // 使用IPv4地址族
	address.sin_addr.s_addr = INADDR_ANY;  // 允许接收来自任何 IP 地址的连接, 此处是指本地服务器的网卡IP
	address.sin_port = htons(PORT);  // 设置监听的端口号
	
	// 将 Socket 绑定到指定地址和端口
	if(bind(server_fd,(struct sockaddr*)&address, sizeof(address)) < 0){
		std::cerr<<"Bind failed!"<< std::endl;  // 输出绑定失败的信息
		close(server_fd);
		return 1;  // 退出程序
	}
	
	// 开始监听传入的连接请求，队列长度为 3
	if(listen(server_fd,3) < 0) {
		std::cerr <<"Listen failed!"<< std::endl;  // 输出监听失败的信息
		close(server_fd);
		return 1;  // 退出程序
	}
	
	std::cout << "TCP Server is listening on port " << PORT << std::endl;  // 输出服务器监听状态

	// 无限循环，接受客户端连接
	while (true){
		new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
		// 检查连接是否成功
		if (new_socket < 0){
			std::cerr <<"Accept failed!"<< std::endl; // 输出接受连接失败的信息
			continue;  // 继续下一次循环
		}
		v0.push_back(new_socket);
		
		pthread_t thread;
		v1.push_back(thread);
	 
		// 新建一个线程与此客户端通信
		int ret = pthread_create(&v1[v1.size()-1], NULL, fun, &v0[v0.size()-1]);
		if (ret != 0) {
			perror("线程创建失败");
			exit(EXIT_FAILURE);
		}
		
	}
	
	for(auto& t: v1){  // 等待所有线程执行完毕
		pthread_join(t, NULL);
	}
	
	close(server_fd);  // 关闭服务器 Socket
	
	return 0;  // 正常退出程序
}
