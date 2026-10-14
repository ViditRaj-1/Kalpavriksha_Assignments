#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool isValid(char *s)
{
    int n = strlen(s);
    if (n == 0)
        return false;
    
    if (s[n - 1] == '+' || s[n - 1] == '-' || s[n - 1] == '*' || s[n - 1] == '/')
        return false;

    for (int i = 0; i < n; i++)
    {
        char ch = s[i];
        if (ch == ' ' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || (ch >= '0' && ch <= '9'))
        {
           
            if (i == 0 && (ch == '*' || ch == '/'))
                return false;

           
            if (i > 0)
            {
                char prev = s[i - 1];
                if ((prev == '+' || prev == '-' || prev == '*' || prev == '/') &&
                    (ch == '+' || ch == '-' || ch == '*' || ch == '/'))
                {
                    if (prev == ch && (ch == '+' || ch == '-'))
                        return false;
                    if (ch == '*' || ch == '/')
                        return false;
                }
            }
            continue;
        }
        else
            return false; 
    }
    return true;
}

int main()
{
    char s[50];

    printf("Enter the expression : ");

    fgets(s, sizeof(s), stdin);
    printf("\n");

    int size = strlen(s);
    if (size > 0 && s[size - 1] == '\n')
    {
        s[size - 1] = '\0';
    }

    if (!isValid(s))
    {
        printf("Error: Invalid expression.");
        return 0;
    }

    char operators[50];
    int opidx = -1;

    int nums[50];
    int numidx = -1;

    int n = strlen(s);
    int i = 0;

    
    int sign = 1;
    if (s[0] == '-')
    {
        sign = -1;
        i++;
    }
    else if (s[0] == '+')
    {
        i++;
    }

    while (i < n)
    {
        int val = 0;
        bool gotzero = false;
        while (i < n && s[i] >= '0' && s[i] <= '9')
        {
            int digit = s[i] - '0';
            gotzero = true;
            val = val * 10 + digit;
            i++;
        }

        if (val > 0 || gotzero)
        {
            if (numidx == -1 && sign == -1)
            {
                val = -val;
            }
            nums[++numidx] = val;
        }

        if (i < n && (s[i] == '+' || s[i] == '-' || s[i] == '*' || s[i] == '/'))
        {

            if (s[i] == '*' || s[i] == '/')
            {
                while (opidx != -1 && (operators[opidx] == '*' || operators[opidx] == '/'))
                {
                    char op = operators[opidx--];
                    int digit2 = nums[numidx--];
                    int digit1 = nums[numidx--];

                    if (op == '*')
                    {
                        int d = digit1 * digit2;
                        nums[++numidx] = d;
                    }
                    else if (op == '/')
                    {
                        if (digit2 == 0)
                        {
                            printf("Error: Division by zero.");
                            return 0;
                        }
                        int d = digit1 / digit2;
                        nums[++numidx] = d;
                    }
                }
            }

            else if (s[i] == '+' || s[i] == '-')
            {
                while (opidx != -1)
                {
                    char op = operators[opidx--];
                    int digit2 = nums[numidx--];
                    int digit1 = nums[numidx--];

                    if (op == '+')
                    {
                        int d = digit1 + digit2;
                        nums[++numidx] = d;
                    }
                    else if (op == '-')
                    {
                        int d = digit1 - digit2;
                        nums[++numidx] = d;
                    }
                    else if (op == '*')
                    {
                        int d = digit1 * digit2;
                        nums[++numidx] = d;
                    }
                    else if (op == '/')
                    {
                        if (digit2 == 0)
                        {
                            printf("Error: Division by zero.");
                            return 0;
                        }
                        int d = digit1 / digit2;
                        nums[++numidx] = d;
                    }
                }
            }

            operators[++opidx] = s[i];
        }
        i++;
    }

    while (opidx != -1)
    {
        char op = operators[opidx--];

        int digit2 = nums[numidx--];
        int digit1 = nums[numidx--];

        if (op == '+')
        {
            int d = digit1 + digit2;
            nums[++numidx] = d;
        }
        else if (op == '-')
        {
            int d = digit1 - digit2;
            nums[++numidx] = d;
        }
        else if (op == '*')
        {
            int d = digit1 * digit2;
            nums[++numidx] = d;
        }
        else if (op == '/')
        {
            if (digit2 == 0)
            {
                printf("Error: Division by zero.");
                return 0;
            }
            int d = digit1 / digit2;
            nums[++numidx] = d;
        }
    }

    if (numidx < 0)
    {
        printf("Error: Invalid expression.");
        return 0;
    }

    printf("The final answer is : %d", nums[numidx]);
    return 0;
}