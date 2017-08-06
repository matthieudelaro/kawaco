#ifndef UTILS_1397483
#define UTILS_1397483

// ---------- OPTIONS:
// #define SERIAL_USB 1 // else will be radio
// #define SERIAL_RADIO 1
// #define DEBUG 1 // See DSERIAL and DSERIALln in utils.h
// ---------- END OF OPTIONS


#define Q(x) #x
#define QUOTE(x) Q(x)

#ifdef SERIAL_USB
  #define SERIAL Serial
#else
    // #ifdef SERIAL_RADIO
      #define SERIAL Serial1
    // #endif
#endif
#ifdef DEBUG // define macros to print on SERIAL in debug mode..
    #ifdef TEST_ON_LAPTOP
        #define DSERIAL(arg) cout << arg;
        #define DSERIALln(arg) cout << arg << endl;
    #else
        #define DSERIAL(arg) SERIAL.print(arg)
        #define DSERIALln(arg) SERIAL.println(arg)
    #endif
#else // and to do nothing in release mode
    #define DSERIAL(arg)
    #define DSERIALln(arg)
#endif
#define DSHOW(arg) DSERIAL(F(QUOTE(arg))); DSERIAL(F(" : ")); DSERIALln(arg);


unsigned char unhex(char c);
unsigned char h2int(char c);

void url_encoder_rfc_tables_init();
char *url_encode( char *table, unsigned char *s, char *enc);

struct CRGB {
    unsigned char r, g, b;

    CRGB(unsigned char r, unsigned char g, unsigned char b) {
        this->r = r;
        this->g = g;
        this->b = b;
    }

    CRGB() {
        this->r = 0;
        this->g = 0;
        this->b = 0;
    }
};


// #include <stdio.h>
// void urldecode(char *str)
// {
//     unsigned int x;

//     for (int i = 0; str[i] != 0; i++) {
//         char c = str[i];
//         if (c == '+') {
//             str[i] = ' ';
//         } else if (c == '%') {
//             // decode the byte
//             sscanf(&str[i + 1], "%02x", &x);
//             str[i] = x;

//             // shift following chars
//             for (int a = i + 3, b = i + 1;; a++, b++) {
//                 str[b] = str[a]; // move
//                 if (str[a] == 0) break;
//             }
//         }
//     }
// }

#ifndef TEST_ON_LAPTOP
    // String urldecode(String str) {

    //     String encodedString="";
    //     char c;
    //     char code0;
    //     char code1;
    //     for (int i =0; i < str.length(); i++){
    //         c=str.charAt(i);
    //         if (c == '+'){
    //             encodedString+=' ';
    //         }else if (c == '%') {
    //             i++;
    //             code0=str.charAt(i);
    //             i++;
    //             code1=str.charAt(i);
    //             c = (h2int(code0) << 4) | h2int(code1);
    //             encodedString+=c;
    //         } else{

    //             encodedString+=c;
    //         }

    //         yield();
    //     }

    //     return encodedString;
    // }
#endif
#endif
