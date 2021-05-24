#include "CLICommand-Monitor.h"

CLICommandMonitor CLICommand_Monitor;


void CLICommandMonitorOnChange(Tag& tag, void* current, const void* previous)
{
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "Tag Update: %s\n", tag.Name);
    CLICommand_Monitor.Print(buffer);
}

char* CLICommandMonitor::GetName()
{
    return name;
}

void CLICommandMonitor::HandleCommand(char* flag, LinkedList<Tag*>& Tags)
{
    // Cycle through the flags
    while (flag !=NULL)
    {
        // If a flag starts with a "-" it is a subcommand
        if(flag[0] == '-')
        {
            Print("Special Command\n");
        }
        else
        {
            bool TagFound = false;
            for(int i=0; i<Tags.size(); i++)
            {
                Tag *ThisTag = Tags.get(i);
                if(strcmp(flag, ThisTag->Name) == 0)
                {
                    TagFound = true;
                    char buffer[255];
                    snprintf(buffer, sizeof(buffer), "Tag Found!\n\tUnit: %s\n", ThisTag->Unit);
                    Print(buffer);

                    ThisTag->OnChange(CLICommandMonitorOnChange);
                }
            }
            if(!TagFound)
            {
                char buffer[256];
                snprintf(buffer, sizeof(buffer), "Tag: \"%s\" not found!\n", flag);
                Print(buffer);
            }
        }

        // Get next flag
        flag = strtok(NULL, " ");
    }
}