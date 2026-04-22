//#include "crypto.c"
#ifndef CRYPTO_H // if NOT defined
#define CRYPTO_H // define it


char *xorencrypt(char *text,char *key);
char *xordecrypt(char *text,char *key);

#endif /* CEYPTO_H */