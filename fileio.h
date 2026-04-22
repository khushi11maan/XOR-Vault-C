#ifndef FILE_IO_H   // if NOT defined
#define FILE_IO_H   // define it

#include "list.h"
#define VAULT_DIR "vaults" // Directory where per-user vault files are stored
#define VAULT_EXT ".vault" // Extension for each username to form the vault filename

void filevaultpath(char *outpath,const char *username);
//basically will build the file path for a users vault  where out_path is output buffer (caller-supplied,should be of enough size to prevent overflow) and username is the logged-in user's name

int filesave(const char *username,const entry *head);
//saves the files in the linked list to the users vault file and each field is XOR encrypted before being written returns 1 if successful

int fileload(const char *username, entry **head);
//Loads entries from the user's vault file into the list,data decrypted after reading returns number of entries loaded, or -1 on failure.

int fileensurevaultdir(void);
//Ensures the vaults/ directory exists (creates it if missing),returns 1 on success, 0 on failure
#endif // FILE_IO_H