#include <Arduino.h>
#include <Tags.h>
#include <avr/dtostrf.h>

TagInt myTag;
TagFloat floatTag;

template<typename T>
void HandleOnChange(TagBase<T> &tag, const T current, const T previous, const Tag::Type type)
{
    char stringBuffer[255];
    char currentBuffer[21];
    char previousBuffer[21];
    
    dtostrf(current, 1, (type == Tag::Type::Float) ? 6 : 0, currentBuffer);
    dtostrf(previous,1, (type == Tag::Type::Float) ? 6 : 0, previousBuffer);
    
    snprintf(stringBuffer, sizeof(stringBuffer), "Update %s, New Value: %s, Old Value: %s, Type: %d, Index: %d", tag.Name, currentBuffer, previousBuffer, type, tag.GetIndex());
    
    Serial.println(stringBuffer);
}

void setup() {
    Serial.begin(115200);

    myTag = 10;
    floatTag = 0;
    strcpy(myTag.Name, "myTag");
    strcpy(floatTag.Name, "Float Tag");
    myTag.OnChange(HandleOnChange);
    floatTag.OnChange(HandleOnChange);
}

void loop() {
    Serial.println(myTag != 10);
    Serial.println(!floatTag);

    delay(1000);
}