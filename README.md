# Socket

小测试

## TCP测试

### 编译

```txt
g++ tcp_server.cpp -o tcp_server -lpthread
g++ tcp_client.cpp -o tcp_client
```

### 测试

Client（Linux）

![image-20241011235225334](./Scoket.assets/image-20241011235225334.png)

Client（Windows）

![image-20241011235317950](./Scoket.assets/image-20241011235317950.png)

TCP Server（Linux）

![image-20241011235326271](./Scoket.assets/image-20241011235326271.png)

## UDP

### 编译

```txt
g++ udp_server.cpp -o udp_server
g++ udp_client.cpp -o udp_client
```

### 测试

Client（Linux）

![image-20241011215020227](./Scoket.assets/image-20241011215020227.png)

Client（Windows）

![image-20241011215140848](./Scoket.assets/image-20241011215140848.png)

UDP Server（Linux）

Windows客户端向Linux服务端发送中文Linux服务器端会显示乱码

![image-20241011215030536](./Scoket.assets/image-20241011215030536.png)

# End
