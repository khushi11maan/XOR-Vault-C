#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crypto.h"
#include "list.h"
#include "fileio.h"
#include "admin.h"
#include "user.h" 
// 1. Admin Login-admin_menu() for user management
//  2. User Login-user_menu() for credential management
//  0. Exit

static void clearscreen(void) {
#ifdef _WIN32
    system("cls");    //Windows(just for me)
#else
    system("clear");  // Linux 
#endif
}

static void read_input(char *prompt,char *line,int size){
    printf("%s",prompt);
    //basically reads a line and replaces\n to \0
    if(fgets(line,size,stdin)){
        int len=(int)strlen(line);
        if(len>0 && line[len-1]=='\n'){
            line[len-1]='\0';
        }
    }
}

static void printbanner(void) {
    printf("\n");
    printf("  ________________________________________\n");
    printf("  #     SECURE PASSWORD MANAGER          #\n");
    printf("  #     XOR-Encrypted Vault System       #\n");
    printf("  #                                      #\n");
    printf("  #                  -BY CS25B039        #\n");
    printf("  #______________________________________#\n");
    printf("\n");
}

static void handleuserlogin(void){//gets credentials  and then goes to usermenu
    char uname[maxusername],pass[maxpass];

    printf("\n----- USER LOGIN -----\n");
    read_input("Username: ",uname,maxusername);
    read_input("Password: ",pass,maxpass);

    if(adminuserlogin(uname,pass)){
        clearscreen();
        printf("\n===   LOGIN SUCCESSFUL    ===\n    WELCOME %s \n",uname);
        entry *vault=NULL; //each user will have their own linkedlist;
        usermenu(uname,&vault);
    }
    else{
        clearscreen();
        printf("\nFAIL INVALID USERNAME OR PASSWORD\n");
    }
}

static void handleadminlogin(void){
    printf("\n----- ADMIN LOGIN -----\n");
    if(adminlogin()){
        adminmenu();
    }
}


int main(void){

    //first run so we have to create everthing admin.txt if missing 
    //and then vaults/directory if not there
    admininit();
    fileensurevaultdir();

    printbanner();
    int command;
    do{
        printf("\n\n=====   MAIN MENU   =====\n");
        printf(" 1.Admin Login\n");
        printf(" 2.User Login\n");
        printf(" 0.EXIT\n");
        printf("\nCOMMAND TO PERFORM: ");
        // scanf("%d",&command);
        // getchar();
        if (scanf("%d",&command) != 1) {
            // User typed something that is not a number
            printf(" WARNING Please enter a number\n");
            command = -1;  // set to invalid so we loop again
        }
 
        //Flush EVERYTHING left in the input buffer (including '\n')
        int c;
        while ((c = getchar()) != '\n' && c != EOF);


        switch(command){
             case 1:
                handleadminlogin();
                break;
            case 2:
                handleuserlogin();
                break;
            case 0:
                printf("\nEXITING SECURE PASSWORD MANAGER\n        BYE!!!      \n\n");
                break;
            default:
                printf("\nINVALID COMMAND\n");
        }
    } while (command!= 0);
 
    return 0;
}