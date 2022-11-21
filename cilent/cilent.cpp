#include"cilent01.h"
SOCKET clientSocket;
char clientIp[36];
struct chat {
	char content[MAXCONTENT];	//内容
	int type;		//0 群聊 1私聊
	int user_Id;	//发送方
	int toUser_Id;		//目标用户id（primary key）
	char userIp[32];
};

struct chatInUser {
	char userIp[32];
	char userName[MAXNAME];
	int userId;
	int client_index;
};

//存放所有连接用户
list<chatInUser> chatAllUser;


void getchatInUser(chatInUser* User, char* userIp, char* userName, int userId, int client_index) {
	memset(User->userIp, 0, sizeof(User->userIp));
	memset(User->userIp, 0, sizeof(User->userName));
	strcpy(User->userName, userName);
	strcpy(User->userName, userName);
	User->userId = userId;
	User->client_index = client_index;
}

chat getChat(char* content, int type, int userId, int toUserId, char* userIp) {
	chat cht;
	//将空的位置全部补上\0，否则传过去乱码
	memset(cht.content, 0, sizeof(cht.content));
	memset(cht.userIp, 0, sizeof(cht.userIp));

	strcpy(cht.content, content);

	cht.type = type;
	cht.user_Id = userId;
	cht.toUser_Id = toUserId;
	strcpy(cht.userIp, userIp);


	return cht;
}

void printChat(chat cht) {
	printf("content: %s\n", cht.content);
	printf("type: %d\n", cht.type);
	printf("userId:%d\n", cht.user_Id);
	printf("toUser: %d\n", cht.toUser_Id);
	printf("userIp: %s\n", cht.userIp);
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

void getServerProtocolAddressFamily(SOCKADDR_IN* addr) {
	(*addr).sin_family = AF_INET;//协议地址簇
//将ip从字符串类型转换成网络字节序类型
	(*addr).sin_addr.S_un.S_addr = inet_addr("");//换成公网ip       原103.46.128.45
	//端口号 htons将整型变量从主机字节顺序转变成网络字节顺序，小端转大端
	(*addr).sin_port = htons(21682);
}

void getClientIp() {
	SOCKADDR_IN name;
	int len2 = sizeof(name);
	getsockname(clientSocket, (sockaddr*)&name, &len2);
	strcpy(clientIp, inet_ntoa(name.sin_addr));
	//连接成功就发送自己的ip，让服务端知道哪一个ip连接了
	send(clientSocket, clientIp, sizeof(clientIp), NULL);

}

void getUserProfile(char* userId, char* userName) {
	
	print_id:printf("请输入你的ID(最大为10个数字): ");
	int r = scanf("%s", userId);
	r = getchar();//吸收换行符
	if (strlen(userId) < 10) {
		send(clientSocket, userId, strlen(userId) * sizeof(int), NULL);
	}
	else {
		printf("字数超过限制\n");
		goto print_id;
		exit(0);
	}
	printf("请输入你的昵称：");
	r = scanf("%s", userName);
	r = getchar();
	if (strlen(userName) < 20) {
		send(clientSocket, userName, strlen(userName) * sizeof(char) * 2, NULL);
	}
	else {
		printf("字数超过限制\n");
		system("pause");
		exit(0);
	}
}


void recvfromServer() {
	int flag = 1;	//除非连接成功，否则只显示一次未连接
	while (1) {
		char buff[1024] = { 0 };
		int r = recv(clientSocket, buff, sizeof(buff), NULL);
		if (r > 0) {
			flag = 1;
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
			printf("\n%s\n\n", buff);
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		}
		else {
			if (flag == 0)
				continue;
			flag = 0;
			printf("未连接状态...\n");

		}

	}
}


int test02() {

	//1 确定协议版本
	WSADATA wsaData;
	getProtocolVersion(&wsaData);

	//2 创建socket
	createSocket(&clientSocket);


	//3 获取服务器协议地址簇
	SOCKADDR_IN addr = { 0 };
	getServerProtocolAddressFamily(&addr);

	//4 连接服务器
	int r = connect(clientSocket, (struct sockaddr*)&addr, sizeof(addr));
	if (-1 == r) {
		printf("连接服务器失败:%d\n", GetLastError());
		//6 关闭socket
		closesocket(clientSocket);
		//7 清理协议版本信息
		WSACleanup();
		system("pause");
		exit(0);
	}
	printf("连接服务器成功！\n");

	//得到客户端ip并发送
	getClientIp();
	//	printf("my ip:%s \n\n", clientIp);

		//得到用户ID和昵称并发送
	char userId[20] = { 0 }, userName[MAXNAME] = { 0 };
	getUserProfile(userId, userName);

	//5 通信
	//创建一个线程来接受服务端的信息
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)recvfromServer, NULL, NULL, NULL);

	while (1) {
		char buff[MAXCONTENT] = { 0 };

		//		printf("请输入你的消息内容：\n");
		//接受消息之前的输入的颜色设置，进程阻塞在get_s，设置可能在其它线程被改变
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		gets_s(buff);//接受用户输入
		if (strlen(buff) < MAXCONTENT) {
			//			printf("my ip is:%s\n", clientIp);
			chat cht = getChat(buff, 0, atoi(userId), 0, clientIp);
			//			printChat(cht);
						//cht 正确
			char snd_buf[MAXCONTENT * 2] = { 0 };
			//注意cht中的空位置要赋\0
			memcpy(snd_buf, &cht, sizeof(cht));
			//for (int i = 0; i < MAXCONTENT * 2; i++) {
			//	printf("%c", snd_buf[i]);
			//}
			//strlen遇到\0会截止，所以直接全部传过去
			send(clientSocket, snd_buf, MAXCONTENT * 2, NULL);//发送给服务器
//			printf("发送成功\n\n");

		}
		else {
			printf("输入超过限制！\n");
			fflush(stdin);
		}
	}

	while (1);//停顿
	
	return 0;
}