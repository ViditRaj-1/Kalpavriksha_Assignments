#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Players_data.h"

typedef struct PlayerNode
{
    int playerID;
    char name[50];
    char teamName[50];
    char role[20];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode *next; // For Linked List requirements
} PlayerNode;

typedef struct Teams
{
    char teamName[50];
    int teamID;
    int totalPlayers;
    double avgBatSR;

    PlayerNode *head; // teams struct will contain ll of players
    PlayerNode *tail;
} Teams;

float findPerformance(PlayerNode *newnode)
{
    if (strcmp(newnode->role, "Batsman") == 0)
    {
        return (newnode->battingAverage * newnode->strikeRate) / 100;
    }
    else if (strcmp(newnode->role, "All-rounder") == 0)
    {
        return ((newnode->battingAverage * newnode->strikeRate) / 100) + (newnode->wickets * 2);
    }
    else
    {
        return (newnode->wickets * 2) + (100 - newnode->economyRate);
    }
}

int teamSearch(Teams **teamsArr, int targetID)
{
    int start = 0;
    int end = teamCount - 1;

    while (start <= end)
    {
        int mid = (start + end) / 2;

        if (teamsArr[mid]->teamID == targetID)
        {

            return mid;
        }

        else if (teamsArr[mid]->teamID < targetID)
        {
            start = mid + 1;
        }
        else if (teamsArr[mid]->teamID > targetID)
        {
            end = mid - 1;
        }
    }

    return -1;
}

void addPlayer(Teams **teamsArr)
{

    printf("Enter Team ID to add player : ");
    int targetID;
    scanf("%d", &targetID);

    int idx = teamSearch(teamsArr, targetID);

    if (idx == -1)
    {
        printf("Team not found !\n");
        return;
    }

    PlayerNode *newnode = (PlayerNode *)malloc(sizeof(PlayerNode));

    newnode->next = NULL;

    strcpy(newnode->teamName, teamsArr[idx]->teamName);

    printf("\nEnter Player Details: \n\n");

    printf("Player ID : ");
    scanf("%d", &newnode->playerID);

    printf("Name : ");
    scanf("%s", newnode->name);

    printf("Role (1-Batsman, 2-Bowler, 3-All-rounder) : ");
    int rolechoice;
    scanf("%d", &rolechoice);

    if (rolechoice == 1)
    {
        strcpy(newnode->role, "Batsman");
    }
    else if (rolechoice == 2)
    {
        strcpy(newnode->role, "Bowler");
    }
    else
    {
        strcpy(newnode->role, "All-rounder");
    }

    printf("Total Runs : ");
    scanf("%d", &newnode->totalRuns);

    printf("Batting Average : ");
    scanf("%f", &newnode->battingAverage);

    printf("Strike Rate : ");
    scanf("%f", &newnode->strikeRate);

    printf("Wickets : ");
    scanf("%d", &newnode->wickets);

    printf("Economy Rate : ");
    scanf("%f", &newnode->economyRate);

    newnode->performanceIndex = findPerformance(newnode);

    teamsArr[idx]->totalPlayers += 1;


    if (teamsArr[idx]->head == NULL)
    {
        teamsArr[idx]->head = newnode;
        teamsArr[idx]->tail = newnode;
    }

    else
    {
        teamsArr[idx]->tail->next = newnode;
        teamsArr[idx]->tail = newnode;
    }

    printf("Player added successfully to Team %s!\n", teamsArr[idx]->teamName);
}

void displayPlayersSpecificTeam(Teams **teamsArr)
{
    printf("Enter Team ID : ");
    int targetID;
    scanf("%d", &targetID);

    int idx = teamSearch(teamsArr, targetID);

    if (idx == -1)
    {
        printf("Team not found !\n");
        return;
    }

    printf("Players of Team : %s\n", teamsArr[idx]->teamName);

    printf("ID  Name     Role   Runs   Avg   SR   Wkts   ER   Perf.Index\n");

    PlayerNode *temp = teamsArr[idx]->head;

    while (temp != NULL)
    {
        printf("%d  ", temp->playerID);
        printf("%s  ", temp->name);
        printf("%s  ", temp->role);
        printf("%d  ", temp->totalRuns);
        printf("%.2f  ", temp->battingAverage);
        printf("%.2f  ", temp->strikeRate);
        printf("%d  ", temp->wickets);
        printf("%.2f  ", temp->economyRate);
        printf("%.2f  ", temp->performanceIndex);

        printf("\n\n");

        temp = temp->next;
    }
    printf("Total Players : %d\n", teamsArr[idx]->totalPlayers);
    printf("Average Batting Strike Rate : %.2f\n", teamsArr[idx]->avgBatSR);
}

void sortByAvgSR(Teams **tempArr)
{

    // by des selection sort
    for (int i = 0; i < teamCount; i++)
    {
        // find maxi SR;
        double maxiSR = tempArr[i]->avgBatSR;
        int maxiIdx = i;

        for (int j = i + 1; j < teamCount; j++)
        {
            if (tempArr[j]->avgBatSR > maxiSR)
            {
                maxiSR = tempArr[j]->avgBatSR;
                maxiIdx = j;
            }
        }
        // swap teams in teamarr with i with maxiIdx
        Teams *temp = tempArr[i];
        tempArr[i] = tempArr[maxiIdx];
        tempArr[maxiIdx] = temp;
    }
}

void displayTeamsbyAvgBatSR(Teams **teamsArr)
{

    // copy teams arr into temp arr
    Teams **tempArr = (Teams **)malloc(teamCount * sizeof(Teams *));

    for (int i = 0; i < teamCount; i++)
    {
        tempArr[i] = teamsArr[i];
    }
    // sort the teamarr by batSr
    sortByAvgSR(tempArr);

    printf("ID   Team Name     Avg Bat SR    TotalPlayers\n");

    for (int i = 0; i < teamCount; i++)
    {
        printf("%d  ", tempArr[i]->teamID);
        printf("%s   ", tempArr[i]->teamName);
        printf("%.2f  ", tempArr[i]->avgBatSR);
        printf("%d   ", tempArr[i]->totalPlayers);

        printf("\n");
    }

    printf("\n");

    
}

void displayTopKByRole(Teams **teamsArr, int idx, int k, char roleName[])
{


    int totalInTeam = teamsArr[idx]->totalPlayers;
    

    PlayerNode **roleArr = (PlayerNode **)malloc(totalInTeam * sizeof(PlayerNode *));
    int roleArrIdx = 0;
    
    PlayerNode *temp = teamsArr[idx]->head;
    while (temp != NULL)
    {
        if (strcmp(temp->role, roleName) == 0)
        {
            roleArr[roleArrIdx++] = temp;
            
        }
        temp = temp->next;
    }

    

    int limit = k;
    

    // Selection sort for only `limit` passes (descending by performanceIndex)
    for (int i = 0; i < limit; i++)
    {
        int maxIdx = i;
        for (int j = i + 1; j < roleArrIdx; j++)
        {
            if (roleArr[j]->performanceIndex > roleArr[maxIdx]->performanceIndex)
            {
                maxIdx = j;
            }
        }
        if (maxIdx != i)
        {
            PlayerNode *temp = roleArr[i];
            roleArr[i] = roleArr[maxIdx];
            roleArr[maxIdx] = temp;
        }
    }

    printf("\nTop %d %s(s) of %s:\n", limit, roleName, teamsArr[idx]->teamName);
    printf("ID     Name       Perf.Index\n");
    for (int i = 0; i < limit; i++)
    {
        printf("%d   ",roleArr[i]->playerID);
        printf("%s   ", roleArr[i]->name);
        printf("%.2f ",roleArr[i]->performanceIndex);

        printf("\n");
    }
    printf("\n");

    
}

void displayKPlayers(Teams**teamsArr){
    printf("Enter Team ID : ");
    int targetID;
    scanf("%d", &targetID);

    int idx = teamSearch(teamsArr, targetID);

    if(idx==-1){
        printf("Team not found !\n");
        return;
    }

    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder) : ");
    int targetRole;
    scanf("%d", &targetRole);

    printf("Enter number of players : ");
    int k;
    scanf("%d", &k);

    if(targetRole==1){
        displayTopKByRole(teamsArr, idx, k, "Batsman");
    }
    else if(targetRole==2){
        displayTopKByRole(teamsArr, idx, k, "Bowlers");
    }
    else {
        displayTopKByRole(teamsArr, idx, k, "All-rounders");
    }   
}

void mergeDescending(PlayerNode **arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    PlayerNode **L = (PlayerNode **)malloc(n1 * sizeof(PlayerNode *));
    PlayerNode **R = (PlayerNode **)malloc(n2 * sizeof(PlayerNode *));
    if (L == NULL || R == NULL)
    {
        free(L);
        free(R);
        return;
    }

    for (int i = 0; i < n1; i++)
    {
        L[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = arr[mid + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = left;

    
    while (i < n1 && j < n2)
    {
        if (L[i]->performanceIndex >= R[j]->performanceIndex)
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}

void mergeSortDescending(PlayerNode **arr, int left, int right)
{
    if (left >= right)
    {
        return;
    }
    int mid = left + (right - left) / 2;
    mergeSortDescending(arr, left, mid);
    mergeSortDescending(arr, mid + 1, right);
    mergeDescending(arr, left, mid, right);
}

void displayAllByRoleAcrossTeams(Teams **teamsArr, char roleName[])
{
   
    int maxPlayers = playerCount + 100;
    PlayerNode **mergedArr = (PlayerNode **)malloc(maxPlayers * sizeof(PlayerNode *));
    if (mergedArr == NULL)
    {
        printf("Memory allocation failed!\n");
        return;
    }

    int mergedCount = 0;

    
    for (int t = 0; t < teamCount; t++)
    {
        int teamTotal = teamsArr[t]->totalPlayers;
        if (teamTotal <= 0)
        {
            continue;
        }

        PlayerNode **teamRoleArr = (PlayerNode **)malloc(teamTotal * sizeof(PlayerNode *));
        if (teamRoleArr == NULL)
        {
            continue;
        }

        int roleCnt = 0;
        PlayerNode *curr = teamsArr[t]->head;
        while (curr != NULL)
        {
            if (strcmp(curr->role, roleName) == 0)
            {
                teamRoleArr[roleCnt] = curr;
                roleCnt++;
            }
            curr = curr->next;
        }

        if (roleCnt > 0)
        {
            mergeSortDescending(teamRoleArr, 0, roleCnt - 1);
            for (int i = 0; i < roleCnt; i++)
            {
                mergedArr[mergedCount] = teamRoleArr[i];
                mergedCount++;
            }
        }

        free(teamRoleArr);
    }

    if (mergedCount == 0)
    {
        printf("No %s found across any team.\n", roleName);
        free(mergedArr);
        return;
    }

    // Merge the sorted team 
    mergeSortDescending(mergedArr, 0, mergedCount - 1);

    printf("\n %s of all teams : \n", roleName);
    printf("ID     Name          Team        Role       Avg    SR    Wkts       ER    Perf.Index\n");
    for (int i = 0; i < mergedCount; i++)
    {

        printf("%d   ",mergedArr[i]->playerID);
        printf("%s   ",mergedArr[i]->name);
        printf("%s   ",mergedArr[i]->teamName);
        printf("%s   ", roleName);
        printf("%d   ", mergedArr[i]->totalRuns);
        printf("%.2f ", mergedArr[i]->battingAverage);
        printf("%.2f ",mergedArr[i]->strikeRate);
        printf("%d   ",mergedArr[i]->wickets);
        printf("%.2f ",mergedArr[i]->economyRate);
        printf("%.2f ", mergedArr[i]->performanceIndex);

        printf("\n");
    }
    printf("\n");

    free(mergedArr);
}

void displayAllRole(Teams **teamsArr)
{
    printf("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder) : ");
    int roleChoice;
    scanf("%d", &roleChoice);

    char roleName[20];
    if (roleChoice == 1)
    {
        strcpy(roleName, "Batsman");
    }
    else if (roleChoice == 2)
    {
        strcpy(roleName, "Bowler");
    }
    else
    {
        strcpy(roleName, "All-rounder");
    }

    displayAllByRoleAcrossTeams(teamsArr, roleName);
}

void freeMemory(Teams **teamsArr)
{
    // free ll nodes from each team and then free Team struct
    for (int i = 0; i < teamCount; i++)
    {

        while (teamsArr[i]->head != NULL)
        {
            PlayerNode *temp = teamsArr[i]->head;
            teamsArr[i]->head = teamsArr[i]->head->next;

            temp->next = NULL;

            free(temp);
        }

        free(teamsArr[i]);
    }
    free(teamsArr);
}

PlayerNode *createPlayerNode(Player newPlayer)
{
    PlayerNode *newnode = (PlayerNode *)malloc(sizeof(PlayerNode));

    newnode->playerID = newPlayer.id;
    strcpy(newnode->name, newPlayer.name);
    strcpy(newnode->teamName, newPlayer.team);
    strcpy(newnode->role, newPlayer.role);
    newnode->totalRuns = newPlayer.totalRuns;

    newnode->battingAverage = newPlayer.battingAverage;
    newnode->strikeRate = newPlayer.strikeRate;
    newnode->wickets = newPlayer.wickets;

    newnode->economyRate = newPlayer.economyRate;

    newnode->performanceIndex = findPerformance(newnode);

    return newnode;
}

int main()
{
    Teams **teamsArr = (Teams **)malloc(teamCount * sizeof(Teams *));

    for (int i = 0; i < teamCount; i++)
    {
        // store each team in teamsArr
        Teams *newTeam = (Teams *)malloc(sizeof(Teams));

        strcpy(newTeam->teamName, teams[i]);
        newTeam->teamID = i + 1;

        newTeam->totalPlayers = 0;
        newTeam->avgBatSR = 0.0;

        newTeam->head = NULL;
        newTeam->tail = NULL;

        teamsArr[i] = newTeam;
    }

    // now inseert players in each team struct
    for (int t = 0; t < teamCount; t++)
    {
        double avgSR;
        double sumSR = 0.0;
        int totalSRPlayers = 0;

        for (int p = 0; p < playerCount; p++)
        {
            if (strcmp(players[p].team, teamsArr[t]->teamName) == 0)
            {
                PlayerNode *newnode = createPlayerNode(players[p]);
                newnode->next = NULL;

                if (teamsArr[t]->head == NULL)
                {
                    teamsArr[t]->head = newnode;
                    teamsArr[t]->tail = newnode;
                }
                else
                {
                    teamsArr[t]->tail->next = newnode;
                    teamsArr[t]->tail = newnode;
                }

                if (strcmp(players[p].role, "All-rounder") == 0 || strcmp(players[p].role, "Batsman") == 0)
                {
                    totalSRPlayers++;
                    sumSR += players[p].strikeRate;
                }
                teamsArr[t]->totalPlayers += 1;
            }
        }

        avgSR = (double)sumSR / totalSRPlayers;

        teamsArr[t]->avgBatSR = avgSR;
    }

    while (true)
    {
        // implment menu
        printf("\nICC ODI Player Performance Analyzer\n");

        printf("1. Add Player to Team\n");
        printf("2. Display Players of a Specific Team\n");
        printf("3. Display Teams by Average Batting Strike Rate\n");
        printf("4. Display Top K Players of a Specific Team by Role\n");
        printf("5. Display all Players of specific role Across All Teams by performance index\n");
        printf("6. Exit\n");

        printf("\nEnter your choice : ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1)
        {
            addPlayer(teamsArr);
        }
        else if (choice == 2)
        {
            displayPlayersSpecificTeam(teamsArr);
        }
        else if (choice == 3)
        {
            displayTeamsbyAvgBatSR(teamsArr);
        }
        else if (choice == 4)
        {
            displayKPlayers(teamsArr);
        }
        else if (choice == 5)
        {
            displayAllRole(teamsArr);
        }
        else if (choice == 6)
        {
            freeMemory(teamsArr);
            return 0;
        }
        else
        {
            printf("Enter a valid choice : \n");
        }
    }
}