#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

int globalID = 0;

typedef struct meta
{

    int startoffset;
    int sizebytes;
    bool isFree;
    int allocID;

} meta;

void setStrategy(char *strategy)
{
    printf("Select allocation strategy (1. First Fit, 2. Best Fit, 3. Worst Fit): ");
    int strchoice;
    scanf("%d", &strchoice);

    if (strchoice == 1)
    {
        strcpy(strategy, "First Fit");
    }
    else if (strchoice == 2)
    {
        strcpy(strategy, "Best Fit");
    }
    else if (strchoice == 3)
    {
        strcpy(strategy, "Worst Fit");
    }
    else
    {
        printf("Please enter valid choice !\n");
        return;
    }

    printf("Strategy set to %s\n", strategy);
}

void allocFirstFit(meta **arr, int *arridx, int reqsize)
{

    bool found = false;

    for (int i = 0; i < *arridx; i++)
    {

        if (arr[i]->isFree && arr[i]->sizebytes >= reqsize)
        {
            found = true;

            meta *m1 = (meta *)malloc(sizeof(meta));

            m1->startoffset = arr[i]->startoffset;
            m1->sizebytes = reqsize;
            m1->isFree = false;
            m1->allocID = ++globalID;

            arr[i]->startoffset += reqsize;
            arr[i]->sizebytes -= reqsize;

            for (int j = *arridx; j > i; j--)
            {
                arr[j] = arr[j - 1];
            }

            arr[i] = m1;

            (*arridx)++;

            break;
        }
    }
    if (found)
    {
        printf("Allocated Successfully !\n");
    }
    else
    {
        printf("No space available !\n");
    }
}

void allocBestFit(meta **arr, int *arridx, int reqsize)
{
    int bestfitidx = -1;
    int bestsize = INT_MAX;
    for (int i = 0; i < *arridx; i++)
    {
        if (arr[i]->isFree && arr[i]->sizebytes >= reqsize && arr[i]->sizebytes < bestsize)
        {
            bestsize = arr[i]->sizebytes;
            bestfitidx = i;
        }
    }

    if (bestfitidx == -1)
    {
        printf("No free block \n");
        return;
    }

    int i = bestfitidx;

    meta *m1 = (meta *)malloc(sizeof(meta));

    m1->startoffset = arr[i]->startoffset;
    m1->sizebytes = reqsize;
    m1->isFree = false;
    m1->allocID = ++globalID;

    arr[i]->startoffset += reqsize;
    arr[i]->sizebytes -= reqsize;

    for (int j = *arridx; j > i; j--)
    {
        arr[j] = arr[j - 1];
    }

    arr[i] = m1;

    (*arridx)++;

    printf("Allocation successful !\n");
}

void allocWorstFit(meta **arr, int *arridx, int reqsize)
{

    int worstfitidx = -1;
    int worstsize = INT_MIN;
    for (int i = 0; i < *arridx; i++)
    {
        if (arr[i]->isFree && arr[i]->sizebytes >= reqsize && arr[i]->sizebytes > worstsize)
        {
            worstsize = arr[i]->sizebytes;
            worstfitidx = i;
        }
    }

    if (worstfitidx == -1)
    {
        printf("No free block \n");
        return;
    }

    int i = worstfitidx;

    meta *m1 = (meta *)malloc(sizeof(meta));

    m1->startoffset = arr[i]->startoffset;
    m1->sizebytes = reqsize;
    m1->isFree = false;
    m1->allocID = ++globalID;

    arr[i]->startoffset += reqsize;
    arr[i]->sizebytes -= reqsize;

    for (int j = *arridx; j > i; j--)
    {
        arr[j] = arr[j - 1];
    }

    arr[i] = m1;

    (*arridx)++;

    printf("Allocation successful !\n");
}

void allocateMem(char *strategy, int memsize, meta **arr, int *arridx, int *arrsize)
{
    printf("Requested size : ");
    int reqsize;
    scanf("%d", &reqsize);

    if (*arridx >= *arrsize)
    {
        int newsize = *arrsize + *arrsize;
        meta **temp = (meta **)realloc(arr, newsize * sizeof(meta *));

        arr = temp;

        *arrsize = newsize;
    }

    if (strcmp(strategy, "First Fit") == 0)
    {
        allocFirstFit(arr, arridx, reqsize);
    }
    else if (strcmp(strategy, "Best Fit") == 0)
    {
        allocBestFit(arr, arridx, reqsize);
    }
    else if (strcmp(strategy, "Worst Fit") == 0)
    {
        allocWorstFit(arr, arridx, reqsize);
    }
}

void releaseMem(meta **arr, int *arridx)
{
    printf("Enter ID of memory to release : ");
    int targetID;
    scanf("%d", &targetID);

    bool found = false;

    int targetidx = -1;

    for (int i = 0; i < *arridx; i++)
    {
        if (arr[i]->allocID == targetID && arr[i]->isFree == false)
        {
            found = true;

            arr[i]->allocID = -1;
            arr[i]->isFree = true;
            targetidx = i;
            break;
        }
    }

    if (!found)
    {
        printf("Target Memory not found !\n");
        return;
    }

    // check nbrs that if they are also free merge them into single free block and remove the nbrs from arr

    // check left nbr
    if (targetidx - 1 >= 0 && arr[targetidx - 1]->isFree)
    {
        arr[targetidx]->startoffset = arr[targetidx - 1]->startoffset;
        arr[targetidx]->sizebytes += arr[targetidx - 1]->sizebytes;

        // remove left nbr from arr
        for (int j = targetidx + 2; j < *arridx; j++)
        {

            arr[j - 1] = arr[j];
        }
        (*arridx)--;
    }

    // check right nbr
    if (targetidx + 1 < *arridx && arr[targetidx + 1]->isFree)
    {

        arr[targetidx]->sizebytes += arr[targetidx + 1]->sizebytes;

        // remove right nbr from arr
        for (int j = targetidx + 1; j < *arridx - 1; j++)
        {

            arr[j] = arr[j + 1];
        }
        (*arridx)--;
    }

    printf("Memory released successfully !\n");
}

void displayMemMap(meta **arr, int *arridx, int memsize)
{
    printf("Memory Map (%d)bytes \n", memsize);

    for (int i = 0; i < *arridx; i++)
    {
        if (arr[i]->isFree)
        {
            printf("[ Free : %d bytes , Start : %d ]\n", arr[i]->sizebytes, arr[i]->startoffset);
        }
        else
        {
            printf("[ Allocated #%d : %d bytes , Start : %d ]\n", arr[i]->allocID, arr[i]->sizebytes, arr[i]->startoffset);
        }
    }
}

void displayMemStats(meta **arr, int *arridx, int memsize)
{
    printf("Total Memory : %d\n", memsize);

    int allocatedbytes = 0;
    int freebytes = 0;
    int freeblocks = 0;
    int largestfreeblock = 0;

    for (int i = 0; i < *arridx; i++)
    {
        if (arr[i]->isFree)
        {
            freeblocks++;
            freebytes += arr[i]->sizebytes;

            if (arr[i]->sizebytes > largestfreeblock)
            {
                largestfreeblock = arr[i]->sizebytes;
            }
        }
        else
        {
            allocatedbytes += arr[i]->sizebytes;
        }
    }

    printf("Allocated : %d bytes\n", allocatedbytes);
    printf("Free : %d bytes\n", freebytes);
    printf("Free blocks : %d \n", freeblocks);
    printf("Largest free block : %d bytes\n", largestfreeblock);
}

void resizeMem(meta **arr, int *arridx)
{
    printf("Enter Allocation ID to resize: ");
    int targetID;
    scanf("%d", &targetID);

    int targetidx = -1;
    for (int i = 0; i < *arridx; i++)
    {
        if (!arr[i]->isFree && arr[i]->allocID == targetID)
        {
            targetidx = i;
            break;
        }
    }

    if (targetidx == -1)
    {
        printf("Error: Allocation ID not found.\n");
        return;
    }

    printf("Enter new size: ");
    int newsize;
    scanf("%d", &newsize);

    int oldsize = arr[targetidx]->sizebytes;

    // shrinking the size
    if (newsize < oldsize)
    {
        int diff = oldsize - newsize;
        arr[targetidx]->sizebytes = newsize;

        // if right nbr is free to absorb the shrunk space
        if (targetidx + 1 < *arridx && arr[targetidx + 1]->isFree)
        {
            arr[targetidx + 1]->startoffset -= diff;
            arr[targetidx + 1]->sizebytes += diff;
        }
        else
        {
            // if not then create a new free block for the leftover space and right-shift
            meta *freeBlock = (meta *)malloc(sizeof(meta));
            freeBlock->startoffset = arr[targetidx]->startoffset + newsize;
            freeBlock->sizebytes = diff;
            freeBlock->isFree = true;
            freeBlock->allocID = 0;

            // do right shift from end down to targetidx + 1
            for (int j = *arridx; j > targetidx + 1; j--)
            {
                arr[j] = arr[j - 1];
            }
            arr[targetidx + 1] = freeBlock;
            (*arridx)++;
        }
        printf("Memory shrunk successfully!\n");
        return;
    }

    // expand
    int needed = newsize - oldsize;
    if (targetidx + 1 < *arridx && arr[targetidx + 1]->isFree && arr[targetidx + 1]->sizebytes >= needed)
    {
        arr[targetidx]->sizebytes = newsize;
        arr[targetidx + 1]->startoffset += needed;
        arr[targetidx + 1]->sizebytes -= needed;

        // If right neighbor is now size 0, remove it
        if (arr[targetidx + 1]->sizebytes == 0)
        {
            free(arr[targetidx + 1]);
            for (int j = targetidx + 2; j < *arridx; j++)
            {
                arr[j - 1] = arr[j];
            }
            (*arridx)--;
        }
        printf("Memory expanded in-place successfully!\n");
    }
}

int main()
{
    printf("Enter Memory pool size : ");
    int memsize;
    scanf("%d", &memsize);
    int arrsize = 10;
    meta **arr = (meta **)malloc(arrsize * sizeof(meta *));
    int arridx = 0;

    meta *m1 = (meta *)malloc(sizeof(meta));

    m1->startoffset = 0;
    m1->sizebytes = memsize;
    m1->isFree = true;
    m1->allocID = 0;

    arr[arridx++] = m1;

    char strategy[50];

    setStrategy(strategy);

    while (true)
    {
        printf("Menu \n");
        printf("1. Allocate memory\n");
        printf("2. Release memory\n");
        printf("3. Resize allocation\n");
        printf("4. Display memory map\n");
        printf("5. Display memory statistics\n");
        printf("6. Change allocation strategy\n");
        printf("7. Exit\n");

        int choice;
        printf("Enter your choice : ");
        scanf("%d", &choice);

        if (choice == 1)
        {
            allocateMem(strategy, memsize, arr, &arridx, &arrsize);
        }
        else if (choice == 2)
        {
            releaseMem(arr, &arridx);
        }
        else if (choice == 3)
        {
            resizeMem(arr, &arridx);
        }
        else if (choice == 4)
        {
            displayMemMap(arr, &arridx, memsize);
        }
        else if (choice == 5)
        {
            displayMemStats(arr, &arridx, memsize);
        }
        else if (choice == 6)
        {
            setStrategy(strategy);
        }
        else if (choice == 7)
        {

            for (int i = 0; i < arrsize; i++)
            {
                free(arr[i]);
            }
            free(arr);

            return 0;
        }
        else
        {
            printf("Enter valid choice !\n");
        }
    }
}