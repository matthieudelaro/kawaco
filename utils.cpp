#include "utils.h"
#include "kawako.h"
#include <ctype.h>
#include <stdio.h>

// cf https://stackoverflow.com/a/21491633
// char rfc3986[256] = {0};
char html5[256] = {0};

void url_encoder_rfc_tables_init(){
    int i;
    for (i = 0; i < 256; i++){
        // rfc3986[i] = isalnum( i) || i == '~' || i == '-' || i == '.' || i == '_' ? i : 0;
        html5[i] = isalnum( i) || i == '*' || i == '-' || i == '.' || i == '_' ? i : (i == ' ') ? '+' : 0;
    }
}

char *url_encode(unsigned char *s, char *enc){
    for (; *s; s++){
        if (html5[*s]) sprintf( enc, "%c", html5[*s]);
        else sprintf( enc, "%%%02X", *s);
        while (*++enc);
    }
    return( enc);
}

// ---------

// Given hexadecimal character [0-9,a-f], return decimal value (0 if invalid)
unsigned char unhex(char c) {
    return ((c >= '0') && (c <= '9')) ?      c - '0' :
    ((c >= 'a') && (c <= 'f')) ? 10 + c - 'a' :
    ((c >= 'A') && (c <= 'F')) ? 10 + c - 'A' : 0;
}

unsigned char h2int(char c) {
    if (c >= '0' && c <='9'){
        return((unsigned char)c - '0');
    }
    if (c >= 'a' && c <='f'){
        return((unsigned char)c - 'a' + 10);
    }
    if (c >= 'A' && c <='F'){
        return((unsigned char)c - 'A' + 10);
    }
    return(0);
}


