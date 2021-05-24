#ifndef _CLI_PROVIDER_H_
#define _CLI_PROVIDER_H_

#include "CLICommand.h"
#include <Tags.h>
#include <stdint.h>
#include <LinkedList.h>

class CLIProvider
{
	public:
		CLIProvider(void (*callback)(const char *string))
        {
            Print = callback;
        }

		// AddCommand
		// Adds a new CLI Command to the Provider
		// \param [CLICommand&] Object - Reference to the new Object
		void AddCommand(CLICommand& Object)
        {
            Object.AddPrint(Print);
            Commands.add(&Object);
        }

        // AddTag
		// Register a Tag to the Provider
		// \param [Tag&] tag - Pointer to the new Object
		void AddTag(Tag& tag)
        {
            Tags.add(&tag);
        }

		// Read
		// Reads and interprets character stream. Calls Message Handler when completed message is recieved
		// \param [char] c - The latest read character from the Serial Stream
		void Read(char c);

		// DebugPrintf
		// Formats a message to send to the debug window in MicroManager
		// \param[string] format - The string with format specifiers
		// \param[...] args - values to fill in the format specifiers in the string
		void DebugPrintf(const char* format, ...);

	private:
		// Message Variables
		const uint8_t messageBufferSize = 255;
		char messageBuffer[255];
		uint8_t messageIndex = 0;
		bool FLAG_NoRead = false;

		// Message Handler
		// Interprets messages interfaces with MM_Objects and sends responses to the app.
		void MessageHandler();

		// Callback functions
		void (*Print)(const char *string);

        // Command List
        LinkedList<CLICommand*> Commands = LinkedList<CLICommand*>();

        // Tag List
        LinkedList<Tag*> Tags = LinkedList<Tag*>();
};

#endif