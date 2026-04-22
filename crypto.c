#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypto.h"
//so basically xor function is changing the char or number to its ascii value and then changing it to binary and then performing xor function with the key
//to decrypt it we will do xorwith the same key and get the ascii value

static const char b64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static char* base64_encode(unsigned char *data, int len) {
    char *out = malloc(4 * ((len + 2) / 3) + 1);//Base64 converts every 3 bytes to 4 characters and +1 is for \0
    int i, j = 0;

    for (i = 0; i < len; i += 3) {//moving 3 bytes at a time
        int val = (data[i] << 16)//Move bits 16 places left 
                  +((i+1 < len ? data[i+1] : 0) << 8) //? is IF next byte exists use it ELSE use 0
                  +(i+2 < len ? data[i+2] : 0);
        ///////combine 3 bytes into ONE BIG 24-bit number
        // NOW we split the 24 bits into FOUR groups of 6 bits This means: shift right 18 bits keep only last 6 bits
        out[j++] = b64[(val >> 18) & 0x3F];//and 0x3F to Mask to keep only 6 bits.
        out[j++] = b64[(val >> 12) & 0x3F];
        out[j++] = (i+1 < len) ? b64[(val >> 6) & 0x3F] : '=';
        out[j++] = (i+2 < len) ? b64[val & 0x3F] : '=';
        //if data length not multiple of 3 but Base64 needs 4 chars so = fills empty space.
    }

    out[j] = '\0';
    return out;
}
static int b64_index(char c) {
    if ('A' <= c && c <= 'Z') return c - 'A';
    if ('a' <= c && c <= 'z') return c - 'a' + 26;
    if ('0' <= c && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}
static unsigned char* base64_decode(char *data, int *outlen) {
    int len = strlen(data);
    unsigned char *out = malloc(len);
    int i, j = 0;

    for (i = 0; i < len; i += 4) { //again Base64 works 4 chars to 3 bytes
        //NOW again Take 4 Base64 chars convert to numbers combine back into one 24-bit value
        int val = (b64_index(data[i]) << 18) |//Get first 8 bits
                  (b64_index(data[i+1]) << 12) |
                  ((data[i+2] == '=' ? 0 : b64_index(data[i+2])) << 6) |//Means if not padding extract second byte
                  ((data[i+3] == '=' ? 0 : b64_index(data[i+3])));//same here also
        

        out[j++] = (val >> 16) & 0xFF;
        if (data[i+2] != '=') out[j++] = (val >> 8) & 0xFF;
        if (data[i+3] != '=') out[j++] = val & 0xFF;
    }

    *outlen = j;
    return out;
}

char* xorencrypt(char *text, char *key) {
    int len = strlen(text);
    int keylen = strlen(key);//so key will be a string
    //in bitwise operator xor is done with the key but if we do that here then it will be very easy to break that
    unsigned char *temp_raw = malloc(len);//allocate temp buffer for raw data
    
    for(int i = 0; i < len; i++) {
        temp_raw[i] = text[i] ^ key[i % keylen];//like in repeated loops first first char then second and then loop mein
    //thus here to xor we will use each char of the string one by one which makes it difficult to break
    }

    char *base64_result = base64_encode(temp_raw, len);//convert that raw binary to Base64 string
    
    free(temp_raw); // Clean up temp buffer
    return base64_result;
}

char* xordecrypt(char *base64_text, char *key) {
    int decoded_len;
    int keylen = strlen(key);//so key will be a string
    //in bitwise operator xor is done with the key but if we do that here then it will be very easy to break that
    unsigned char *raw_data = base64_decode(base64_text, &decoded_len);// Convert Base64 back to raw binary bytes
    char *decryptedtext = malloc(decoded_len + 1); // Convert Base64 back to raw binary bytes
    
    //XOR the raw bytes to get original characters
    for(int i = 0; i < decoded_len; i++) {
        decryptedtext[i] = raw_data[i] ^ key[i % keylen];
    }
    
    decryptedtext[decoded_len] = '\0'; //cause string should end with \0 char null terminated
    
    //free(raw_data); // Clean up the decoded buffer
    return decryptedtext;
}
/*int main() {
    char text[] = "HELLO";
    char key[] = "KEY";

    // int len = strlen(text);

    // // XOR encrypt
    char *xor_data = xorencrypt(text, key);

    // Base64 encode
    //char *encoded = base64_encode((unsigned char*)xor_data);

    printf("Encrypted (Base64): %s\n",xor_data);

    // Decode
    // int decoded_len;
    // unsigned char *decoded = base64_decode(encoded, &decoded_len);

    // XOR decrypt
    char *decrypted = xordecrypt(xor_data, key);

    printf("Decrypted: %s\n", decrypted);

    return 0;
}*/
//this main is just for testing
