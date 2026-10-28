#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void swap(int **mat, int currrow, int currcol, int targetrow, int targetcol, int **ptr)
{

    int temp = *(*(ptr + currrow) + currcol);
    *(*(ptr + currrow) + currcol) = *(*(ptr + targetrow) + targetcol);
    *(*(ptr + targetrow) + targetcol) = temp;
}

void printMat(int **mat, int n, int **ptr)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            printf("%d ", *(*(ptr + i) + j));
        }
        printf("\n");
    }
}

void transpose(int **mat, int n, int **ptr)
{

    int currrow = 0;
    int currcol = 0;
    while (currrow < n && currcol < n)
    {
        int i = currrow; // trvaser rows
        int j = currcol; // traverse cols
        for (int k = currrow; k < n; k++)
        {
            swap(mat, i, k, k, i, ptr);
        }
        currrow++;
        currcol++;
    }
}

void rotate(int **mat, int n, int **ptr)
{
    // reverse every row
    for (int i = 0; i < n; i++)
    {
        int v = 0, k = n - 1;
        while (v < k)
        {
            swap(mat, i, v, i, k, ptr);
            v++, k--;
        }
    }
}

int getSmooth(int **mat, int n, int currrow, int currcol, int **ptr, int *count)
{
    int smooth = 0;

    // currEle
    smooth += *(*(ptr + currrow) + currcol);
    (*count)++;

    // left
    if (currcol >= 1)
    {
        smooth += *(*(ptr + currrow) + currcol - 1);
        (*count)++;
    }

    // right
    if (currcol < n - 1)
    {
        smooth += *(*(ptr + currrow) + currcol + 1);
        (*count)++;
    }

    // top
    if (currrow >= 1)
    {
        smooth += *(*(ptr + currrow - 1) + currcol);
        (*count)++;
    }

    // bottom
    if (currrow < n - 1)
    {
        smooth += *(*(ptr + currrow + 1) + currcol);
        (*count)++;
    }

    // left top
    if (currrow >= 1 && currcol >= 1)
    {
        smooth += *(*(ptr + currrow - 1) + currcol - 1);
        (*count)++;
    }

    // left bottom
    // row inc and col dec
    if (currrow < n - 1 && currcol >= 1)
    {
        smooth += *(*(ptr + currrow + 1) + currcol - 1);
        (*count)++;
    }

    // right top
    // row kam hogi col badhega
    if (currrow >= 1 && currcol < n - 1)
    {
        smooth += *(*(ptr + currrow - 1) + currcol + 1);
        (*count)++;
    }

    // right bottom
    if (currrow < n - 1 && currcol < n - 1)
    {
        smooth += *(*(ptr + currrow + 1) + currcol + 1);
        (*count)++;
    }

    return smooth;
}

void recursiveFun(int **mat, int n, int currrow, int currcol, int **ptr, int i)
{

    // base case
    if (i > n * n)
        return;

    int nextrow = i / n;
    int nextcol = i % n;

    int count = 0;
    int smooth = getSmooth(mat, n, currrow, currcol, ptr, &count);

    recursiveFun(mat, n, nextrow, nextcol, ptr, i + 1);

    *(*(ptr + currrow) + currcol) = smooth / count;
}

void applySmoothing(int **mat, int n, int **ptr)
{

    // travsese recuusively

    int totalEle = n * n;

    recursiveFun(mat, n, 0, 0, ptr, 0);
}

int main()
{

    int n;
    printf("Enter rows and cols size (2-10) : ");
    scanf("%d", &n);

    if (n < 2 || n > 10)
    {
        printf("Enter value in range of (2-10)");
        return 0;
    }

    int **mat = (int **)malloc(n * sizeof(int *));

    // aaccess value
    int **ptr = mat;

    for (int i = 0; i < n; i++)
    {
        mat[i] = (int *)malloc(n * sizeof(int));
    }

    srand(time(NULL));

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            int randomVal = rand() % 256;
            *(*(ptr + i) + j) = randomVal;
        }
    }

    printf("Original \n");
    printMat(mat, n, ptr);

    // transpose array

    transpose(mat, n, ptr);

    printf("After Transpose \n");
    printMat(mat, n, ptr);

    // rotate
    rotate(mat, n, ptr);

    printf("After Rotate \n");
    printMat(mat, n, ptr);

    // SmoothingFilter
    applySmoothing(mat, n, ptr);

    printf("After Smoothing  \n");
    printMat(mat, n, ptr);
}
