#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int totalBlocks = 1024;
int blockSize = 512;

typedef struct FreeBlockNode
{
    int blockIndex;
    struct FreeBlockNode *next;
    struct FreeBlockNode *prev;
} FreeBlockNode;

FreeBlockNode *blockHead = NULL;
FreeBlockNode *blockTail = NULL;

char storage[1024][512];

typedef struct FileNode
{
    char name[50];
    int isDirectory; // 1 for yes , 0 for no

    struct FileNode *next;
    struct FileNode *parent;

    struct FileNode *childHead;

    struct FileNode *childTail;

    int contentSize;
    int blockPointers[10];
} FileNode;

void initializeBlocks()
{
    // make DLL of 1024 nodes

    for (int i = 0; i < totalBlocks; i++)
    {
        FreeBlockNode *newnode = (FreeBlockNode *)malloc(sizeof(FreeBlockNode));
        newnode->blockIndex = i;
        newnode->prev = NULL;
        newnode->next = NULL;

        if (blockHead == NULL)
        {
            blockHead = newnode;
            blockTail = newnode;
        }
        else
        {
            newnode->prev = blockTail;
            blockTail->next = newnode;
            blockTail = blockTail->next;
        }
    }
}

int allocateFreeBlock()
{
    if (blockHead == NULL)
    {
        return -1; // disk full
    }

    FreeBlockNode *temp = blockHead;
    int index = temp->blockIndex;

    blockHead = blockHead->next;
    if (blockHead != NULL)
    {
        blockHead->prev = NULL;
    }
    else
    {
        blockTail = NULL;
    }

    free(temp);
    return index;
}

void makeFolder(FileNode *cwd, char *foldername)
{
    FileNode *temp = cwd->childHead;

    bool found = false;

    if (temp != NULL)
    {
        do
        {
            if (strcmp(temp->name, foldername) == 0)
            {
                found = true;
                break;
            }
            temp = temp->next;
        } while (temp != cwd->childHead);
    }

    if (found)
    {
        printf("Duplicate Folder already exist! \n");
        return;
    }

    FileNode *newnode = (FileNode *)malloc(sizeof(FileNode));
    strcpy(newnode->name, foldername);

    newnode->isDirectory = 1;

    newnode->next = NULL;
    newnode->parent = cwd;

    newnode->childHead = NULL;
    newnode->childTail = NULL;

    // connect

    if (cwd->childTail == NULL)
    {
        // empty list
        cwd->childTail = newnode;
        cwd->childHead = newnode;

        cwd->childTail->next = cwd->childHead;
    }
    else
    {
        newnode->next = cwd->childHead;
        cwd->childTail->next = newnode;

        cwd->childTail = newnode;
    }

    printf("Directory is created successfully !\n");
}

void makeFile(FileNode *cwd, char *filename)
{
    // check if dupicate file exist in cwd child head

    bool found = false;

    FileNode *temp = cwd->childHead;

    if (temp != NULL)
    {
        do
        {
            if (strcmp(temp->name, filename) == 0)
            {
                found = true;
                break;
            }
            temp = temp->next;
        } while (temp != cwd->childHead);
    }

    if (found)
    {
        printf("Duplicate file or folder name present!\n");
        return;
    }

    FileNode *newnode = (FileNode *)malloc(sizeof(FileNode));

    strcpy(newnode->name, filename);

    newnode->isDirectory = 0;

    newnode->next = NULL;
    newnode->parent = cwd;

    newnode->childHead = NULL;
    newnode->childTail = NULL;

    if (cwd->childHead == NULL)
    {
        cwd->childHead = newnode;
        cwd->childTail = newnode;

        cwd->childTail->next = cwd->childHead;
    }
    else
    {
        newnode->next = cwd->childHead;
        cwd->childTail->next = newnode;
        cwd->childTail = newnode;
    }

    printf("File is been created successfully !\n");
}

void returnBlockToFreeList(int blockIdx)
{
    FreeBlockNode *newFreeNode = (FreeBlockNode *)malloc(sizeof(FreeBlockNode));
    newFreeNode->blockIndex = blockIdx;
    newFreeNode->next = NULL;
    newFreeNode->prev = blockTail;

    if (blockTail == NULL)
    {
        blockHead = newFreeNode;
        blockTail = newFreeNode;
    }
    else
    {
        blockTail->next = newFreeNode;
        blockTail = newFreeNode;
    }
}

void removeFile(FileNode *cwd, char *targetFile)
{
    FileNode *temp = cwd->childHead;
    FileNode *prev = cwd->childTail;

    if (temp == NULL)
    {
        printf("File not found to delete!\n");
        return;
    }

    do
    {
        if (temp->isDirectory == 0 && strcmp(temp->name, targetFile) == 0)
        {

            int blocksToFree = (temp->contentSize + 512 - 1) / 512;
            for (int i = 0; i < blocksToFree; i++)
            {
                returnBlockToFreeList(temp->blockPointers[i]);
            }

            if (cwd->childHead == cwd->childTail)
            {
                cwd->childHead = NULL;
                cwd->childTail = NULL;
            }
            else
            {
                prev->next = temp->next;
                if (temp == cwd->childHead)
                {
                    cwd->childHead = temp->next;
                }
                if (temp == cwd->childTail)
                {
                    cwd->childTail = prev;
                }
                cwd->childTail->next = cwd->childHead;
            }

            free(temp);
            printf("File has been deleted successfully!\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    } while (temp != cwd->childHead);

    printf("File not found to delete!\n");
}

void listAll(FileNode *cwd)
{
    FileNode *temp = cwd->childHead;

    if (temp == NULL)
    {
        printf("Directory is Empty !\n");
        return;
    }

    do
    {
        printf("%s ", temp->name);
        if (temp->isDirectory == 1)
        {
            printf("(Directory)\n");
        }
        else
        {
            printf("(File)\n");
        }
        temp = temp->next;
    } while (temp != cwd->childHead);
}

void switchDirectory(FileNode **cwd, char *targetFolder)
{
    FileNode *temp = (*cwd)->childHead;

    if (temp == NULL)
    {
        printf("Diretory not found to swicth!\n");
        return;
    }

    do
    {
        if (temp->isDirectory == 1 && strcmp(temp->name, targetFolder) == 0)
        {
            *cwd = temp;
            return;
        }
        temp = temp->next;
    } while (temp != (*cwd)->childHead);

    printf("Diretory not found to swicth!\n");
}

void removeFolder(FileNode *cwd, char *targetFolder)
{
    // search for folder present in cwd
    FileNode *temp = cwd->childHead;
    FileNode *prev = cwd->childTail;

    if (temp == NULL)
    {
        printf("Directory not found to be delete\n");
        return;
    }

    do
    {
        if (temp->isDirectory == 1 && strcmp(temp->name, targetFolder) == 0)
        {
            if (temp->childHead != NULL)
            {
                printf("Directory not empty !\n");
                return;
            }

            if (temp == cwd->childHead)
            {
                cwd->childHead = cwd->childHead->next;
            }
            else if (temp == cwd->childTail)
            {
                cwd->childTail = prev;
            }
            prev->next = temp->next;
            temp->next = NULL;

            free(temp);

            printf("Folder has been deleted successfully!\n");
            return;
        }
        prev = temp;
        temp = temp->next;
    } while (temp != cwd->childHead);

    printf("Directory not found to be delete\n");
}

void writeFile(FileNode *cwd, char *targetFile, char *text)
{
    // Search for file
    FileNode *temp = cwd->childHead;
    FileNode *curr = NULL;

    if (temp == NULL)
    {
        printf("File not found !\n");
        return;
    }

    do
    {
        if (temp->isDirectory == 0 && strcmp(temp->name, targetFile) == 0)
        {
            curr = temp;
            break;
        }
        temp = temp->next;
    } while (temp != cwd->childHead);

    if (curr == NULL)
    {
        printf("File not present to write!\n");
        return;
    }

    // text ki length nikalo aur blocks calculate karo
    int textlen = strlen(text);
    int blockNeed = (textlen + 512 - 1) / 512;

    if (blockNeed > 10)
    {
        printf("Error: File size too large (Max 10 blocks allowed).\n");
        return;
    }

    int bytesWritten = 0;

    // blocks allocate karke data likho
    for (int i = 0; i < blockNeed; i++)
    {
        int blockIdx = allocateFreeBlock();

        if (blockIdx == -1)
        {
            printf("Error: Disk is full! Write operation failed.\n");
            return;
        }

        curr->blockPointers[i] = blockIdx;

        int chunkToCopy = 512;
        if (textlen - bytesWritten < 512)
        {
            chunkToCopy = textlen - bytesWritten;
        }

        storage[blockIdx][0] = '\0';

        strncpy(storage[blockIdx], text + bytesWritten, chunkToCopy);

        storage[blockIdx][chunkToCopy] = '\0';

        bytesWritten += chunkToCopy;
    }

    curr->contentSize = textlen;
    printf("Data written successfully (size=%d bytes).\n", textlen);
}

void readFile(FileNode *cwd, char *targetFile)
{
    FileNode *temp = cwd->childHead;

    FileNode *curr = NULL;

    if (temp == NULL)
    {
        printf("File not found !\n");
        return;
    }

    do
    {
        if (temp->isDirectory == 0 && strcmp(temp->name, targetFile) == 0)
        {
            curr = temp;
            break;
        }
        temp = temp->next;
    } while (temp != cwd->childHead);

    if (curr == NULL)
    {
        printf("File not present to write!\n");
        return;
    }

    if (curr->contentSize == 0)
    {
        printf("(File is empty)\n");
        return;
    }

    int blocksToRead = (curr->contentSize + 512 - 1) / 512;

    for (int i = 0; i < blocksToRead; i++)
    {
        int blockIdx = curr->blockPointers[i];

        printf("%s", storage[blockIdx]);
    }
    printf("\n");
}

void displayDiskUsage()
{
    int totalNodes = 0;
    FreeBlockNode *temp = blockHead;
    while (temp != NULL)
    {
        totalNodes++;
        temp = temp->next;
    }

    int usedNodes = totalBlocks - totalNodes;

    printf("Used Blocks  : %d\n", usedNodes);
    printf("Free Blocks  : %d\n", totalNodes);
}

void freeDoublyLL()
{

    FreeBlockNode *curr = blockHead;

    while (curr != NULL)
    {
        FreeBlockNode *temp = curr;
        curr = curr->next;

        free(temp);
    }
}

void freeCircularLL(FileNode *node)
{
    if (node == NULL || node->childHead == NULL)
    {
        return;
    }

    node->childTail->next = NULL;

    FileNode *curr = node->childHead;
    while (curr != NULL)
    {
        FileNode *nextTemp = curr->next;

        if (curr->isDirectory == 1)
        {
            freeCircularLL(curr);
        }

        free(curr);
        curr = nextTemp;
    }

    node->childHead = NULL;
    node->childTail = NULL;
}

int main()
{
    char userinput[300];

    printf("Compact VFS - ready. Type 'exit' to quit.\n");

    FileNode *root = (FileNode *)malloc(sizeof(FileNode));
    strcpy(root->name, "/");
    root->isDirectory = 1;
    root->parent = NULL;
    root->childHead = NULL;
    root->next = NULL;

    FileNode *cwd = root;

    initializeBlocks();

    while (true)
    {
        printf("%s > ", cwd->name);

        fgets(userinput, sizeof(userinput), stdin);

        int n = strlen(userinput);
        userinput[n - 1] = '\0';
        n--;

        char *command = strtok(userinput, " ");
        char *argument = strtok(NULL, " ");

        if (strcmp(command, "mkdir") == 0)
        {
            makeFolder(cwd, argument);
        }
        else if (strcmp(command, "create") == 0)
        {
            makeFile(cwd, argument);
        }
        else if (strcmp(command, "ls") == 0)
        {
            listAll(cwd);
        }
        else if (strcmp(command, "cd") == 0)
        {
            // check if to go prev or next
            if (strcmp(argument, "..") == 0)
            {
                // prev
                // if already on root
                if (cwd == root)
                {
                    printf("Already on root !\n");
                }
                else
                {
                    cwd = cwd->parent;
                }
            }
            else
            {
                // we have to switch directory
                switchDirectory(&cwd, argument);
            }
        }
        else if (strcmp(command, "rmdir") == 0)
        {
            removeFolder(cwd, argument);
        }
        else if (strcmp(command, "pwd") == 0)
        {
            printf("%s > \n", cwd->name);
        }

        else if (strcmp(command, "write") == 0)
        {
            char *text = strtok(NULL, "");

            writeFile(cwd, argument, text);
        }
        else if (strcmp(command, "read") == 0)
        {
            readFile(cwd, argument);
        }
        else if (strcmp(command, "delete") == 0)
        {
            removeFile(cwd, argument);
        }
        else if (strcmp(command, "df") == 0)
        {
            displayDiskUsage();
        }
        else if (strcmp(command, "exit") == 0)
        {
            freeDoublyLL();

            freeCircularLL(root);
            free(root);

            return 0;
        }
    }
}