
#include <stdio.h>
#include <string.h>
#include <list>
#include<WinSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include<windows.h>
using namespace std;
#pragma warning(disable : 4996)
#define MAXCONTENT 256
#define MAXNAME 32
#pragma once
#pragma warning(disable : 4996)

void getProtocolVersion(WSADATA* wsaData);
void createSocket(SOCKET* soc);
void getServerProtocolAddressFamily(SOCKADDR_IN* addr);
void getClientIp();
void getUserProfile(char* userId, char* userName);
void recvfromServer();
int test02();
#pragma once