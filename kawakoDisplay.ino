#include <Adafruit_NeoMatrix.h>
#include <LiquidCrystal_I2C.h>

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
}

void setup() {
    Serial.begin(9600);
    Kawako::leds.begin();
}

void loop() {
    static String currentText = "Welcome to Kawako";
    // static String currentText = "Welcome to Kawako and welcome to the festival and try to make the board crash. Or doesn't it matter, that the message is so long? Nice :)";
    static Kawako::Color currentColor(255, 255, 255);

    // Text reception
    // bool textChanged = false;
    bool textChanged = checkNewTextSerial(currentText, currentColor);

    if (textChanged) {
        Serial.print("Received text: ");
        Serial.println(currentText);
    }

    // display text
    displayOnLEDs(currentText, textChanged, currentColor);

    // Control loop length
    // delay(100); // TODO: check the duration of the loop
}

// debug function. BUG: if the main loop goes to fast, it will crop the text
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

