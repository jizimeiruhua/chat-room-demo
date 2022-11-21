#pragma once

#include <stdio.h>
#include <string.h>
#include <list>

using namespace std;

#pragma warning(disable : 4996)

#define MAXCONTENT 256
#define MAXNAME 32

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


void getchatInUser(chatInUser* User, char* userIp, char* userName, int userId, int client_index){
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