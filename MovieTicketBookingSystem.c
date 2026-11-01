#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int main()
{

    const int standardPrice = 150;
    const int premiumPrice = 250;
    const int reclinerPrice = 400;

    while (true)
    {

        char name[50];
        printf("Enter your name : ");
        scanf("%49s", name);

        int count;
        printf("Enter number of tickets : ");
        

        if(scanf("%d", &count) != 1)
        {
            
            printf("Please enter a valid numeric number\n");
           
            break;
        }

        if(count < 1 || count > 20)
        {
            printf("Please enter valid number of tickets\n");
            continue;
        }

        printf("Standard : 150 , Premium : 250 , Recliner : 400 \n");
        printf("Choose 1 for selecting Standard\n");
        printf("Choose 2 for selecting Premium\n");
        printf("Choose 3 for selecting Recliner\n");

        int choice;
        scanf("%d", &choice);

        if (choice < 1 || choice > 3)
        {
            printf("Please enter valid choice\n");
            continue;
        }

        char category[50];

        float totalcost;

        switch (choice)
        {
        case 1:
            strcpy(category, "Standard");
            totalcost = count * standardPrice;
            break;
        case 2:
            strcpy(category, "Premium");
            totalcost = count * premiumPrice;
            break;
        case 3:
            strcpy(category, "Recliner");
            totalcost = count * reclinerPrice;
            break;
        }

        float discount = 0;

        if (count >= 5)
        {
            discount = 10.0 / 100.0 * totalcost;
        }

        printf("Output\n");

        printf("Customer : %s\n", name);
        printf("Ticket Category : %s\n", category);
        printf("Tickets : %d\n", count);
        printf("Subtotal : %.2f\n", totalcost);
        printf("Discount : %.2f\n", discount);
        printf("Final Bill : %.2f\n", totalcost - discount);
    }
}