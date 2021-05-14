#include <Arduino.h>
#include <Tags.h>

Tag::Description<int> TagDescription;

TagInt myTag;
TagBase<float> floatTag;

template<typename T>
void HandleOnChange(TagBase<T> &tag, const T current, const T previous, const Tag::Type type)
{
    char buffer[255];
    if(type == Tag::Type::Int)
        snprintf(buffer, sizeof(buffer), "Update %s, New Value: %li, Old Value: %li, Type: %d", tag.Name, current, previous, type);
    else
        char currentBuffer[16];
        char previousBuffer[16];
        dtostrf(current, 8, 4, currentBuffer);
        dtostrf(previous, 8, 4, previousBuffer);
        snprintf(buffer, sizeof(buffer), "Updated Tag: %s, New Value: %s, Old Value: %s, Type: %d", tag.Name, currentBuffer, previousBuffer, type);
    Serial.println(buffer);
    Serial.println(current);
}

void setup() {
    Serial.begin(115200);

    myTag = 10;
    strcpy(myTag.Name, "myTag");
    strcpy(floatTag.Name, "Float Tag");
    myTag.OnChange(HandleOnChange);
    floatTag.OnChange(HandleOnChange);
}

void loop() {
    myTag = 1 + myTag;
    floatTag = floatTag + 0.5f;

    delay(1000);
}