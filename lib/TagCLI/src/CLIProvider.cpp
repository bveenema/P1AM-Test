#include "CLIProvider.h"

#include <string.h>
#include <ctype.h>
#include <stdio.h> // remove if not using sprintf

void CLIProvider::Read(char c)
{
    /*
    Serial Stream comes in form of:
        {command} {tag/value} {--subcommand} {subcommandData} {--subcommand2} {subcommand2Data}
    followed by a newline character.

    ex. monitor myTag --alarms
    */
	if(messageIndex >= messageBufferSize) messageIndex = 0; // prevent overflow

	if(FLAG_NoRead) return; // ignore stream while handling current message (prevents buffer from changing while handling)

    // Store all characters but newline in buffer
	if(c != '\n' && c != '\r') 
	{
		messageBuffer[messageIndex++] = c;
	}

    // Process string on newline (\n) character
	else if(c == '\n')
	{
        // Null terminate string
		messageBuffer[messageIndex] = 0;

        // Break the message into tokens
        char *token;
        token = strtok(messageBuffer, " ");

        // Lowercase the first token (command candidate)
        for(int i = 0; token[i]; i++){
            token[i] = tolower(token[i]);
        }

        // Match the first token (command candidate) to the command list
        bool CommandFound = false;
        for(int i = 0; i<Commands.size(); i++)
        {
            CLICommand *Command = Commands.get(i);

            // If a match is found, set the flag, discard the command token and call the command handler
            if(strcmp(token, Command->GetName()) == 0)
            {
                CommandFound = true;
                token = strtok(NULL, " ");
                Command->HandleCommand(token, Tags);
            }
        }

        if(!CommandFound)
        {
            Print("Command not found\n");
            Print("\n");
            Print(token);
        }

        messageIndex = 0; // reset buffer position
	}
}