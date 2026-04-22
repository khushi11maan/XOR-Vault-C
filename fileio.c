#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>//header for windows
#else
#include <unistd.h>//for linux
#include <sys/stat.h>
#endif
//used for file status operations like checking existence and creating directories using functions like stat() and mkdir()
#include "fileio.h"
#include "list.h"
#include "crypto.h"

int fileensurevaultdir(void){
#ifdef _WIN32//maybe remove later as we will run it in linux only
    if(mkdir(VAULT_DIR)!=0){//this is req as in windows mkdir just takes oneinput and give permission acc to its own system
#else
  if(mkdir(VAULT_DIR,0700)!=0){//but in linux mkdir also takes the number and thus give permissions
#endif
        // Ignore error (directory may already exist)
  }
  return 1;//treats as success;fopen will catch actual I/O error
/*JUST FOR REFERNCE OR LEARNING PURPOSE
0 7 0 0 starts with 0 as octal
  ↑ ↑ ↑
  | | ── others (everyone else)
  | ──── group
  ────── owner 
  7 = 4 + 2 + 1
 read + write + execute--->full access
 thus this means that owner has full access but group and others have no access
  */ 
}

void filevaultpath(char *outpath,const char *username){
    snprintf(outpath,maxusername*2,"%s/%s%s",VAULT_DIR,username,VAULT_EXT);
    outpath[maxusername*2]='\0';
}

int filesave(const char *username,const entry *head){
    //all entries in the linked list to the user's vault file,encrypted,1 on success, 0 on file open failure
    char path[2*maxusername];
    filevaultpath(path,username);
    FILE *fptr=fopen(path,"w");
    if(!fptr){
         fprintf(stderr,"\nERROR\n");
         return 0;
        }
        
    const entry *curr=head;
    while(curr){
            /* Copy fields into local buffers so we don't modify in-memory data */
        char site[maxsite];
        char uname[maxusername];
        char pass[maxpass];
        strncpy(site,curr->site,maxsite-1);
        site[maxsite-1]='\0';
        strncpy(uname,curr->usern,maxusername-1);
        uname[maxusername-1]='\0';
        strncpy(pass,curr->passw,maxpass-1);
        pass[maxpass-1] ='\0';

        //encrypt each field independently
        char key[] = "secret";
        char *site_enc=xorencrypt((char *)site,key);
        char *uname_enc=xorencrypt((char *)uname,key);
        char *pass_enc=xorencrypt((char *)pass,key);
 
        //Write fixed-size encrypted fields to file
        fwrite(site_enc,sizeof(char),maxsite,fptr);
        fwrite(uname_enc,sizeof(char),maxusername,fptr);
        fwrite(pass_enc,sizeof(char),maxpass,fptr);
 
        free(site_enc);
        free(uname_enc);
        free(pass_enc);
        curr=curr->next;
    }
 
    fclose(fptr);
    return 1;
}

int fileload(const char *username,entry **head){//reads encrypted entries from the user's vault file, decrypts them
    //appends them to the linked list pointed to by *head returns number of entries loaded, or -1 if file cannot be opened
    char path[2*maxusername];
    filevaultpath(path,username);
    FILE *fptr=fopen(path,"r");//open in read
    if(!fptr){
         // Vault not found = new user with no entries yet (not an error)
         return 0;
        }
        
    int count = 0;
    char site[maxsite];
    char uname[maxusername];
    char pass[maxpass];
     // read one record at a time until EOF 
    while (fread(site,sizeof(char),maxsite,fptr)==(size_t)maxsite  &&
           fread(uname,sizeof(char),maxusername,fptr)==(size_t)maxusername &&
           fread(pass,sizeof(char),maxpass,fptr)==(size_t)maxpass) {
 
        char key[] = "secret";
        char *site_dec=xordecrypt((char *)site,key);
        char *uname_dec=xordecrypt((char *)uname,key);
        char *pass_dec=xordecrypt((char *)pass,key);
        addtolist(head,site_dec,uname_dec,pass_dec);
        count++;
        free(site_dec);
        free(uname_dec);
        free(pass_dec);
    }
 
    fclose(fptr);
    return count;
}

// int main() {
//     return 0;
// }