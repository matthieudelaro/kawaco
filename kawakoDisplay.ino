#include <Adafruit_NeoMatrix.h>

namespace Kawako {
    const unsigned char ledPinNumber = 3;

    Adafruit_NeoMatrix leds = Adafruit_NeoMatrix(120, 16, ledPinNumber,
        NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
        NEO_MATRIX_ROWS    + NEO_MATRIX_PROGRESSIVE,
        NEO_GRB            + NEO_KHZ800);
}

void setup() {
    Serial.begin(9600);
}

void loop() {
    // Text reception
    static String currentText = "";
    if (checkNewTextSerial(currentText)) {
        Serial.print("Received text: ");
        Serial.println(currentText);
    }

    // Control loop length
    delay(100); // TODO: check the duration of the loop
}

bool checkNewTextSerial(String &text) {
    static String buffer = "";
    buffer += Serial.readString();
    short indexOfNewLine = buffer.indexOf('\n');
    // In case we did not receive the full text yet
    if (indexOfNewLine == -1) {
        Serial.println("No \\n found");
        return false;
    } else {
        short length = buffer.length();
        text = buffer.substring(0, indexOfNewLine);
        buffer.remove(0, length);
        Serial.print("Found a \\n at ");
        Serial.println(length);
        Serial.print("New buffer: ");
        Serial.println(buffer);
        return true;
    }
}

