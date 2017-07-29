
void setup() {
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    String text = Serial.readString();
    if (text.length() > 0) {
        Serial.print("Received text: ");
        Serial.println(text);
    }
}
