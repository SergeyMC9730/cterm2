/**
	CTerm -- Basic virtual terminal
    Copyright (C) 2024  Sergei Baigerov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

	You can contact Sergei Baigerov in Discord -- @dogotrigger
*/

#include <cterm/cterm.h>
#include <cterm/cterm_command_line.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CPRINTF(format, ...) instance->internal_funcs.cprintf(instance, format, __VA_ARGS__)

void _ctermInternalPrintStringArray(char **argv, int argc) {
    for (int i = 0; i < argc; i++) {
        printf(" - %d -> %s (%ld)\n", i, argv[i], strlen(argv[i]));
    }
}

// make unlisted functions accessible inside this file
extern void _ctermInternalCleanupInput(struct cterm_command_line_input input);
extern struct cterm_command_line_input _ctermInternalParseInput(const char *user_input);
extern unsigned int _ctermInternalGetCommandIndex(struct cterm_instance *instance, const char *name);

extern bool _ctermInternalHelp(struct cterm_command *command);

// inits command line for this instance
// this is a synchronous function! this function would exit only on an according command
void _ctermInitCommandLine(struct cterm_instance *instance, FILE *_input, FILE *output) {
    instance->command_line.input = _input;
    instance->command_line.output = output;
    
    bool should_exit = false;

    // register internal commands
    _ctermRegisterCommand(instance, "help", "Help command", _ctermInternalHelp);
    _ctermRegisterCommand(instance, "exit", "Exit from terminal", _ctermInternalHelp);
    _ctermRegisterCommand(instance, "line", "Internal command line", _ctermInternalHelp);

    CPRINTF("\n--- Welcome to CTerm %s\n\b", _ctermGetVersion());

    // allocate string buffer
    char *string_buffer = (char *)malloc(1024);

    bool command_failed = false;

    char status = ' ';

    while (should_exit == false) {
        // clear string buffer
        memset(string_buffer, 0, 1024);
    
        // check if previous command failed
        if (command_failed) {
            status = '!';  
        } else {
            status = ' ';
        }

        CPRINTF("%c> ", status);

        // get string from user
        fgets(string_buffer, 1024, instance->command_line.input);

        // parse input string
        struct cterm_command_line_input input = _ctermInternalParseInput(string_buffer);

        // check if parsing failed. if failed dont do anything
        if (input.argument_list_size == 0) continue;

        // check if requested command is "exit"
        if (!strcmp(input.argument_list[0], "exit")) {
            // cleanup parsed input
            _ctermInternalCleanupInput(input);
            
            break;
        }

        // check if requested command is "line"
        else if (!strcmp(input.argument_list[0], "line")) {
            _ctermInitCommandLine(instance, _input, output);
        } else {
            // try to find command
            cterm_command_alloc command = _ctermGetCommand(instance, input.argument_list[0]);

            // check if command does not exist
            if (command == NULL) {
                CPRINTF("%s: command not found\n", input.argument_list[0]);
                command_failed = true;
            } else {
                // setup log stuff
                instance->processed_command = instance->commands + _ctermInternalGetCommandIndex(instance, command->name);

                command->linked_instance = instance;

                // execute command if possible
                if (command->execute) {
                    command_failed = !command->execute(command);
                }

                instance->processed_command = NULL;

                free(command);
            }
        }

        // cleanup parsed input
        _ctermInternalCleanupInput(input);
    }

    // free string_buffer because we dont need it anymore
    free(string_buffer);
}