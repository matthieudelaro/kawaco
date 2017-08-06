#include <Adafruit_NeoMatrix.h>
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>
#include "utils.h"
#include "kawako.h"


namespace Kawako {
    const unsigned char ledPinNumber = 3;

    Adafruit_NeoMatrix leds = Adafruit_NeoMatrix(120, 16, ledPinNumber,
        NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
        NEO_MATRIX_ROWS    + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);


    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

    EthernetServer server(80);  // create a server listening on port 80
    LiquidCrystal_I2C lcd(0x3f,16,2);
}


#ifdef TEST_ON_LAPTOP
    #include <iostream>
    using namespace std;
    #define F(arg) arg
#else
    #define NULL 0
    #include <string.h>
    #include <avr/pgmspace.h>
#endif


#include <stdio.h>
void urldecode(char *str) // inplace urldecode from https://gist.github.com/MightyPork/a3d1a2bae8fbbbc79e5e
{
    unsigned int x;

    for (int i = 0; str[i] != 0; i++) {
        char c = str[i];
        if (c == '+') {
            str[i] = ' ';
        } else if (c == '%') {
            // decode the byte
            sscanf(&str[i + 1], "%02x", &x);
            str[i] = x;

            // shift following chars
            for (int a = i + 3, b = i + 1;; a++, b++) {
                str[b] = str[a]; // move
                if (str[a] == 0) break;
            }
        }
    }
}

// Returns true if receivedTheWholeRequest
bool parseRequest(const char c, char* &text_c_str, CRGB &color, bool &userProvidedData, unsigned char &requestType) {
    // static const int maxlength = 2000;
    static const int maxlength = 100;
    static char HTTP_req[maxlength];
    static int currentIndex = 0;
    static bool currentLineIsBlank = true;
    static bool receivedTheWholeRequest = false;
    static bool receivedTheFirstLine = false;
    static bool url_encoder_rfc_tables_initialized = false;

    if (!receivedTheFirstLine && currentIndex < maxlength-1) {
        HTTP_req[currentIndex] = c;
        HTTP_req[currentIndex+1] = '\0';
        currentIndex++;
    }

    if (c == '\n' && HTTP_req[currentIndex-1] == '\r') { // end of the first line // TODO: fix: always false
        receivedTheFirstLine = true;
        DSERIALln("Received the first line:");
        DSERIALln(HTTP_req);
    }

    if (c == '\n' && currentLineIsBlank) {
        receivedTheWholeRequest = true;
        DSERIALln("Received the whole request:");
        DSERIALln(HTTP_req);
    }

    // if the request has been completely received
    // if (c == '\n' && currentLineIsBlank) {
    if (receivedTheWholeRequest) {
        DSERIALln("Received the whole request");
        DSERIALln(strlen(HTTP_req));
        receivedTheWholeRequest = true;
        if (currentIndex < maxlength) {
            HTTP_req[currentIndex] = '\0';
            currentIndex++;
        }
        char *result = NULL;
        if ((result = strstr(HTTP_req, "favicon")) != NULL) {
            requestType = REQUESTED_FAVICON;
        } else if ((result = strstr(HTTP_req, "/?COLR=%23")) != NULL) {
            requestType = REQUESTED_GET;

            size_t colorStartIndex = result - HTTP_req + 10; // 10 characters in /?COLR=%23
            size_t colorEndIndex = 0;
            size_t textStartIndex = 0;
            size_t textEndIndex = 0;
            if ((result = strstr(HTTP_req, "&TEXT=")) != NULL) {
                textStartIndex = result - HTTP_req + 6; // 6 characters in &TEXT=
                colorEndIndex = result - HTTP_req;

                if ((result = strstr(HTTP_req, " HTTP/1.1\r\n")) != NULL) {
                    textEndIndex = result - HTTP_req;

                    // copy text in given buffer
                    // char * strncpy ( char * destination, const char * source, size_t num );
                    const size_t textLength = textEndIndex - textStartIndex;
                    if (text_c_str != NULL) {
                        delete[] text_c_str;
                    }
                    text_c_str = new char[textLength + 1];
                    strncpy(text_c_str, HTTP_req+textStartIndex, textLength);
                    text_c_str[textLength] = '\0';
                    urldecode(text_c_str);

                    // parse color
                    // colorHexadecimal ="#" + (HTTP_req.substring(currentIndex,nextIndex)).substring(3);
                    //             color = Kawako::CRGB(
                    //                 (unhex(colorHexadecimal[1]) << 4) + unhex(colorHexadecimal[2]),
                    //                 (unhex(colorHexadecimal[3]) << 4) + unhex(colorHexadecimal[4]),
                    //                 (unhex(colorHexadecimal[5]) << 4) + unhex(colorHexadecimal[6])
                    //             );
                    if (!url_encoder_rfc_tables_initialized) {
                        url_encoder_rfc_tables_init(); // required to use unhex
                    }
                    color.r = (unhex(HTTP_req[colorStartIndex]) << 4) + unhex(HTTP_req[colorStartIndex+1]);
                    color.g = (unhex(HTTP_req[colorStartIndex+2]) << 4) + unhex(HTTP_req[colorStartIndex+3]);
                    color.b = (unhex(HTTP_req[colorStartIndex+4]) << 4) + unhex(HTTP_req[colorStartIndex+5]);

                    userProvidedData = true;
                } else {
                    requestType = REQUESTED_ERROR_MISSING_END_TEXT;
                }
            } else {
                requestType = REQUESTED_ERROR_MISSING_TEXT;
            }
        } else {
            requestType = REQUESTED_SLASH;
        }
    }

     // every line of text received from the client ends with \r\n
    if (c == '\n') {
        // last character on line of received text
        // starting new line with next character read
        currentLineIsBlank = true;
    } else if (c != '\r') {
        // a text character was received from client
        currentLineIsBlank = false;
    }

    if (receivedTheWholeRequest) {
        receivedTheWholeRequest = false;
        receivedTheFirstLine = false;
        currentIndex = 0;
        currentLineIsBlank = true;
        return true;
    } else {
        return false;
    }
}


void setup();
void loop();
bool checkNewTextHTTP(String &text, CRGB &color);
bool checkNewTextSerial(String &text, CRGB &color);
bool displayOnLEDs(const String &newText, const bool &startAnew, const CRGB &color);

void setup() {
    #ifdef SERIAL_USB
        Serial.begin(9600);
    #endif
    Kawako::leds.begin();

    Ethernet.begin(Kawako::mac);
    Kawako::server.begin(); // start to listen for clients


    Kawako::lcd.init();                      // initialize the lcd
    Kawako::lcd.backlight();
    Kawako::lcd.setCursor(0,0);
    Kawako::lcd.print(F("Change text: web"));
    Kawako::lcd.setCursor(2,1);
    Kawako::lcd.print(Ethernet.localIP());
}

void loop() {
    static String currentText = String(F("WTP 2017"));
    // static String currentText = "Welcome to Kawako and welcome to the festival and try to make the board crash. Or doesn't it matter, that the message is so long? Nice :)";
    static CRGB currentColor(0, 250, 243);
    static bool initialMessage = true;
    // DSERIALln(Ethernet.localIP());

    bool textChanged = false;
    DSERIALln("------------");
    if (!initialMessage) {
        // Text reception:
        // mock reception:
        // textChanged = false;
        // serial reception:
        // with bool textChanged... : works great
        // without bool textChanged... : works great as well
        // textChanged = checkNewTextSerial(currentText, currentColor); DSERIALln("Ended checkNewTextSerial");
        // fake HTTP:
        // String test12345 = "";
        // CRGB trucColor = CRGB(10, 120, 200);
        // bool truc123 = checkNewTextSerial(test12345, trucColor); DSERIALln("Ended checkNewTextSerial");
        // HTTP reception:
        textChanged = checkNewTextHTTP(currentText, currentColor); DSERIALln("Ended checkNewTextHTTP");

        // DSERIAL("textChanged: "); DSERIALln(textChanged? "true" : "false");
        // DSERIAL("currentText "); DSERIALln(currentText);
    }

    DSERIAL("initialMessage: "); DSERIALln(initialMessage ? "true" : "false");
    DSERIAL("textChanged: "); DSERIALln(textChanged ? "true" : "false");
    DSERIAL("currentText: "); DSERIALln(currentText);
    DSERIAL("currentColor: "); DSERIAL(currentColor.r); DSERIAL(" "); DSERIAL(currentColor.g); DSERIAL(" "); DSERIALln(currentColor.b);

    if (textChanged || initialMessage) {
        DSERIAL("Received text: ");
        DSERIALln(currentText);
        // currentText = "trick";

        // String newText = String("new");
        // for (int i = 0; i < currentText.length(); ++i) {
        //     char c = currentText[i];
        //     if (c == '\n' || c == '\r') {
        //         c = ' ';
        //     }
        //     newText += currentText[i];
        // }
        // // newText += '\n'; // TRICK ?
        // newText += '\0'; // TRICK ?
        // currentText = "";
        // currentText = newText;

        DSERIAL("New Received text: ");
        DSERIALln(currentText);
    }
    // else {
    // }

    // display text
    displayOnLEDs(currentText, textChanged || initialMessage, currentColor);

    // Control loop length
    // delay(500); // TODO: check the duration of the loop

    initialMessage = false;
}

bool checkNewTextHTTP(String &text, CRGB &color) {
    // static String HTTP_req = String(""); // stores the HTTP request
    bool userProvidedData = false;

    EthernetClient client = Kawako::server.available(); // try to get a client
    if (client) {
        DSERIALln("Client connected");
        // bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) { // if data from the client is available
                // DSERIALln("data available from client");
                char c = client.read();
                // DSERIAL("+");
                // if (c == '\n') {
                //     DSERIALln("\\n");
                // } else if (c == '\r') {
                //     DSERIALln("\\r");
                // } else if (c == '\0') {
                //     DSERIALln("\\0");
                // } else {
                //     DSERIALln(c);
                // }
                // while (!parseRequest(c, HTTP_req, maxlength, text_c_str, color, userProvidedData)) {}
                char* text_c_str = NULL;
                unsigned char requestType = 0;
                // if (false) {
                if (parseRequest(c, text_c_str, color, userProvidedData, requestType)) {
                    DSERIALln("parsed the whole request");
                    if (requestType == REQUESTED_FAVICON) {
                        DSERIALln("favicon");
                        client.println(F("HTTP/1.0 404 <CRLF>\r\n<CRLF>\r\n\r\n"));
                        // client.println("<CRLF>");
                        delay(10); // give the web browser time to receive the data
                        client.stop();
                        DSERIALln("answered the HTTP request");
                    } else {
                        DSERIALln("not the favicon");
                        String errorMessage = String();
                        if (requestType == REQUESTED_GET && userProvidedData) {
                            DSERIALln("requestType == REQUESTED_GET && userProvidedData");
                            // DSERIAL("text_c_str: ");DSERIALln(text_c_str);
                            // DSERIAL("text_c_str == NULL : ");DSERIALln(text_c_str == NULL ? "true" : "false");
                            // text = String(text_c_str);
                            // DSERIAL("text: ");DSERIALln(text);
                            // DSERIAL("color: ");DSERIAL(color.r); DSERIAL(" ");DSERIAL(color.g); DSERIAL(" ");DSERIAL(color.g); DSERIALln(" ");
                            // text = String(text_c_str);

                            const int newTextLength = strlen(text_c_str);
                            // text = String("aba");
                            text = String("");
                            for (int i = 0; i < newTextLength; ++i) {
                                text += text_c_str[i];
                            }
                        } else if (requestType != REQUESTED_SLASH) {
                            DSERIALln("requestType != REQUESTED_SLASH");
                        }
                        // DSERIAL("userProvidedData: "); DSERIALln(userProvidedData?"true":"false");
                        // DSERIAL("text: "); DSERIALln(text_c_str);
                        // DSERIAL("text: "); DSERIALln(text);

                            DSERIAL("text_c_str == NULL : ");DSERIALln(text_c_str == NULL ? "true" : "false");
                            DSERIAL("text: "); DSERIALln(text_c_str);
                            DSERIAL("text: ");DSERIALln(text);
                            DSERIAL("color: ");DSERIAL(color.r); DSERIAL(" ");DSERIAL(color.g); DSERIAL(" ");DSERIAL(color.g); DSERIALln(" ");
                        switch (requestType) {
                            case REQUESTED_ERROR_MISSING_TEXT:
                                errorMessage = F("REQUESTED_ERROR_MISSING_TEXT");
                                DSERIAL("errorMessage: "); DSERIALln("REQUESTED_ERROR_MISSING_TEXT");
                                break;
                            case REQUESTED_ERROR_MISSING_COLOR:
                                errorMessage = F("REQUESTED_ERROR_MISSING_COLOR");
                                DSERIAL("errorMessage: "); DSERIALln("REQUESTED_ERROR_MISSING_COLOR");
                                break;
                            case REQUESTED_ERROR_MISSING_END_TEXT:
                                errorMessage = F("REQUESTED_ERROR_MISSING_END_TEXT");
                                DSERIAL("errorMessage: "); DSERIALln("REQUESTED_ERROR_MISSING_END_TEXT");
                                break;
                            case REQUESTED_SLASH:
                                errorMessage = F("Please enter a text and a color");
                                DSERIAL("errorMessage: "); DSERIALln("REQUESTED_SLASH");
                                break;
                            case REQUESTED_GET:
                                errorMessage = F("Updated text.");
                                DSERIAL("errorMessage: "); DSERIALln("REQUESTED_GET");
                                break;
                        }
                        DSERIAL("errorMessage: "); DSERIALln(errorMessage);
                        // TODO: handle errors
                        // answer the request
                        // send a standard http response header
                        client.println(F("HTTP/1.1 200 OK"));
                        client.println(F("Content-Type: text/html"));
                        client.println(F("Connection: close"));
                        client.println();
                        // send web page
                        client.println(F("<!DOCTYPE html>"));
                        client.println(F("<html>"));
                        client.println(F("<head>"));
                        client.println(F("<title>Welcome to Kawaco</title>"));
                        client.println(F("</head>"));
                        client.println(F("<body>"));
                        client.println(F("<h1>Controller Interface</h1>"));
                        client.print(F("<h2>")); client.print(errorMessage); client.print(F("</h2>"));
                        client.println(F("<form method=\"get\">"));
                        // client.print  ("<div style='padding:10px'><input type=\"color\" name=\"COLR\" value=");client.print(colorHexadecimal);client.println(">CRGB</div>");
                        client.print  (F("<div style='padding:10px'><input type=\"color\" name=\"COLR\" value=#FF0000>CRGB</div>"));
                        client.println(F("<div style='padding:10px'><input maxlength=\"30\" type=\"textarea\" size=\"32\" name=\"TEXT\" value="">Enter a text (if longer as 10 characters, it will move around).</div>"));
                        client.println(F("<div style='padding:10px'><input type=\"submit\" value=\"Submit\"></div>"));
                        client.println(F("</form>"));

                        client.println(F("</body>"));
                        client.println(F("</html>"));

                        delay(10); // give the web browser time to receive the data
                        client.stop();
                        DSERIALln("answered the HTTP request");
                    }
                } else {
                    // DSERIALln(".");
                }
            }
        }
    }
    DSERIAL("checkNewTextHTTP, userProvidedData: "); DSERIALln(userProvidedData? "true":"false");
    return userProvidedData;
}

// debug function. BUG: if the main loop goes to fast, it will crop the text.
// For example: "je suis la" => Found a \n at 3
bool checkNewTextSerial(String &text, CRGB &color) {
    static String buffer = "";
    Serial.setTimeout(1);
    buffer += Serial.readString(); // TODO: set timeout
    short indexOfNewLine = buffer.indexOf('\n');
    // In case we did not receive the full text yet
    if (indexOfNewLine < 0) {
        // DSERIALln("No \\n found");
        return false;
    } else {
        short length = buffer.length();
        text = buffer.substring(0, indexOfNewLine);
        buffer.remove(0, length);
        DSERIALln("---");
        DSERIAL("Found a \\n at ");
        DSERIALln(length);
        DSERIAL("New buffer: ");
        DSERIALln(buffer);
        color.r = 255;
        color.g = 255;
        color.b = 255;
        return true;
    }
}

bool displayOnLEDs(const String &newText, const bool &startAnew, const CRGB &color) {
    const unsigned short fontSize = 2;
    const unsigned short charWidth = 6;

    static short textWidth = 0;
    static String currentText = "";
    static short startingPositionOfText = Kawako::leds.width();

    // initialize variables to restart animation (with a new text)
    if (startAnew || !currentText.equals(newText)) {
        DSERIAL("DISPLAYING NEW newText: "); DSERIALln(newText);
        currentText = newText;
        DSERIAL("DISPLAYING NEW currentText: "); DSERIALln(currentText);
        textWidth = currentText.length() * fontSize * charWidth;

        startingPositionOfText = Kawako::leds.width();
        Kawako::leds.setTextWrap(false);// will not create a new line, but move the text instead
        Kawako::leds.setBrightness(80);
        Kawako::leds.setTextSize(fontSize);
        Kawako::leds.setTextColor(Kawako::leds.Color(color.r, color.g, color.b)); // TODO: use the color provided
        // Kawako::leds.setTextColor(Kawako::leds.Color(150, 150, 150)); // TODO: use the color provided
    }

    // compute the position of the text
    if (textWidth <= Kawako::leds.width()) {
        startingPositionOfText = Kawako::leds.width() / 2 - textWidth / 2;
    } else {
        --startingPositionOfText;
        if (startingPositionOfText < -textWidth) {
            startingPositionOfText = Kawako::leds.width();
        }
    }

    DSERIAL("Still displaying currentText: "); DSERIAL(currentText); DSERIAL(" at: "); DSERIALln(startingPositionOfText);
    // display the animation
    // Kawako::leds.setTextColor(Kawako::leds.Color(150, 150, 150)); // TODO: use the color provided
    Kawako::leds.setTextColor(Kawako::leds.Color(color.r, color.g, color.b)); // TODO: use the color provided
    Kawako::leds.setTextWrap(false);// will not create a new line, but move the text instead
    Kawako::leds.setBrightness(80);
    Kawako::leds.setTextSize(fontSize);
    Kawako::leds.fillScreen(0);
    Kawako::leds.setCursor(startingPositionOfText, 0);
    Kawako::leds.print(currentText);
    Kawako::leds.show();
}


//############################################################################# Fonctions #################################################################################

