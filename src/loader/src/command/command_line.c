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
    int i = 0;

    for (i = 0; i < argc; i++) {
        if (sizeof(size_t) == 8) {
            printf(" - %d -> %s (%ld)\n", i, argv[i], strlen(argv[i]));
        } else {
            printf(" - %d -> %s (%d)\n", i, argv[i], strlen(argv[i]));
        }
    }
}

// make unlisted functions accessible inside this file
extern void _ctermInternalCleanupInput(struct cterm_command_line_input input);
extern struct cterm_command_line_input _ctermInternalParseInput(const char *user_input);
extern unsigned int _ctermInternalGetCommandIndex(struct cterm_instance *instance, const char *name);
extern char *_ctermInternalFilterInput(const char *v, char f);

extern bool _ctermInternalHelp(struct cterm_command *command);
extern bool _ctermInternalLicenseCommand(struct cterm_command *command);
extern bool _ctermInternalHelloCommand(struct cterm_command *command);

// inits command line for this instance
// this is a synchronous function! this function would exit only on an according command
void _ctermInitCommandLine(struct cterm_instance *instance, FILE *_input, FILE *output) {
    instance->command_line.input = _input;
    instance->command_line.output = output;

#ifdef __unix__
#ifndef _USE_IO_BUFFERING_
    setbuf(instance->command_line.input, NULL);
    setbuf(instance->command_line.output, NULL);
#endif
#endif
    
    bool should_exit = false;

    // register internal commands
    _ctermRegisterCommand(instance, "help", "Help command", _ctermInternalHelp);
    _ctermRegisterCommand(instance, "exit", "Exit from terminal", _ctermInternalHelp);
    _ctermRegisterCommand(instance, "line", "Internal command line", _ctermInternalHelp);
    _ctermRegisterCommand(instance, "license", "See license", _ctermInternalLicenseCommand);
    _ctermRegisterCommand(instance, "hello", "Print hello world", _ctermInternalHelloCommand);

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

        // filter string fron newlines
        char *filtered_string_buf = _ctermInternalFilterInput(string_buffer, '\n');

        struct cterm_execute_result result = {};

        // try to execute command
        _ctermExecute(instance, filtered_string_buf, &result);

        // remove filtered string buffer
        free(filtered_string_buf);

        command_failed = !result.execute_successful;

        // check if command is exit
        if (result.internal_command == ICommandExit) {
            free(result.executed_command);

            break;
        }
        
        // check if command is line
        else if (result.internal_command == ICommandLine) {
            _ctermInitCommandLine(instance, _input, output);
        }

        // check if command doesnt exist
        if (result.command_not_found) {
            CPRINTF("%s: command not found\n", result.executed_command);
        }

        free(result.executed_command);
    }

    // free string_buffer because we dont need it anymore
    free(string_buffer);
}