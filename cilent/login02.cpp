
#include"cilent01.h"
#include"login01.h"

//此系统能存放最多的用户数

typedef struct User
{
    char name[10];
    char password[10];
} ;
//定义一个存放用户的结构体

User list01[USER_MAX];
//定义一个全局数组，类型是User,大小是USER_MAX

const char* filename = "user.txt";
//需要在你的程序目录下新建一个user.txt文件


//登录函数
User login()
{
    char name[10];
    char password[10];
    User lg;
    printf("输入用户名");
    scanf("%s", name);
    strcpy(lg.name, name);
    printf("输入密码");
    scanf("%s", password);
    strcpy(lg.password, password);
    return lg;
}

//写入txt文件，每一行存在一个用户
void writeToFile(User u)
{
    FILE* fw = fopen(filename, "a+");
    fprintf(fw, u.name);
    fprintf(fw, "\t");
    fprintf(fw, u.password);
    fprintf(fw, "\n");
    fclose(fw);
}

//判断是否存在该用户
int exist(User u)
{
    int i;
    for (i = 0; i < USER_MAX; i++)
    {
        if (0 == strcmp(list01[i].name, u.name) && 0 == strcmp(list01[i].password, u.password))
        {
            return 1;
        }
    }
    return -1;
}

//注册用户
void registerUser()
{
    char name[10];
    char password[10];
    User user;
    int i;

    printf("输入你的用户名：");
    scanf("%s", name);
    strcpy(user.name, name);

    //判断注册的用户是否已存在
    for (i = 0; i < USER_MAX; i++)
    {
        if (0 == strcmp(list01[i].name, name))
        {
            printf("用户已存在！\n");
            test();
            
        }
    }

    printf("输入密码：");
    scanf("%s", password);
    strcpy(user.password, password);
    writeToFile(user);
    printf("注册成功！");
    system("pause");
    system("cls");
    test();
  
}

int menu()
{
    while (true)
    {
        int choice;
        printf("1.登陆\n");
        printf("2.注册\n");
        printf("0.退出\n");
        printf("请选择你的操作:");
        scanf("%d", &choice);
        return choice;
    }
}
int judge(int choice)
{
    if (1 == choice)
    {
        system("cls");
        User u = login();
        if (1 == exist(u))
        {
            printf("登陆成功");
            test02();//启动聊天客户端
            return 1;
        }
        else
        {
            printf("没有该用户或密码错误,请重新输入\n");
            system("pause");
            system("cls");
            test();
        }

    }
    else if (2 == choice)
    {
        system("cls");
        registerUser();
    }
    else if (0==choice)
    {
        return 0;
    }
    else
    {
        printf("请重新选择");
        system("pause");
        system("cls");
        int choice = menu();
        judge(choice);
    }
}



int test()
{
    User test[USER_MAX];
    FILE* fp = fopen(filename, "r");
    int i = 0;

    if (NULL == fp)
    {
        printf("没找到文件");
        return -1;
    }
    for (i = 0; i < USER_MAX; i++)
    {
        char uname[1024];
        char upassword[1024];
        fscanf(fp, "%s%s", uname, upassword);

        strcpy(list01[i].name, uname);
        strcpy(list01[i].password, upassword);
    }

    int choice = menu();
    judge(choice);
}
