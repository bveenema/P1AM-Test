#ifndef _TAGS_CLI_PROVIDER_H_
#define _TAGS_CLI_PROVIDER_H_

#include <stdint.h>

class CLIProvider
{
	public:
		CLIProvider(void (*callback)(char *string))
        {
            Print = callback;
        }

		// Add
		// Adds a new Object to the CommandMap. Returns false if new element cannot be added
		// \param [ObjectInterface*] Object - Pointer to the new Object
		// \return [bool] - false if out of bounds, true if added
		// bool Add(ObjectInterface& Object);

		// Read
		// Reads and interprets character stream. Calls Message Handler when completed message is recieved
		// \param [char] c - The latest read character from the Serial Stream
		void Read(char c);

        // Write
        // Takes a callback function to be called when the CLI Provider needs to write to the output

		// DebugPrintf
		// Formats a message to send to the debug window in MicroManager
		// \param[string] format - The string with format specifiers
		// \param[...] args - values to fill in the format specifiers in the string
		void DebugPrintf(const char* format, ...);

		void test();

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
		void (*Print)(char *string);

};

#endif