#ifndef _CLI_COMMAND_H_
#define _CLI_COMMAND_H_

#include <LinkedList.h>
#include <Tags.h>

class CLICommand
{
public:
    virtual char* GetName();
    virtual void HandleCommand(char* command, LinkedList<Tag*>& Tags);
    void AddPrint(void (*callback)(const char *string))
    {
        Print = callback;
    }
    void (*Print)(const char *string);
};

#endif