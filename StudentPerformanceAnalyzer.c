#include<stdio.h>


typedef struct {
    int rollno;
    char name[50];
    int mark1;
    int mark2;
    int mark3;
    int totalmarks;
    float avgmarks;
    char grade;
    int performanceStars;
} Student;


void printList(int * initial , int n){
    if(*initial>n)
        return;

    printf("%d ", *initial);

    (*initial)++;

    printList(initial, n);
}

int findsum(int a , int b , int c){
    return a + b + c;
}

float findavg(int totalmarks){
    return (float)totalmarks / 3;
}


char findgrade(float avg){
    if(avg>=85)
        return 'A';
    else if(avg>=70)
        return 'B';
    else if(avg>=50)
        return 'C';
    else if(avg>=35)
        return 'D';
    else if(avg<35)
        return 'F';
}

int findPerformance(char grade){
    if(grade=='A')
        return 5;
    else if(grade=='B')
        return 4;
    else if(grade=='C')
        return 3;
    else if(grade=='D')
        return 2;
    else if(grade=='F')
        return 0;
}

int main(){
    int n;
    printf("Enter no. of students : ");
    scanf("%d", &n);

    Student arr[n];

    for (int i = 0; i < n;i++){
        Student s1;
        scanf("%d", &s1.rollno);
        //take name in input
        scanf("%s", s1.name);
        //take 3 subject marks in input
        scanf("%d %d %d", &s1.mark1, &s1.mark2, &s1.mark3);

        int totalmarks = findsum(s1.mark1, s1.mark2, s1.mark3);
        s1.totalmarks = totalmarks;

        float avgmarks = findavg(totalmarks);
        s1.avgmarks = avgmarks;

        char grade = findgrade(avgmarks);
        s1.grade = grade;

        int performanceStars = findPerformance(grade);
        s1.performanceStars = performanceStars;

        arr[i] = s1;
    }

    //printing output
    for (int i = 0; i < n;i++){
        // print output
        Student s1 = arr[i];

        printf("Roll no: %d\n", s1.rollno);
        printf("Name : %s\n", s1.name);
        printf("Total : %d\n", s1.totalmarks);
        printf("Average : %f\n", s1.avgmarks);
        printf("Grade : %c\n", s1.grade);

        if (s1.performanceStars)
        {
            printf("Performance : ");
            while (s1.performanceStars)
            {
                printf("*");
                s1.performanceStars--;
            }
            printf("\n");
        }
    }

    printf("List of Roll Numbers : ");
    int initial = 1;
    printList(&initial, n);
}