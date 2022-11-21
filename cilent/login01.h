#ifndef LOGIN_H
#define LOGIN_H
#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#define USER_MAX 20
typedef struct User;
void reprintf();
void registerUser();
void writeToFile();
int menu();
int test();
#endif