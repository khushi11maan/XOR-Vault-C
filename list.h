#ifndef LIST_H // if NOT defined
#define LIST_H // define it

#define maxsite 1000
#define maxusername 100
#define maxpass 100

typedef struct entry{
    char site[maxsite];
    char usern[maxusername];
    char passw[maxpass];
    struct entry *next;
}entry;

entry *addtolist();
int deletefromlist();//returns 1 if value found and deleted
int searchlist();//returns 0 if not found
void editentryinlist();
void printlist();
void freelist();


#endif /* LIST_H */