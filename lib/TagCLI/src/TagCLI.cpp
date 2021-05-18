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

        // Determine the command
        char *token = strtok(messageBuffer, " ");

        //  Print out tokens
        // while (token !=NULL)
        // {
        //     Print(strcat(token, "\n"));
        //     token = strtok(NULL, " ");
        // }

        Print(strcat(token, "\n"));
        
		messageIndex = 0; // reset buffer position

		// Interpet the completed command
		MessageHandler();
	}
}