#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "admin.h"
#include "crypto.h"
#include "fileio.h"
//admin.dat to store the admin password (XOR-encrypted, fixed-size)
//users.dat  stores up to MAX_USERS records, each:[char username[MAX_USERNAME]] [char password[MAX_PASSWORD]]
//Both fields are XOR-encrypted on disk.

#define defaultadminuser  "admin"
#define defaultadminpass  "admin123password"

static void clearscreen(void) {
#ifdef _WIN32
    system("cls");    //Windows(just for me)
#else
    system("clear");  // Linux 
#endif
}
static void read_line(char *line,int size){
    //basically reads a line and replaces\n to \0
    if(fgets(line,size,stdin)){
        int len=(int)strlen(line);
        if(len>0 && line[len-1]=='\n'){
            line[len-1]='\0';
        }
    }
}
// static void saveadminpass(const char *pass){
//     //takes the password,encrypt it and writes it
//     char enc[maxpass];
//     strncpy(enc,pass,maxpass-1);
//     enc[maxpass-1]='\0';
//     char key[] = "secret";
//     xorencrypt(enc,key);

//     FILE *fptr=fopen(ADMIN_FILE,"wb");
//     if(fptr){
//         fwrite(enc,sizeof(char),maxpass,fptr);
//         fclose(fptr);
//     }
// }
static void saveadminpass(const char *pass) {
    char key[] = "secret";
    
    // CRITICAL: Capture the pointer returned by the function!
    char *encrypted_version = xorencrypt((char*)pass, key);

    // Use "w" for text mode. "wb" is for raw binary.
    FILE *fptr = fopen(ADMIN_FILE, "w"); 
    if (fptr) {
        // fprintf saves ONLY the string, no extra garbage at the end
        fprintf(fptr, "%s", encrypted_version);
        fclose(fptr);
    }

    // Since xorencrypt used malloc, we must free it here
    free(encrypted_version);
}
static int loadadminpass(char *outpass) {
    FILE *fpte = fopen(ADMIN_FILE, "r");
    if (!fpte) return 0;

    char enc[maxpass] = {0};
    if (fscanf(fpte, "%s", enc) != 1) {
        fclose(fpte);
        return 0;
    }
    fclose(fpte);

    char key[] = "secret";
    char *decrypted = xordecrypt(enc, key);
    
    if (decrypted) {
        //copy the actual decrypted string into outpass
        strncpy(outpass, decrypted, maxpass - 1);
        outpass[maxpass - 1] = '\0';
        free(decrypted);
        return 1;
    }

    return 0;
}
void admininit(void){
    //FILE *fptr=fopen(ADMIN_FILE,"rb"); will open in binarty and give some random var text
    FILE *fptr=fopen(ADMIN_FILE,"r");
    if(fptr){
        fclose(fptr);
        return;//already exsits or initialiazed
    }
    //if doesnt exist then creates a defualt admin account;
    saveadminpass(defaultadminpass);
    printf("\nADMIN ACC INITIALISED WITH DEFAULT PASWORD %s\n",defaultadminpass);
}

int adminlogin(void){
    char uname[maxusern],pass[maxpass],stored[maxpass];

    printf("ADMIN USERNAME: ");
    read_line(uname,maxusern);
    //##could have used scanf but it will stop as space and might overflow but  might consider it later

    //check if username matches or not
    if(strcmp(uname,defaultadminuser)!=0){
        printf("\n  INVALID CREDENTILAS   \n");
        return 0;
    }
    //load stored password and compare
    if(!loadadminpass(stored)){//this will decrypt the password and stoe it in stored
        printf("\n ADMIN DATA FILE NOT PRESENT\n");
        return 0;
    }

    printf("ADMIN PASSWORD: ");
    read_line(pass,maxpass);
    //##could have used scanf but it will stop as space and might overflow but  might consider it later

    if(strcmp(pass,stored)==0){
        clearscreen();
        printf("\n  ADMIN LOGIN SUCCESSFUL  \n");
        return 1;
    }
    printf("\n  INVALID CREDENTILAS \n");
    return 0;

}

// basically admin functions defined

typedef struct userrecord{
    char username[maxusern];
    char password[maxpass];
}urecord;

static int loaduser(urecord *outrecords,int maxcount){
    FILE *fptr=fopen(USERS_FILE,"r");
    if(!fptr) return 0;

    int count=0;
    urecord rec;
    while(count<maxcount && fread(&rec,sizeof(urecord),1,fptr)==1){
        //decrypt fields
        char key[] = "secret";//##change it later
        char* dec_user= xordecrypt((char*)rec.username,key);
        char *dec_pass= xordecrypt((char*)rec.password,key);

        strncpy(outrecords[count].username,dec_user,maxusern-1);
        strncpy(outrecords[count].password,dec_pass,maxpass-1);

        outrecords[count].username[maxusern-1] = '\0';
        outrecords[count].password[maxpass-1] = '\0';
        count++;

        free(dec_user);
        free(dec_pass);
    }
    fclose(fptr);
    return count;
}

static void saveusers(const urecord *record,int count){
    //Save all user records in records[] to USERS_FILE (encrypted)
    FILE *fptr=fopen(USERS_FILE,"w");
    if(!fptr){
        fprintf(stderr,"\n[ERROR]\n");
        return;
    
    }
    for(int i=0;i<count;i++){
        urecord enc=record[i];
        char key[] = "secret";//##change it later
        char *enc_user = xorencrypt((char*)record[i].username, key);
        char *enc_pass = xorencrypt((char*)record[i].password, key);

        strncpy(enc.username,enc_user,maxusern);
        strncpy(enc.password,enc_pass,maxpass);

        fwrite(&enc,sizeof(urecord),1,fptr);
        //fprintf(fptr, "%s   %s\n",enc_user,enc_pass);
        free(enc_user);
        free(enc_pass);
    }
    fclose(fptr);
}

int admincreateuser(const char *username,const char *password){
    urecord recs[maxusers];
    int count=loaduser(recs,maxusers);
    
    //check  for duplicate entries
    for(int i=0;i<count;i++){
        if(strcmp(recs[i].username,username)==0){
            printf("USERNAME ALREADY EXISTS\n");
            printf("  USER NOT CREATED  \n");
            return 0;
        }
    }
    if(count>=maxusers){
        printf("\n USER LIMIT REACHED \n");
        return 0;
    }

    strncpy(recs[count].username,username,maxusern-1);
    strncpy(recs[count].password,password,maxpass-1);
    
    recs[count].username[maxusern-1]='\0';
    recs[count].password[maxpass-1]='\0';
    
    count++;

    saveusers(recs,count);
    clearscreen();
    printf("\n   USER CREATED    \n");
    return 1;
}

int adminresetpassword(const char *username,const char *newpassword){
    urecord recs[maxusers];
    int count=loaduser(recs,maxusers);
    for(int i=0;i<count;i++){
        if(strcmp(recs[i].username,username)==0){
            strncpy(recs[i].password,newpassword,maxpass-1);
            recs[i].password[maxpass-1]='\0';
            saveusers(recs,count);
            clearscreen();
            printf("\nPASSWORD RESET FOR %s\n",username);
            return 1;
        }
    }
    printf("\n WARNING USER NOT FOUND \n");
    return 0;
}

void adminlistusers(void){
    urecord recs[maxusers];
    int count=loaduser(recs,maxusers);

    if(count==0){
        printf("\n  NO USERS    \n");
        return;
    }
    printf("\n\n----------------------------------");
    printf("\nNUMBER OF REGISTERED USERS: %d\n",count);
    for(int i=0;i<count;i++){
        //char key[] = "secret";//##change it later
        //char *dec_uname= xordecrypt((char *)recs[i].username,key);
        printf(" %d)    %s\n",i+1,recs[i].username);
        //free(dec_uname);
    }
    printf("----------------------------------\n");
    printf("\n");
}

int adminuserlogin(const char* username,const char*password){
    urecord recs[maxusers];
    int count=loaduser(recs,maxusers);
    for(int i=0;i<count;i++){
        if((strcmp(recs[i].username,username)==0) && (strcmp(recs[i].password,password)==0)){
            return 1;
        }
    }
    return 0;
}
int admindeleteuser(const char *username){
    urecord recs[maxusers];
    int count=loaduser(recs,maxusers);
    if(strlen(username)==0){
        printf("\nUSERNAME CANNOT BE EMPTY\n");
    }
    //finding thse user
    int founduser=-1;
    for(int i=0;i<count;i++){
        if(strcmp(recs[i].username,username)==0){
            founduser=i;
            break;
        }
    }
    if(founduser==-1){
        printf("\n USER DOES NOT EXIST \n");
        return 0;
    }
    //now shifting recs
    for(int i=founduser;i<count-1;i++){
        recs[i]=recs[i+1];
    }
    count=count-1;

    saveusers(recs,count);//saving the users

    char vaultpath[2*maxusern];
    snprintf(vaultpath,sizeof(vaultpath),"%s/%s%s",VAULT_DIR,username,VAULT_EXT);
    if(remove(vaultpath)==0){
        printf("\n  VAULT FILE DELETED  \n");
    }
    else{
        printf("\n  NO VAULT FOUND  \n");
        //return 0;
    }


    printf("USER DELETED SUCCESSFULLY   \n");
    return 1;

}

//admin menu all the options to execute commands
int adminmenu(void){
    int command;
    char uname[maxusern],pass[maxpass];

    do{
        printf("\n====  ADMIN MENU  ====\n");
        printf(" 1. Create New User\n");
        printf(" 2. Reset User Password\n");
        printf(" 3. List all Users\n");
        printf(" 4. Delete a User\n");
        printf(" 0. Logout\n");
        printf("\nCOMMAND TO PERFORM: ");
        // scanf("%d",&command);
        // getchar();//consumes leftover newline /n
        if (scanf("%d",&command) != 1) {
            printf("PLEASE ENTER A NUMBER\n");
            command = -1;  //set to invalid so we loop again
        }
 
        //Flush EVERYTHING left in the input buffer (including '\n')
        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        switch (command)
        {
        case 1:
            printf("NEW USERNAME: ");
            read_line(uname,maxusern);
            printf("NEW PASSWORD: ");
            read_line(pass,maxpass);
            admincreateuser(uname,pass);
            break;
        
        case 2:
            printf("USERNAME: ");
            read_line(uname,maxusern);
            printf("NEW PASSWORD(to reset): ");
            read_line(pass,maxpass);
            adminresetpassword(uname,pass);
            break;
        
        case 3:
            adminlistusers();
            break;
        case 4:
            adminlistusers(); 
            printf("USERNAME: ");
            read_line(uname,maxusern);
            admindeleteuser(uname);
            break;
        case 0:
            clearscreen();
            printf("\nADMIN LOGGED OUT\n");
            break;
        
        default:
            printf("\nINVALID COMMAND\n");
        }
    }
    while(command!=0);
    return 0;
}

// int main() {
//     return 0;
// }