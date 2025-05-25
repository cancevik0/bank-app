#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "cJSON.h"

#define FILE_NAME "users.json"

typedef struct
{
    char id[7];
    char fullname[100];
    char identification[50];
    char password[50];
    double balance;
} User;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

char *readFile()
{
    FILE *file = fopen(FILE_NAME, "r");
    if (!file)
    {
        file = fopen(FILE_NAME, "w");
        fprintf(file, "[]");
        fclose(file);
        file = fopen(FILE_NAME, "r");
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);
    char *data = malloc(length + 1);
    fread(data, 1, length, file);
    data[length] = '\0';
    fclose(file);
    return data;
}

void writeFile(cJSON *json)
{
    FILE *file = fopen(FILE_NAME, "w");
    char *data = cJSON_Print(json);
    fprintf(file, "%s", data);
    fclose(file);
    free(data);
}

char *generateID(cJSON *users)
{
    static char id[7];
    int unique = 0;
    srand(time(NULL));
    while (!unique)
    {
        sprintf(id, "%06d", rand() % 1000000);
        unique = 1;
        cJSON *user = NULL;
        cJSON_ArrayForEach(user, users)
        {
            if (strcmp(cJSON_GetObjectItem(user, "id")->valuestring, id) == 0)
            {
                unique = 0;
                break;
            }
        }
    }
    return id;
}

void kayitOl()
{
    clearScreen();
    char fullname[100], identification[50], password[50];
    printf("Ad Soyad: ");
    scanf(" %[^\n]", fullname);
    printf("Kimlik No: ");
    scanf("%s", identification);
    printf("Sifre: ");
    scanf("%s", password);

    char *data = readFile();
    cJSON *users = cJSON_Parse(data);
    free(data);

    cJSON *user = cJSON_CreateObject();
    cJSON_AddStringToObject(user, "id", generateID(users));
    cJSON_AddStringToObject(user, "fullname", fullname);
    cJSON_AddStringToObject(user, "identification", identification);
    cJSON_AddStringToObject(user, "password", password);
    cJSON_AddNumberToObject(user, "balance", 1000.0);

    cJSON_AddItemToArray(users, user);
    writeFile(users);
    cJSON_Delete(users);

    printf("Kayit basarili!\n");
}

User *girisYap()
{
    clearScreen();
    char identification[50], password[50];
    printf("Kullanici adi: ");
    scanf("%s", identification);
    printf("Sifre: ");
    scanf("%s", password);

    char *data = readFile();
    cJSON *users = cJSON_Parse(data);
    free(data);

    cJSON *user = NULL;
    cJSON_ArrayForEach(user, users)
    {
        if (strcmp(cJSON_GetObjectItem(user, "identification")->valuestring, identification) == 0 &&
            strcmp(cJSON_GetObjectItem(user, "password")->valuestring, password) == 0)
        {
            User *u = malloc(sizeof(User));
            strcpy(u->id, cJSON_GetObjectItem(user, "id")->valuestring);
            strcpy(u->fullname, cJSON_GetObjectItem(user, "fullname")->valuestring);
            strcpy(u->identification, identification);
            strcpy(u->password, password);
            u->balance = cJSON_GetObjectItem(user, "balance")->valuedouble;
            cJSON_Delete(users);
            return u;
        }
    }
    cJSON_Delete(users);
    printf("Gecersiz kimlik no veya sifre!\n");
    return NULL;
}

void kullaniciMenusu(User *u)
{
    int secim;
    while (1)
    {
        clearScreen();
        printf("Hosgeldiniz, %s!\n\n", u->fullname);
        printf("[1] Bakiye Goruntule\n");
        printf("[2] Para Yatir\n");
        printf("[3] Para Cek\n");
        printf("[4] Para Gonder\n");
        printf("[0] Cikis\n");
        printf("Secim: ");
        scanf("%d", &secim);

        char *data = readFile();
        cJSON *users = cJSON_Parse(data);
        free(data);

        cJSON *user = NULL;
        cJSON_ArrayForEach(user, users)
        {
            if (strcmp(cJSON_GetObjectItem(user, "id")->valuestring, u->id) == 0)
                break;
        }

        if (secim == 1)
        {
            printf("Bakiye: %.2f\n", u->balance);
        }
        else if (secim == 2)
        {
            double miktar;
            printf("Yatirilacak miktar: ");
            scanf("%lf", &miktar);
            u->balance += miktar;
            cJSON_ReplaceItemInObject(user, "balance", cJSON_CreateNumber(u->balance));
            writeFile(users);
            printf("Yeni bakiye: %.2f\n", u->balance);
        }
        else if (secim == 3)
        {
            double miktar;
            printf("Cekilecek miktar: ");
            scanf("%lf", &miktar);
            if (miktar <= u->balance)
            {
                u->balance -= miktar;
                cJSON_ReplaceItemInObject(user, "balance", cJSON_CreateNumber(u->balance));
                writeFile(users);
                printf("Yeni bakiye: %.2f\n", u->balance);
            }
            else
            {
                printf("Yetersiz bakiye!\n");
            }
        }
        else if (secim == 4)
        {
            char hedefID[7];
            double miktar;
            printf("Alici ID: ");
            scanf("%s", hedefID);
            printf("Gonderilecek miktar: ");
            scanf("%lf", &miktar);
            if (miktar <= u->balance)
            {
                cJSON *alici = NULL;
                cJSON_ArrayForEach(alici, users)
                {
                    if (strcmp(cJSON_GetObjectItem(alici, "id")->valuestring, hedefID) == 0)
                        break;
                }
                if (alici)
                {
                    double aliciBakiye = cJSON_GetObjectItem(alici, "balance")->valuedouble + miktar;
                    cJSON_ReplaceItemInObject(alici, "balance", cJSON_CreateNumber(aliciBakiye));
                    u->balance -= miktar;
                    cJSON_ReplaceItemInObject(user, "balance", cJSON_CreateNumber(u->balance));
                    writeFile(users);
                    printf("%s kullanicisina %.2f tutarinda transfer başarili, guncel bakiyeniz: %.2f\n",
                           cJSON_GetObjectItem(alici, "fullname")->valuestring, miktar, u->balance);
                }
                else
                {
                    printf("Alici bulunamadi!\n");
                }
            }
            else
            {
                printf("Yetersiz bakiye!\n");
            }
        }
        else if (secim == 0)
        {
            cJSON_Delete(users);
            free(u);
            break;
        }
        else
        {
            printf("Gecersiz secim!\n");
        }

        cJSON_Delete(users);
        printf("\nDevam etmek icin Enter'a bas...");
        getchar();
        getchar();
    }
}

int main()
{
    while (1)
    {
        clearScreen();
        printf("[1] Kayit Ol\n[2] Giris Yap\n[0] Cikis\nSecim: ");
        int secim;
        scanf("%d", &secim);

        if (secim == 1)
        {
            kayitOl();
        }
        else if (secim == 2)
        {
            User *u = girisYap();
            if (u)
                kullaniciMenusu(u);
        }
        else if (secim == 0)
        {
            break;
        }
        else
        {
            printf("Gecersiz secim!\n");
        }

        printf("\nDevam etmek icin Enter'a bas...");
        getchar();
        getchar();
    }
    return 0;
}


// Developed by naC
