#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
    int id;
    char name[50];
    int age;
} User;

bool checkid(int target)
{
    FILE *fptr = fopen("users.txt", "r");

    if (fptr == NULL)
    {

        return false;
    }

    int id;
    char name[50];
    int age;

    while (fscanf(fptr, "%d %s %d", &id, name, &age) != EOF)
    {
        if (id == target)
        {
            fclose(fptr);
            return true;
        }
    }

    fclose(fptr);
    return false;
}

void writedata()
{
    FILE *fptr = fopen("users.txt", "a");
    User data;

    printf("Enter Unique user id : ");
    scanf("%d", &data.id);
    printf("\n");

    printf("Enter Name of user : ");
    scanf("%s", data.name);
    printf("\n");

    printf("Enter Age of user : ");
    scanf("%d", &data.age);
    printf("\n");

    bool ispresent = checkid(data.id);
    if (ispresent)
    {
        printf("Duplicate ID entered , kindly enter a unique id !\n");
        return;
    }

    else
        fprintf(fptr, "%d %s %d \n", data.id, data.name, data.age);

    fclose(fptr);

    printf("Data added successfully!\n");
}

void displaydata()
{
    FILE *fptr;

    fptr = fopen("users.txt", "r");

    if (fptr == NULL)
    {
        printf("File is empty !\n");
        return;
    }

    User data;

    printf("UniqueID Name Age \n");

    while (fscanf(fptr, "%d %s %d", &data.id, data.name, &data.age) != EOF)
    {
        printf("%d %s %d \n", data.id, data.name, data.age);
    }

    fclose(fptr);
}

void updatedata()
{
    FILE *fptr = fopen("users.txt", "r");

    if (fptr == NULL)
    {
        printf("File not exist!\n");
        return;
    }

    FILE *tptr = fopen("temp.txt", "a");

    User data;

    int targetid;
    printf("Enter ID where you want to make changes : ");
    scanf("%d", &targetid);

    while (fscanf(fptr, "%d %s %d", &data.id, data.name, &data.age) != EOF)
    {
        if (targetid == data.id)
        {

            char newname[50];
            int newage;
            printf("Enter new name : ");
            scanf("%s", newname);
            printf("Enter new age : ");
            scanf("%d", &newage);

            fprintf(tptr, "%d %s %d\n", targetid, newname, newage);
        }
        else
        {
            fprintf(tptr, "%d %s %d \n", data.id, data.name, data.age);
        }
    }

    fclose(fptr);
    fclose(tptr);

    remove("users.txt");
    rename("temp.txt", "users.txt");
    printf("Data updated successfully!\n");
}

void deletedata()
{
    FILE *fptr = fopen("users.txt", "r");

    if (fptr == NULL)
    {
        printf("File does not exist!");
        return;
    }

    User data;

    FILE *tptr = fopen("temp.txt", "a");

    int targetid;
    printf("Enter ID which needs to be deleted : ");
    scanf("%d", &targetid);

    while (fscanf(fptr, "%d %s %d", &data.id, data.name, &data.age) != EOF)
    {
        if (targetid == data.id)
        {
            continue;
        }

        fprintf(tptr, "%d %s %d \n", data.id, data.name, data.age);
    }

    fclose(fptr);
    fclose(tptr);

    remove("users.txt");
    rename("temp.txt", "users.txt");
    printf("Data deleted successfully!\n");
}

int main()
{

    while (true)
    {
        printf("Select options :\n 1 for writing\n 2 for displaying content\n 3 for updating\n 4 for deleting user\n 5 for exit\n");
        int option;
        scanf("%d", &option);

        if (option == 1)
        {
            writedata();
        }
        else if (option == 2)
        {
            displaydata();
        }
        else if (option == 3)
        {
            updatedata();
        }

        else if (option == 4)
        {
            deletedata();
        }
        else if (option == 5)
            break;
        else
            printf("Enter valid option ! ");
    }
}