#ifndef USER_H
#define USER_H
#include "list.h"
//head is the double pointer to the in memory creditilas list
//thingd to do add new entry,view all entries,search by site name,delete and entry,logout
int usermenu(const char *usernmae,entry **head);
void userresetpassword(const char *username);//resets users password
void useraddentry(entry **head);//adds a new entry
void userviewentries(const entry *head);//displays all the entires in the list
void usersearchentries(const entry *head);//give keyword and searches entries by site name
void userdeleteentry(entry **head);//give site name and it will delete

#endif//USER_H