#include "utils.h"
// #include "kawako.h"
// #ifdef TEST_ON_LAPTOP
//     #include <iostream>
//     using namespace std;
// #else
//     #define NULL 0
//     #include <string.h>
// #endif


// // Returns true if receivedTheWholeRequest
// bool parseRequest(const char c, char* &text_c_str, CRGB &color, bool &userProvidedData, unsigned char &requestType) {
//     static const int maxlength = 1000;
//     static char HTTP_req[maxlength];
//     static int currentLength = 0;
//     static bool currentLineIsBlank = true;
//     static bool receivedTheWholeRequest = false;
//     static bool url_encoder_rfc_tables_initialized = false;

//     if (currentLength < maxlength-1) {
//         HTTP_req[currentLength-1] = c;
//         currentLength++;
//     }

//     // if the request has been completely received
//     if (c == '\n' && currentLineIsBlank) {
//         receivedTheWholeRequest = true;
//         if (currentLength < maxlength-1) {
//             HTTP_req[currentLength-1] = '\0';
//             currentLength++;
//         }
//         char *result = NULL;
//         if ((result = strstr(HTTP_req, "favicon")) != NULL) {
//             requestType = REQUESTED_FAVICON;
//         } else if ((result = strstr(HTTP_req, "/?COLR=%23")) != NULL) {
//             requestType = REQUESTED_GET;

//             size_t colorStartIndex = result - HTTP_req + 10; // 10 characters in /?COLR=%23
//             size_t colorEndIndex = 0;
//             size_t textStartIndex = 0;
//             size_t textEndIndex = 0;
//             if ((result = strstr(HTTP_req, "&TEXT=")) != NULL) {
//                 textStartIndex = result - HTTP_req + 6; // 6 characters in &TEXT=
//                 colorEndIndex = result - HTTP_req;

//                 if ((result = strstr(HTTP_req, " HTTP/1.1\r\n")) != NULL) {
//                     textEndIndex = result - HTTP_req;

//                     // copy text in given buffer
//                     // char * strncpy ( char * destination, const char * source, size_t num );
//                     const size_t textLength = textEndIndex - textStartIndex;
//                     if (text_c_str != NULL) {
//                         delete[] text_c_str;
//                     }
//                     text_c_str = new char[textLength + 1];
//                     strncpy(text_c_str, HTTP_req+textStartIndex, textLength);
//                     text_c_str[textLength] = '\0';

//                     // parse color
//                     // colorHexadecimal ="#" + (HTTP_req.substring(currentIndex,nextIndex)).substring(3);
//                     //             color = Kawako::CRGB(
//                     //                 (unhex(colorHexadecimal[1]) << 4) + unhex(colorHexadecimal[2]),
//                     //                 (unhex(colorHexadecimal[3]) << 4) + unhex(colorHexadecimal[4]),
//                     //                 (unhex(colorHexadecimal[5]) << 4) + unhex(colorHexadecimal[6])
//                     //             );
//                     if (!url_encoder_rfc_tables_initialized) {
//                         url_encoder_rfc_tables_init(); // required to use unhex
//                     }
//                     color.r = (unhex(HTTP_req[colorStartIndex]) << 4) + unhex(HTTP_req[colorStartIndex+1]);
//                     color.g = (unhex(HTTP_req[colorStartIndex+2]) << 4) + unhex(HTTP_req[colorStartIndex+3]);
//                     color.b = (unhex(HTTP_req[colorStartIndex+4]) << 4) + unhex(HTTP_req[colorStartIndex+5]);

//                     userProvidedData = true;
//                 } else {
//                     requestType = REQUESTED_ERROR_MISSING_END_TEXT;
//                 }
//             } else {
//                 requestType = REQUESTED_ERROR_MISSING_TEXT;
//             }
//         } else {
//             requestType = REQUESTED_SLASH;
//         }
//     }

//     // every line of text received from the client ends with \r\n
//     if (c == '\n') {
//         // last character on line of received text
//         // starting new line with next character read
//         currentLineIsBlank = true;
//     } else if (c != '\r') {
//         // a text character was received from client
//         currentLineIsBlank = false;
//     }

//     if (receivedTheWholeRequest) {
//         receivedTheWholeRequest = false;
//         currentLength = 0;
//         currentLineIsBlank = true;
//         return true;
//     } else {
//         return false;
//     }
// }
