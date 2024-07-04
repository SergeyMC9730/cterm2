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
#include <cterm/cterm_general_tools.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// make unlisted functions accessible inside this file
extern void _ctermInternalCleanupInput(struct cterm_command_line_input input);
extern struct cterm_command_line_input _ctermInternalParseInput(const char *user_input);
extern unsigned int _ctermInternalGetCommandIndex(struct cterm_instance *instance, const char *name);

void _ctermExecute(struct cterm_instance *instance, const char *input, struct cterm_execute_result *result) {
    // parse input string
    struct cterm_command_line_input cmd = _ctermInternalParseInput(input);

    // check if parsing failed. if failed dont do anything
    if (cmd.argument_list_size == 0) {
        result->empty = true;
        result->execute_successful = true;
        result->internal_command = ICommandNotACommand;
        result->command_not_found = false;
        result->executed_command = NULL;

        return;
    }

    result->executed_command = _ctermCopyString(cmd.argument_list[0]);

    // check if requested command is "exit"
    if (!strcmp(cmd.argument_list[0], "exit")) {
        // cleanup parsed input
        _ctermInternalCleanupInput(cmd);

        result->internal_command = ICommandExit;
        result->execute_successful = true;
        result->empty = false;
            
        return;
    }

    // check if requested command is "line"
    else if (!strcmp(cmd.argument_list[0], "line")) {
        // cleanup parsed input
        _ctermInternalCleanupInput(cmd);

        result->internal_command = ICommandLine;
        result->execute_successful = true;
        result->command_not_found = false;
        result->empty = false;

        return;
    }

    // try to find command
    cterm_command_alloc command = _ctermGetCommand(instance, cmd.argument_list[0]);

    // check if command does not exist
    if (command == NULL) {
        result->command_not_found = true;
        result->execute_successful = false;
        result->internal_command = ICommandNotACommand;
        result->empty = false;
    } else {
        result->command_not_found = false;
        result->internal_command = ICommandNotACommand;
        result->empty = false;

        // setup log stuff
        instance->processed_command = instance->commands + _ctermInternalGetCommandIndex(instance, command->name);

        command->linked_instance = instance;
        command->argc = cmd.argument_list_size;
        command->argv = cmd.argument_list;

        // execute command if possible
        if (command->execute) {
            result->execute_successful = command->execute(command);
        }

        instance->processed_command = NULL;

        free(command);
    }

    // cleanup parsed input
    _ctermInternalCleanupInput(cmd);
}