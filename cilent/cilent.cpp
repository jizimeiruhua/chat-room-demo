#include"cilent01.h"
SOCKET clientSocket;
char clientIp[36];
struct chat {
	char content[MAXCONTENT];	//����
	int type;		//0 Ⱥ�� 1˽��
	int user_Id;	//���ͷ�
	int toUser_Id;		//Ŀ���û�id��primary key��
	char userIp[32];
};

struct chatInUser {
	char userIp[32];
	char userName[MAXNAME];
	int userId;
	int client_index;
};

//������������û�
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
	//���յ�λ��ȫ������\0�����򴫹�ȥ����
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
		printf("ȷ��Э��汾ʧ��!\n");
		system("pause");
		exit(0);
	}
	printf("ȷ��Э��汾�ɹ�!\n");
}

void createSocket(SOCKET* soc) {
	//AF_INET ѡ��tcpЭ��
	//SOCK_STREAM��tcp��Ӧ���ֽ������壬IPPROTO_TCP��tcp��ip������ʽ
	*soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == *soc) {
		printf("����socketʧ��:%d\n", GetLastError());
		//9 ����Э��汾��Ϣ
		WSACleanup();
		system("pause");
		exit(0);
	}
	printf("����socket�ɹ�!\n");
}

void getServerProtocolAddressFamily(SOCKADDR_IN* addr) {
	(*addr).sin_family = AF_INET;//Э���ַ��
//��ip���ַ�������ת���������ֽ�������
	(*addr).sin_addr.S_un.S_addr = inet_addr("");//���ɹ���ip       ԭ103.46.128.45
	//�˿ں� htons�����ͱ����������ֽ�˳��ת��������ֽ�˳��С��ת���
	(*addr).sin_port = htons(21682);
}

void getClientIp() {
	SOCKADDR_IN name;
	int len2 = sizeof(name);
	getsockname(clientSocket, (sockaddr*)&name, &len2);
	strcpy(clientIp, inet_ntoa(name.sin_addr));
	//���ӳɹ��ͷ����Լ���ip���÷����֪����һ��ip������
	send(clientSocket, clientIp, sizeof(clientIp), NULL);

}

void getUserProfile(char* userId, char* userName) {
	
	print_id:printf("���������ID(���Ϊ10������): ");
	int r = scanf("%s", userId);
	r = getchar();//���ջ��з�
	if (strlen(userId) < 10) {
		send(clientSocket, userId, strlen(userId) * sizeof(int), NULL);
	}
	else {
		printf("������������\n");
		goto print_id;
		exit(0);
	}
	printf("����������ǳƣ�");
	r = scanf("%s", userName);
	r = getchar();
	if (strlen(userName) < 20) {
		send(clientSocket, userName, strlen(userName) * sizeof(char) * 2, NULL);
	}
	else {
		printf("������������\n");
		system("pause");
		exit(0);
	}
}


void recvfromServer() {
	int flag = 1;	//�������ӳɹ�������ֻ��ʾһ��δ����
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
			printf("δ����״̬...\n");

		}

	}
}


int test02() {

	//1 ȷ��Э��汾
	WSADATA wsaData;
	getProtocolVersion(&wsaData);

	//2 ����socket
	createSocket(&clientSocket);


	//3 ��ȡ������Э���ַ��
	SOCKADDR_IN addr = { 0 };
	getServerProtocolAddressFamily(&addr);

	//4 ���ӷ�����
	int r = connect(clientSocket, (struct sockaddr*)&addr, sizeof(addr));
	if (-1 == r) {
		printf("���ӷ�����ʧ��:%d\n", GetLastError());
		//6 �ر�socket
		closesocket(clientSocket);
		//7 ����Э��汾��Ϣ
		WSACleanup();
		system("pause");
		exit(0);
	}
	printf("���ӷ������ɹ���\n");

	//�õ��ͻ���ip������
	getClientIp();
	//	printf("my ip:%s \n\n", clientIp);

		//�õ��û�ID���ǳƲ�����
	char userId[20] = { 0 }, userName[MAXNAME] = { 0 };
	getUserProfile(userId, userName);

	//5 ͨ��
	//����һ���߳������ܷ���˵���Ϣ
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)recvfromServer, NULL, NULL, NULL);

	while (1) {
		char buff[MAXCONTENT] = { 0 };

		//		printf("�����������Ϣ���ݣ�\n");
		//������Ϣ֮ǰ���������ɫ���ã�����������get_s�����ÿ����������̱߳��ı�
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
		gets_s(buff);//�����û�����
		if (strlen(buff) < MAXCONTENT) {
			//			printf("my ip is:%s\n", clientIp);
			chat cht = getChat(buff, 0, atoi(userId), 0, clientIp);
			//			printChat(cht);
						//cht ��ȷ
			char snd_buf[MAXCONTENT * 2] = { 0 };
			//ע��cht�еĿ�λ��Ҫ��\0
			memcpy(snd_buf, &cht, sizeof(cht));
			//for (int i = 0; i < MAXCONTENT * 2; i++) {
			//	printf("%c", snd_buf[i]);
			//}
			//strlen����\0���ֹ������ֱ��ȫ������ȥ
			send(clientSocket, snd_buf, MAXCONTENT * 2, NULL);//���͸�������
//			printf("���ͳɹ�\n\n");

		}
		else {
			printf("���볬�����ƣ�\n");
			fflush(stdin);
		}
	}

	while (1);//ͣ��
	
	return 0;
}