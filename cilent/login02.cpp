
#include"cilent01.h"
#include"login01.h"

//��ϵͳ�ܴ�������û���

typedef struct User
{
    char name[10];
    char password[10];
} ;
//����һ������û��Ľṹ��

User list01[USER_MAX];
//����һ��ȫ�����飬������User,��С��USER_MAX

const char* filename = "user.txt";
//��Ҫ����ĳ���Ŀ¼���½�һ��user.txt�ļ�


//��¼����
User login()
{
    char name[10];
    char password[10];
    User lg;
    printf("�����û���");
    scanf("%s", name);
    strcpy(lg.name, name);
    printf("��������");
    scanf("%s", password);
    strcpy(lg.password, password);
    return lg;
}

//д��txt�ļ���ÿһ�д���һ���û�
void writeToFile(User u)
{
    FILE* fw = fopen(filename, "a+");
    fprintf(fw, u.name);
    fprintf(fw, "\t");
    fprintf(fw, u.password);
    fprintf(fw, "\n");
    fclose(fw);
}

//�ж��Ƿ���ڸ��û�
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

//ע���û�
void registerUser()
{
    char name[10];
    char password[10];
    User user;
    int i;

    printf("��������û�����");
    scanf("%s", name);
    strcpy(user.name, name);

    //�ж�ע����û��Ƿ��Ѵ���
    for (i = 0; i < USER_MAX; i++)
    {
        if (0 == strcmp(list01[i].name, name))
        {
            printf("�û��Ѵ��ڣ�\n");
            test();
            
        }
    }

    printf("�������룺");
    scanf("%s", password);
    strcpy(user.password, password);
    writeToFile(user);
    printf("ע��ɹ���");
    system("pause");
    system("cls");
    test();
  
}

int menu()
{
    while (true)
    {
        int choice;
        printf("1.��½\n");
        printf("2.ע��\n");
        printf("0.�˳�\n");
        printf("��ѡ����Ĳ���:");
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
            printf("��½�ɹ�");
            test02();//��������ͻ���
            return 1;
        }
        else
        {
            printf("û�и��û����������,����������\n");
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
        printf("������ѡ��");
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
        printf("û�ҵ��ļ�");
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
