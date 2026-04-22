#ifndef ADMIN_H   // if NOT defined
#define ADMIN_H   // define it

#define ADMIN_FILE "admin.txt" //stores admins credentials
#define USERS_FILE "users.txt" //stores list of registered users
#define maxpass 100
#define maxusern 100
#define maxusers 1000

void admininit(void);//creates admin.txt if doesnt exist basically initialises
int adminlogin(void);
int adminmenu(void);
int admincreateuser(const char *username,const char *password);
int adminresetpassword(const char*username,const char *newpassword);
void adminlistusers(void);
int adminuserlogin(const char *username,const char *password);
int admindeleteuser(const char *username);

#endif // ADMIN_H 