#include <stdio.h>

typedef struct
{
    int rollno;
    char name[50];

    int mark1;
    int mark2;
    int mark3;

} Student;

void printRollNo(int n)
{
    if (n == 0)
        return;

    printRollNo(n - 1);

    printf("%d ", n);
}

int getsum(int mark1, int mark2, int mark3)
{
    return mark1 + mark2 + mark3;
}

float getavg(int totalmarks)
{
    return totalmarks / 3;
}

char getgrade(float avg)
{
    if (avg >= 85)
        return 'A';
    else if (avg >= 70)
        return 'B';
    else if (avg >= 50)
        return 'C';
    else if (avg >= 35)
        return 'D';
    else if (avg < 35)
        return 'F';
}

int main()
{
    int n;
    printf("Enter no. of students : ");
    scanf("%d", &n);

    Student arr[n];

    for (int i = 0; i < n; i++)
    {
        Student s1;
        scanf("%d", &s1.rollno);

        scanf("%s", s1.name);

        scanf("%d %d %d", &s1.mark1, &s1.mark2, &s1.mark3);

        arr[i] = s1;
    }

    // printing output
    for (int i = 0; i < n; i++)
    {
        // print output
        Student s1 = arr[i];

        int totalmarks = getsum(s1.mark1, s1.mark2, s1.mark3);

        float avgmarks = getavg(totalmarks);

        char grade = getgrade(avgmarks);

        printf("Roll no: %d\n", s1.rollno);
        printf("Name : %s\n", s1.name);
        printf("Total : %d\n", totalmarks);
        printf("Average : %f\n", avgmarks);
        printf("Grade : %c\n", grade);

        if (grade == 'F')
            continue;
        else if (grade == 'A')
        {
            printf("Performance : *****\n");
        }
        else if (grade == 'B')
        {
            printf("Performance : ****\n");
        }
        else if (grade == 'C')
        {
            printf("Performance : ***\n");
        }
        else if (grade == 'D')
        {
            printf("Performance : **\n");
        }
    }

    printf("List of Roll Numbers : ");

    printRollNo(n);
}