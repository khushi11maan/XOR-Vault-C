#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "fileio.h"
#include "crypto.h"
#include "user.h"
#include "admin.h"

static void clearscreen(void) {
#ifdef _WIN32
    system("cls");    //Windows(just for me)
#else
    system("clear");  // Linux 
#endif
}

static void input_line(char *prompt,char *line,int size){
    printf("%s",prompt);
    //basically reads a line and replaces\n to \0
    if(fgets(line,size,stdin)){
        int len=(int)strlen(line);
        if(len>0 && line[len-1]=='\n'){
            line[len-1]='\0';
        }
    }
}
void useraddentry(entry **head){
    char site[maxsite],uname[maxusername],pass[maxpass];

    printf("\n --- NEW CREDENTIALS --- \n");
    input_line("Site Name:",site,maxsite);
    input_line("Username:",uname,maxusername);
    input_line("Password:",pass,maxpass);

   if(strlen(site)==0){
        printf("\nSITE NAME CAN'T BE EMPTY\n");
        return;
    }
    if(strlen(uname)==0){
        printf("\nUSERNAME CAN'T BE EMPTY\n");
        return;
    }
    if(strlen(pass)==0){
        printf("\nPASSWORD CAN'T BE EMPTY\n");
        return;
    }
    if(addtolist(head,site,uname,pass)){
        printf("\n    ENTRY ADDED \n");
        return;
    }
    else{
        printf("\n    FAILED TO ADD   \n");
        return;
    }
}
void userviewentries(const entry *head) {
    printf("\n -------------- ALL CREDENTIALS --------------\n");
    printlist(head);
    return;
}
void usersearchentries(const entry *head){
    char keyword[maxsite];
    printf("\n      SEARCH CREDENTIALS   \n");
    input_line("Enter site:",keyword,maxsite);
    printf("\n");

    if(strlen(keyword)==0){
        printf("\n  NO KEYWORD GIVEN    \n");
        return;
    }

    int found=searchlist(head,keyword);
    if(found==0){
        printf("\n  NO ENTRIES MATCHED  \n");
        return;
    }
    else{
        printf("\n  %d MATCH(ES) FOUND  \n",found);
        return;
    }

}
void userresetpassword(const char *username){
    char currentpass[maxpass];
    char newpass[maxpass];
    char confirm[maxpass];
    printf("\n---RESET YOUR PASSWORD---\n");
    input_line("CURRENT PASSWORD: ",currentpass,maxpass);
    
    if(!adminuserlogin(username,currentpass)){
        printf("\n   FAILED  \nCURRENT PASSWORD IS INCORRECT\n");
        return;
    }
    input_line("NEW PASSWORD: ",newpass,maxpass);
    
    if(strlen(newpass)==0){
        printf("\nPASSWORD CANT BE EMPTY\n");
        return;
    }
    input_line("CONFIRM: ",confirm,maxpass);

    if(strcmp(newpass,confirm)!=0){
        printf("\nTHE PASSWORDS DOESNT MATCH\n");
        printf("\n   TRY AGAIN\n");
        return;
    }
    
    
    if (adminresetpassword(username,newpass))
        printf("\nPASSWORD CHANGE SUCCESSFUL\n");
    else
        printf("\nCOULDNT UPDATE\n       TRY AGAIN\n");


}

void userdeleteentry(entry **head){
    char site[maxsite];
    printf("\n      DELETE CREDENTIAL       \n");
    input_line("Enter site name:",site,maxsite);

    if(strlen(site)==0){
        printf("\n      SITE NAME EMPTY  \n");
        return;
    }
    if(deletefromlist(head,site)){
        printf("\n      ENTRY DELETED       \n");
        return;
    }
    else{
        printf("\n  NO ENTRY FOUND FOR %s   ",site);
        return;
    }
}

int usermenu(const char *username,entry **head){
    int command;
    //load existing credetials from vault into memory
    int loaded=fileload(username,head);
    if(loaded>0){
        printf("LOADED %d CREDENTIAL(S) FROM VAULT\n",loaded);
    }
    else{
        printf("\n  NO EXISTING VAULT FOUND\n");
    }

    do{
        printf("\n====  USER MENU %s    ====\n",username);
        printf(" 1. Add Credentials\n");
        printf(" 2. View all Credentials\n");
        printf(" 3. Search by Site\n");
        printf(" 4. Delete Credentials\n");
        printf(" 5. Change Password for the user acc\n");
        printf(" 0. Logout and Save\n");
        printf("\nCOMMAND TO PERFORM: ");
        // scanf("%d",&command);
        // getchar();//consumes leftover newline /n from scanf
        if (scanf("%d",&command) != 1) {
            /* User typed something that is not a number */
            printf("PLEASE ENTER A NUMBER\n");
            command = -1;  /* set to invalid so we loop again */
        }
 
        /* Flush EVERYTHING left in the input buffer (including '\n') */
        int c;
        while ((c = getchar()) != '\n' && c != EOF);


        switch (command)
        {
        case 1:
            useraddentry(head);
            break;
        
        case 2:
            userviewentries(*head);
            break;
        
        case 3:
            usersearchentries(*head);
            break;
        case 4:
            userdeleteentry(head);
            break;
        case 5:
            userresetpassword(username);
            break;
        case 0:
            //save encrypted vault before going
            if(filesave(username,*head)){
                clearscreen();
                printf("\nVALUT SAVED AND LOGGED OUT\n");
            }
            else{
                printf("\nFAILED TO SAVE VAULT\n");
            }
            break;
        
        default:
            printf("\nINVALID COMMAND\n");
        }
    }
    while(command!=0);

    freelist(head);//freeing linked list from memory
    return 0;
}
// int main() {
//     return 0;
// }