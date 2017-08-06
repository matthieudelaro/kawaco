#ifndef kawako_2096974
#define kawako_2096974

#include "utils.h"

// #define NULL 0
#define REQUESTED_FAVICON 1
#define REQUESTED_SLASH   2
#define REQUESTED_GET     3
#define REQUESTED_ERROR_MISSING_TEXT 4
#define REQUESTED_ERROR_MISSING_COLOR 5
#define REQUESTED_ERROR_MISSING_END_TEXT 6
// return true when the request has completed
// bool parseRequest(const char c, char* &HTTP_req, const int maxlength, char* &text_c_str, CRGB &color, bool &userProvidedData) {
bool parseRequest(const char c, char* &text_c_str, CRGB &color, bool &userProvidedData, unsigned char &requestType);

#endif
