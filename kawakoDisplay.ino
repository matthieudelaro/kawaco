#include <Adafruit_NeoMatrix.h>
#include <LiquidCrystal_I2C.h>
#include <Ethernet.h>

namespace Kawako {
    const unsigned char ledPinNumber = 3;

    Adafruit_NeoMatrix leds = Adafruit_NeoMatrix(120, 16, ledPinNumber,
        NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
        NEO_MATRIX_ROWS    + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);


    struct Color {
        unsigned char r, g, b;

        Color(byte r, byte g, byte b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        Color() {
            this->r = 0;
            this->g = 0;
            this->b = 0;
        }
    };

    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

    EthernetServer server(80);  // create a server listening on port 80
}

void setup() {
    Serial.begin(9600);
    Kawako::leds.begin();

    Ethernet.begin(Kawako::mac);
    Kawako::server.begin(); // start to listen for clients
}

void loop() {
    static String currentText = "Welcome to Kawako";
    // static String currentText = "Welcome to Kawako and welcome to the festival and try to make the board crash. Or doesn't it matter, that the message is so long? Nice :)";
    static Kawako::Color currentColor(255, 255, 255);
    // Serial.println(Ethernet.localIP());

    // Text reception:
    // mock reception:
    // bool textChanged = false;
    // serial reception:
    // bool textChanged = checkNewTextSerial(currentText, currentColor);
    // HTTP reception:
    bool textChanged = checkNewTextHTTP(currentText, currentColor);

    if (textChanged) {
        Serial.print("Received text: ");
        Serial.println(currentText);
    }

    // display text
    displayOnLEDs(currentText, textChanged, currentColor);

    // Control loop length
    // delay(100); // TODO: check the duration of the loop
}

bool checkNewTextHTTP(String &text,
                      Kawako::Color &color) {
    static String HTTP_req = ""; // stores the HTTP request
    bool userProvidedData = false;

    EthernetClient client = Kawako::server.available(); // try to get a client
    if (client) {
        Serial.println("Client connected");
        bool currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) { // if data from the client is available
                // Serial.println("data available from client");
                char c = client.read();
                HTTP_req += c;

                // if the request has been completely received
                if (c == '\n' && currentLineIsBlank) {
                    unsigned int currentIndex=0;
                    unsigned int nextIndex=HTTP_req.length();
                    String colorHexadecimal = "#AABBCC";

                    Serial.println("client is done with his request");
                    Serial.println(HTTP_req);

                    // check that the browser is asking for the page, or for favicon.ico
                    // Serial.println("-----------Checking icon:");

                    // Serial.print("indexOf favicon: ");
                    // Serial.println(HTTP_req.indexOf("favicon"));
                    // Serial.print("lastIndexOf of favicon: ");
                    // Serial.println(HTTP_req.lastIndexOf("favicon"));

                    // Serial.print("indexOf /favicon.ico: ");
                    // Serial.println(HTTP_req.indexOf("favicon"));
                    // Serial.print("indexOf /favicon.ico: ");
                    // Serial.println(HTTP_req.indexOf("/favicon.ico"));
                    // Serial.print("lastIndexOf of /favicon.ico: ");

                    // HTTP_req = " coucou he sui la";
                    // Serial.print("Changing HTTP_req to: ");
                    // Serial.println(HTTP_req);

                    // Serial.print("indexOf favicon: ");
                    // Serial.println(HTTP_req.indexOf("favicon"));
                    // Serial.print("lastIndexOf of favicon: ");
                    // Serial.println(HTTP_req.lastIndexOf("favicon"));

                    // Serial.print("indexOf /favicon.ico: ");
                    // Serial.print("indexOf /favicon.ico: ");
                    // Serial.println(HTTP_req.indexOf("/favicon.ico"));
                    // Serial.print("lastIndexOf of /favicon.ico: ");

                    // HTTP_req = "GET /favicon.ico HTTP";
                    // Serial.print("Changing HTTP_req to: ");
                    // Serial.println(HTTP_req);

                    // Serial.print("indexOf favicon: ");
                    // Serial.println(HTTP_req.indexOf("favicon"));
                    // Serial.print("lastIndexOf of favicon: ");
                    // Serial.println(HTTP_req.lastIndexOf("favicon"));

                    // Serial.print("indexOf /favicon.ico: ");
                    // Serial.print("indexOf /favicon.ico: ");
                    // Serial.println(HTTP_req.indexOf("/favicon.ico"));
                    // Serial.print("lastIndexOf of /favicon.ico: ");

                    // HTTP_req = "GET / HTTP";
                    // Serial.print("Changing HTTP_req to: ");
                    // Serial.println(HTTP_req);

                    // Serial.print("indexOf favicon: ");
                    // Serial.println(HTTP_req.indexOf("favicon"));
                    // Serial.print("lastIndexOf of favicon: ");
                    // Serial.println(HTTP_req.lastIndexOf("favicon"));

                    // Serial.print("indexOf /favicon.ico: ");
                    // Serial.print("indexOf /favicon.ico: ");
                    // Serial.println(HTTP_req.indexOf("/favicon.ico"));
                    // Serial.print("lastIndexOf of /favicon.ico: ");

                    // Serial.println(HTTP_req.lastIndexOf("/favicon.ico"));
                    // Serial.println("-----------Done checking icon.");
                    if (HTTP_req.lastIndexOf("favicon") > -1) {
                        Serial.print("client requested favicon. Index is:");
                    // }
                        client.println("HTTP/1.0 404 <CRLF>");
                        client.println("<CRLF>");
                    } else {
                    // if (true) {
                        Serial.println("client requested/answered the form");
                        // read the color, if given
                        if (HTTP_req.indexOf("COLR=") > -1) {
                            Serial.println("client provided COLR");
                            currentIndex = HTTP_req.indexOf("COLR=")+5;

                            // TODO: proof the next 5 lines
                            if(HTTP_req.indexOf("&",currentIndex)>-1) {
                                nextIndex = HTTP_req.indexOf("&",currentIndex);
                            } else {
                                nextIndex = HTTP_req.indexOf("HTTP",currentIndex);
                            }

                            userProvidedData = true;
                            colorHexadecimal ="#" + (HTTP_req.substring(currentIndex,nextIndex)).substring(3);
                            color = Kawako::Color(
                                (unhex(colorHexadecimal[1]) << 4) + unhex(colorHexadecimal[2]),
                                (unhex(colorHexadecimal[3]) << 4) + unhex(colorHexadecimal[4]),
                                (unhex(colorHexadecimal[5]) << 4) + unhex(colorHexadecimal[6])
                            );
                        }

                        // read the text
                        if (HTTP_req.indexOf("TEXT=")>-1) {
                            Serial.println("client provided TEXT");
                            currentIndex = HTTP_req.indexOf("TEXT=")+5;
                            nextIndex = HTTP_req.indexOf("HTTP",currentIndex)-1;
                            text = HTTP_req.substring(currentIndex,nextIndex);
                            text = urldecode(text);
                            userProvidedData = true;
                            // TODO:control characters
                        }

                        // answer the request
                        // send a standard http response header
                        client.println("HTTP/1.1 200 OK");
                        client.println("Content-Type: text/html");
                        client.println("Connection: close");
                        client.println();
                        // send web page
                        client.println("<!DOCTYPE html>");
                        client.println("<html>");
                        client.println("<head>");
                        client.println("<title>Welcome to Kawako</title>");
                        client.println("</head>");
                        client.println("<body>");
                        client.println("<h1>Controller Interface</h1>");

                        client.println("<form method=\"get\">");
                        client.print  ("<div style='padding:10px'><input type=\"color\" name=\"COLR\" value=");client.print(colorHexadecimal);client.println(">Color</div>");
                        client.println("<div style='padding:10px'><input maxlength=\"30\" type=\"textarea\" size=\"32\" name=\"TEXT\" value="">Entrer un texte a afficher, si il fait plus de 10 cracteres il sera defilant.</div>");
                        client.println("<div style='padding:10px'><input type=\"submit\" value=\"Submit\"></div>");
                        client.println("</form>");

                        // client.println("<form method=\"get\">");
                        // client.println("<div style='padding:10px'><input type=\"submit\" value=\"Clear\"></div>");
                        // client.println("<div style='padding:10px'><input type=\"hidden\" name=\"COLR\" value=");client.println(colorHexadecimal);client.println("></div>");
                        // client.println("<div style='padding:10px'><input maxlength=\"20\" type=\"hidden\" name=\"TEXT\" value=""></div>");
                        // client.println("</form>");

                        client.println("</body>");
                        client.println("</html>");
                    }

                    HTTP_req = "";
                    delay(10); // give the web browser time to receive the data
                    client.stop();
                    Serial.println("answered the request");
                    Serial.println("-------------------");
                    break; // TODO: replace this
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
            }
        }
    }

    return userProvidedData;
}

// debug function. BUG: if the main loop goes to fast, it will crop the text.
// For example: "je suis la" => Found a \n at 3
bool checkNewTextSerial(String &text,
                        Kawako::Color &color) {
    static String buffer = "";
    Serial.setTimeout(1);
    buffer += Serial.readString(); // TODO: set timeout
    short indexOfNewLine = buffer.indexOf('\n');
    // In case we did not receive the full text yet
    if (indexOfNewLine < 0) {
        // Serial.println("No \\n found");
        return false;
    } else {
        short length = buffer.length();
        text = buffer.substring(0, indexOfNewLine);
        buffer.remove(0, length);
        Serial.println("---");
        Serial.print("Found a \\n at ");
        Serial.println(length);
        Serial.print("New buffer: ");
        Serial.println(buffer);
        color.r = 255;
        color.g = 255;
        color.b = 255;
        return true;
    }
}

bool displayOnLEDs(const String &newText, const bool &startAnew,
                   const Kawako::Color &color) {
    const unsigned fontSize = 2;
    const unsigned charWidth = 6;
    static short textWidth = 0;
    static String currentText = "";
    static short startingPositionOfText = Kawako::leds.width();

    // initialize variables to restart animation (with a new text)
    if (startAnew || !currentText.equals(newText)) {
        currentText = newText;
        textWidth = currentText.length() * fontSize * charWidth;

        startingPositionOfText = Kawako::leds.width();
        Kawako::leds.setTextWrap(false);// will not create a new line, but move the text instead
        Kawako::leds.setBrightness(80);
        Kawako::leds.setTextSize(fontSize);
        Kawako::leds.setTextColor(Kawako::leds.Color(color.r, color.g, color.b));
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

    // display the animation
    Kawako::leds.fillScreen(0);
    Kawako::leds.setCursor(startingPositionOfText, 0);
    Kawako::leds.print(currentText);
    Kawako::leds.show();
}


//############################################################################# Fonctions #################################################################################

// Given hexadecimal character [0-9,a-f], return decimal value (0 if invalid)
uint8_t unhex(char c) {
    return ((c >= '0') && (c <= '9')) ?      c - '0' :
    ((c >= 'a') && (c <= 'f')) ? 10 + c - 'a' :
    ((c >= 'A') && (c <= 'F')) ? 10 + c - 'A' : 0;
}



String urldecode(String str)
{

    String encodedString="";
    char c;
    char code0;
    char code1;
    for (int i =0; i < str.length(); i++){
        c=str.charAt(i);
        if (c == '+'){
            encodedString+=' ';
        }else if (c == '%') {
            i++;
            code0=str.charAt(i);
            i++;
            code1=str.charAt(i);
            c = (h2int(code0) << 4) | h2int(code1);
            encodedString+=c;
        } else{

            encodedString+=c;
        }

        yield();
    }

    return encodedString;
}


unsigned char h2int(char c)
{
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

