#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

entry *addtolist(entry **head, char *sitename, char *username, char *password)
{
    entry *curr;
    entry *newnode = (entry *)malloc(sizeof(entry));
    strncpy(newnode->site, sitename, maxsite - 1);
    strncpy(newnode->usern, username, maxusername - 1);
    strncpy(newnode->passw, password, maxpass - 1);
    newnode->site[maxsite - 1] = '\0';
    newnode->usern[maxusername - 1] = '\0';
    newnode->passw[maxpass - 1] = '\0';
    newnode->next = NULL;
    if (*head == NULL){
        *head = newnode;
    }
    else{
        curr = *head;
        while (curr->next != NULL){
            curr = curr->next;
        }
        curr->next = newnode;
    }
    return newnode;
}

int searchlist(const entry *head, const char *tofind){
    int flag = 0;
    const entry *curr = head; // const used cause only traversing and we dont wanna change it's value

    while (curr)
    {
        if (strstr(curr->site,tofind) != NULL){
            printf("%d. %s  %s  %s\n",flag+1,curr->site,curr->usern,curr->passw);
            flag++;
        }
        curr = curr->next;
    }

    if (flag == 0){
        printf("\nNOT FOUND\n");
    }
    return flag;
}

int deletefromlist(entry **head, const char *todelete){
    if (!*head){
        return 0;
    }
    entry *curr = *head;
    entry *prev = NULL;
    while (curr){
        if (strcmp(curr->site, todelete) == 0){
            if (!prev){
                *head = curr->next;
            }
            else{
                prev->next = curr->next;
            }
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0;//when not found
}

void printlist(const entry *head){
    int i=1;
    const entry *curr=head;
     while (curr) {
        printf("  %-4d %-20s %-20s %-20s\n",//these are the spacings change later
               i, curr->site, curr->usern, curr->passw);
        curr = curr->next;
        i++;
    }
    printf("\n");
}

void freelist(entry **head){
    entry *curr=*head;
    while(curr){
        entry *next=curr->next;
        free(curr);
        curr=next;
    }
    *head =NULL;
}

// int main() {
//     return 0;
// }
