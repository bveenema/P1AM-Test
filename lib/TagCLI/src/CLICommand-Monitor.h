#ifndef _CLI_COMMAND_MONITOR_H_
#define _CLI_COMMAND_MONITOR_H_

#include "CLICommand.h"
#include <string.h>

class CLICommandMonitor : public CLICommand
{
public:
    char* GetName();

    void HandleCommand(char* flag, LinkedList<Tag*>& Tags);
private: 
    char name[8] = "monitor";
};

extern CLICommandMonitor CLICommand_Monitor;
#endif