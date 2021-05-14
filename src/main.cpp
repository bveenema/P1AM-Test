#include <Arduino.h>
#include <Tags.h>

TagInt Tag;

void setup() {
    Serial.begin(115200);
}

void loop() {
    Tag.Update();
    Tag::TagType type = Tag.GetType();
    delay(1000);
}