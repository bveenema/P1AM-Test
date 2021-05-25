#include <Arduino.h>
#include <Tags.h>
#include <TagCLI.h>
#include <avr/dtostrf.h>

TagInt myTag;
TagFloat floatTag;

CLIProvider TagCLI([](const char *string)
{
    Serial.print(string);
});

void HandleOnChange(Tag& tag, void* current, const void* previous)
{
    // char stringBuffer[255];
    // char currentBuffer[21];
    // char previousBuffer[21];

    // // Cast the pointers to their types
    // Tag::Type type = tag.GetType();
    // switch(type)
    // {
    //     case Tag::Bool:
    //         strcpy(currentBuffer, (*(bool*)current) ? "true" : "false");
    //         strcpy(previousBuffer, (*(bool*)previous) ? "true" : "false");
    //         break;
    //     case Tag::Int:
    //         snprintf(currentBuffer, sizeof(currentBuffer), "%li", *(int32_t*)current);
    //         snprintf(previousBuffer, sizeof(previousBuffer), "%li", *(int32_t*)previous);
    //         break;
    //     case Tag::Unsigned:
    //         snprintf(currentBuffer, sizeof(currentBuffer), "%lu", *(uint32_t*)current);
    //         snprintf(previousBuffer, sizeof(previousBuffer), "%lu", *(uint32_t*)previous);
    //         break;
    //     case Tag::LongInt:
    //         snprintf(currentBuffer, sizeof(currentBuffer), "%lli", *(int64_t*)current);
    //         snprintf(previousBuffer, sizeof(previousBuffer), "%lli", *(int64_t*)previous);
    //         break;
    //     case Tag::LongUnsigned:
    //         snprintf(currentBuffer, sizeof(currentBuffer), "%lli", *(uint64_t*)current);
    //         snprintf(previousBuffer, sizeof(previousBuffer), "%lli", *(uint64_t*)previous);
    //         break;
    //     case Tag::Float:
    //         dtostrf(*(float*)current, 1, 6, currentBuffer);
    //         dtostrf(*(float*)previous,1, 6, previousBuffer);
    //         break;
    //     case Tag::Double:
    //         dtostrf(*(double*)current, 1, 6, currentBuffer);
    //         dtostrf(*(double*)previous,1, 6, previousBuffer);
    //         break;
    //     default:
    //         strcpy(currentBuffer, "Unknown Type");
    //         strcpy(previousBuffer, "Unknown Type");
    //         break;
    // }
    // snprintf(stringBuffer, sizeof(stringBuffer), "Update %s, New Value: %s, Old Value: %s, Type: %d, Index: %lu", tag.Name, currentBuffer, previousBuffer, tag.GetType(), tag.GetIndex());
    
    // Serial.println(stringBuffer);
}

void setup() {
    Serial.begin(115200);
    while(!Serial);

    myTag = 10;
    floatTag = 1;
    strcpy(myTag.Name, "myTag");
    strcpy(floatTag.Name, "FloatTag");
    myTag.OnChange(HandleOnChange);
    floatTag.OnChange(HandleOnChange);

    TagCLI.AddCommand(CLICommand_Monitor);
    TagCLI.AddTag(myTag);
    TagCLI.AddTag(floatTag);
}

void loop() {
    static uint32_t LastPrintTime = 0;
    if(millis() - LastPrintTime > 5000){
        LastPrintTime = millis();
        myTag += 1;
        floatTag -= .02;
        Serial.println("tick");
    }
    while(Serial.available()>0)
    {
        TagCLI.Read(Serial.read());
    }
}