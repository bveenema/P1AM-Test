#include "CLICommand-Monitor.h"
#include <forward_list>
#include <avr/dtostrf.h>

CLICommandMonitor CLICommand_Monitor;

void CLICommandMonitorOnChange(Tag& tag, void* current, const void* previous)
{
    char cString[21];

    // Cast the pointers to their types
    Tag::Type type = tag.GetType();
    switch(type)
    {
        case Tag::Bool:
            strcpy(cString, (*(bool*)current) ? "true" : "false");
            break;
        case Tag::Int:
            snprintf(cString, sizeof(cString), "%li", *(int32_t*)current);
            break;
        case Tag::Unsigned:
            snprintf(cString, sizeof(cString), "%lu", *(uint32_t*)current);
            break;
        case Tag::LongInt:
            snprintf(cString, sizeof(cString), "%lli", *(int64_t*)current);
            break;
        case Tag::LongUnsigned:
            snprintf(cString, sizeof(cString), "%lli", *(uint64_t*)current);
            break;
        case Tag::Float:
            dtostrf(*(float*)current, 1, 6, cString);
            break;
        case Tag::Double:
            dtostrf(*(double*)current, 1, 10, cString);
            break;
        default:
            strcpy(cString, "Unknown Type");
            break;
    }

    char output[255];
    snprintf(output, sizeof(output), "%s = %s %s\n", tag.Name, cString, tag.Unit);
    CLICommand_Monitor.Print(output);
}

void SubcommandHandler_All(char* flag, LinkedList<Tag*>& Tags)
{
    // Confirm Action
    CLICommand_Monitor.Print("Monitor all tags\n");
    
    // Turn on monitoring for all tags
    for(int i=0; i<Tags.size(); i++)
    {
        Tag *tag = Tags.get(i);
        tag->OnChange(CLICommandMonitorOnChange);
    }
}

void SubcommandHandler_None(char* flag, LinkedList<Tag*>& Tags)
{
    // Confirm Action
    CLICommand_Monitor.Print("Monitor no tags\n");
    
    // Turn on monitoring for all tags
    for(int i=0; i<Tags.size(); i++)
    {
        Tag *tag = Tags.get(i);
        tag->RemoveOnChange(CLICommandMonitorOnChange);
    }
}

char* CLICommandMonitor::GetName()
{
    return name;
}

void CLICommandMonitor::HandleCommand(char* flag, LinkedList<Tag*>& Tags)
{
    // Create a list to hold tags that will be updated
    std::forward_list<char*> TagNames;

    struct subcommand
    {
        char* Name;
        char* ShortName;
        void (*handler)(char* flag, LinkedList<Tag*>& Tags);
        
        bool Detected;
    };

    subcommand SubCommands[2] = 
    {
        {"all", "a", SubcommandHandler_All},
        {"none", "n", SubcommandHandler_None},
    };

    // Cycle through the flags
    while (flag !=NULL)
    {
        // If a flag starts with a "-" it is a subcommand
        if(flag[0] == '-')
        {
            // strip out all '-'
            char subcommandName[25];
            if(flag[1] == '-')   strcpy(subcommandName, flag+2);
            else                 strcpy(subcommandName, flag+1);


            // Compare subcommandName to SubCommands
            bool SubCommandFound = false;
            for(int i=0; i<sizeof(SubCommands)/sizeof(SubCommands[0]); i++)
            {
                if( strcmp(subcommandName, SubCommands[i].Name)      == 0  ||
                    strcmp(subcommandName, SubCommands[i].ShortName) == 0    )
                {
                    SubCommands[i].Detected = true;
                    SubCommandFound = true;
                }
            }
            if(!SubCommandFound)
            {
                char buffer[256];
                snprintf(buffer, sizeof(buffer), "Command: \"%s\" not found\n", subcommandName);
                Print(buffer);
            }
        }
        // else it is a tag, add it to the tag list
        else
        {
            TagNames.push_front(flag);
        }

        // Get next flag
        flag = strtok(NULL, " ");
    }

    // Iterate through the TagNames, adding listeners. Remove listeners that don't match
    // loop through the Tags
    for(int i=0; i<Tags.size(); i++)
    {
        Tag *tag = Tags.get(i);

        // for every tag, check if it is in the TagNames
        bool MatchFound = false;
        auto tagName = TagNames.begin();
        while(tagName != TagNames.end() && !MatchFound)
        {
            if(strcmp(*tagName, tag->Name) == 0)
            {
                // Add the listener
                tag->OnChange(CLICommandMonitorOnChange);

                // Remove the TagName from the list
                TagNames.remove(*tagName);

                // Mark as match found
                MatchFound = true;

                // Confirm monitoring
                char buffer[256];
                snprintf(buffer, sizeof(buffer), "Monitoring: %s\n", tag->Name);
                Print(buffer);
            }
            tagName++;
        }
    }

    // Report any tag names that did not match a tag
    if(!TagNames.empty())
    {
        auto tagName = TagNames.begin();
        while(tagName != TagNames.end())
        {
            
            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Tag %s not found\n", *tagName);
            Print(buffer);

            tagName++;
        }
    }

    // Handle any commands
    for(int i=0; i<sizeof(SubCommands)/sizeof(SubCommands[0]); i++)
    {
        if(SubCommands[i].Detected)
            if(SubCommands[i].handler) SubCommands[i].handler(flag, Tags);
    }
}