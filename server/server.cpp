#include<stdio.h>
#include<list>

using namespace std;

#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<windows.h>

#include "head.h"

#pragma warning(disable : 4996)

//最大连接数
#define MAXUSER 10
//已连接数
int connNum = 0;

SOCKADDR_IN cAddr[MAXUSER];

SOCKET clientSocket[MAXUSER];

char clientIp[MAXUSER][100] = { 0 };


void getProtocolVersion(WSADATA* wsaData);
void createSocket(SOCKET* soc);
void createServerProtocolAddressFamily(SOCKADDR_IN* addr);
void bindSocketAddress(SOCKET* soc, SOCKADDR_IN* addr);
void listenServer(SOCKET* soc);


int clientConn(int index);
void chatGroup(int index, chat cht);
void chatSomeone(int index, chat cht);
void getNameById(char* userName, int userId);

int clientConn(int index) {
	//7 通信
	while (1) {

		char snd_buf[1024] = { 0 };
		chat cht;
		int r = recv(clientSocket[index], snd_buf, 1024, NULL);
		//		printf("r:%d\n", r);
		if (r > 0) {
			////遇到\0截止了，显示不出后面的
			//printf("ALL: %s\n", snd_buf);
		//for (int i = 0; i < r; i++) {
		//	printf("%c", snd_buf[i]);
		//}
		//	printf("接受成功");
			memcpy(&cht, snd_buf, sizeof(cht));
			printChat(cht);

			//客户端只需知道id即可
			printf("%d发来的消息:%s\n\n", cht.user_Id, cht.content);

			//向其它客户端发送信息
			if (cht.type == 0)
				chatGroup(index, cht);
			else if (cht.type == 1)
				chatSomeone(index, cht);
		}

	}
}

void chatGroup(int index, chat cht) {
	char* str = (char*)malloc(MAXCONTENT * 2);
	if (str != 0) {
		memset(str, 0, sizeof(str));
		char userId[11], userName[16];

		getNameById(userName, cht.user_Id);
		itoa(cht.user_Id, userId, 10);

		strcpy(str, userId);
		strcat(str, userName);
		strcat(str, ":\n");
		strcat(str, cht.content);
		for (int j = 0; j < connNum; j++) {
			if (j != index)
				send(clientSocket[j], str, strlen(str) * sizeof(char), NULL);
		}
	}
}

void chatSomeone(int index, chat cht) {
	int flag = 0;
	list<chatInUser>::iterator iter;
	for (iter = chatAllUser.begin(); iter != chatAllUser.end(); iter++) {
		//找到了对应的联系人
		if (iter->userId == cht.toUser_Id) {
			flag = 1;

			//设置消息格式
			char* str = (char*)malloc(MAXCONTENT * 2);
			if (str != 0) {
				memset(str, 0, sizeof(str));
				char userId[11];
				itoa(cht.user_Id, userId, 10);
				strcpy(str, userId);
				strcat(str, ":\n");
				strcat(str, cht.content);

				send(clientSocket[iter->client_index], str, strlen(str) * sizeof(char), NULL);
				break;
			}
		}
	}
	//未找到该联系人
	if (flag == 0) {
		char reMessage[40] = { 0 };
		strcpy(reMessage, "未找到该联系人");
		printf("\n未找到用户所指定的接收方\n");
		send(clientSocket[index], reMessage, strlen(reMessage), NULL);
	}
}

void getNameById(char* userName, int userId) {
	list<chatInUser>::iterator iter;
	for (iter = chatAllUser.begin(); iter != chatAllUser.end(); iter++) {
		if (iter->userId == userId)
			strcpy(userName, iter->userName);
	}
}

void getProtocolVersion(WSADATA* wsaData) {
	WSAStartup(MAKEWORD(2, 2), wsaData);
	if (LOBYTE((*wsaData).wVersion) != 2 || HIBYTE((*wsaData).wVersion) != 2) {
		printf("确定协议版本失败!\n");
		system("pause");
		exit(0);
	}
	printf("确定协议版本成功!\n");
}

void createSocket(SOCKET* soc) {
	//AF_INET 选择tcp协议
	//SOCK_STREAM是tcp对应的字节流载体，IPPROTO_TCP是tcp的ip保护方式
	*soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == *soc) {
		printf("创建socket失败:%d\n", GetLastError());
		//9 清理协议版本信息
		WSACleanup();
		system("pause");
		exit(0);
	}
	printf("创建socket成功!\n");
}

void createServerProtocolAddressFamily(SOCKADDR_IN* addr) {
	(*addr).sin_family = AF_INET;//协议地址簇
	//将ip从字符串类型转换成网络字节序类型
	(*addr).sin_addr.S_un.S_addr = inet_addr("192.168.43.2");
	//端口号 htons将整型变量从主机字节顺序转变成网络字节顺序，小端转大端
	(*addr).sin_port = htons(9999);
}

void bindSocketAddress(SOCKET* soc, SOCKADDR_IN* addr) {
	int r = bind(*soc, (sockaddr*)addr, sizeof(*addr));
	if (-1 == r) {
		printf("绑定失败：%d\n", GetLastError());
		//8 关闭socket
		closesocket(*soc);

		//9 清理协议版本信息
		WSACleanup();
		system("pause");
		exit(0);
	}
	printf("绑定成功！\n");
}

void listenServer(SOCKET* soc) {
	int r = listen(*soc, 10);//监听数：10
	if (-1 == r) {
		printf("监听失败:%d\n", GetLastError());
		//8 关闭socket
		closesocket(*soc);

		//9 清理协议版本信息
		WSACleanup();
		system("pause");
		exit(0);
	}
	printf("监听成功!\n");
}



int main() {

	//1 确定协议版本
	WSADATA wsaData;
	getProtocolVersion(&wsaData);

	//2 创建socket
	SOCKET serverSocket;
	createSocket(&serverSocket);

	//3 创建服务器协议地址簇
	SOCKADDR_IN addr = { 0 };
	createServerProtocolAddressFamily(&addr);

	//4 绑定
	bindSocketAddress(&serverSocket, &addr);

	//5 监听
	listenServer(&serverSocket);

	//6 等待客户端连接
	for (int i = 0; i < MAXUSER; i++) {
		int len = sizeof(cAddr[i]);
		clientSocket[i] = accept(serverSocket, (struct sockaddr*)(cAddr + i), &len);
		if (SOCKET_ERROR == clientSocket[i]) {
			printf("客户端%s连接失败:%d\n", inet_ntoa(cAddr[i].sin_addr), GetLastError());
			//8 关闭socket
			closesocket(serverSocket);

			//9 清理协议版本信息
			WSACleanup();
			system("pasue");
			exit(0);
		}
		connNum = i + 1;

		//接收客户端的ip
		int r = recv(clientSocket[i], clientIp[i], 99, NULL);
		//if (r > 0) {
		//	clientIp[i][r] = 0;
		//}
		//接受客户端的id
		char userId[30] = { 0 }, userName[MAXNAME] = { 0 };
		r = recv(clientSocket[i], userId, sizeof(userId), NULL);
		r = recv(clientSocket[i], userName, sizeof(userName), NULL);
		chatInUser inUser;
		getchatInUser(&inUser, clientIp[i], userName, atoi(userId), i);
		chatAllUser.push_back(inUser);
		printf("%2d 客户端 %d(userId)%s 连接成功！\n", connNum, atoi(userId), userName);
		printf("%2d 客户端 %s(recvIp) 连接成功！\n", connNum, clientIp[i]);
		printf("%2d 客户端 %s(acceptcAddr) 连接成功！\n\n", connNum, inet_ntoa(cAddr[i].sin_addr));


		//新线程接受客户端的消息
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)clientConn, (LPVOID)i, NULL, NULL);

		if (i == MAXUSER - 1) {
			printf("客户端连接已达上限！\n");
		}
	}


	while (1);//停顿
	return 0;
}