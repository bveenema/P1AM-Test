#include "TagCLI.h"
#include <string.h>

void CLIProvider::Read(char c)
{
    /*
    Serial Stream comes in form of:
        {command} {value} {--specifier}
    followed by a newline character.

    ex. monitor myTag --alarms
    */
	if(messageIndex >= messageBufferSize) messageIndex = 0; // prevent overflow

	if(FLAG_NoRead) return; // ignore stream while handling current message (prevents buffer from changing while handling)

    // Store all characters but newline in buffer
	if(c != '\n') 
	{
		messageBuffer[messageIndex++] = c;
	}

    // Process string on newline (\n) character
	else
	{
        // Null terminate string
		messageBuffer[messageIndex] = 0;

        //  Print out tokens
        char *token;
        token = strtok(messageBuffer, " ");
        
        while (token !=NULL)
        {
            Print(token);
            Print("\n");
            token = strtok(NULL, " ");
        }
        
		messageIndex = 0; // reset buffer position

		// Interpet the completed command
		// MessageHandler();
	}
}