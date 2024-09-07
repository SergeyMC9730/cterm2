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
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

// tries to find command by its name and make a copy of it
// you should free this command after use!
cterm_command_alloc _ctermGetCommand(struct cterm_instance *instance, const char *command) {
    cterm_command_alloc _command = NULL;

    unsigned int copy_index = 0xFFFFFFFF;

    // check if command array is not accessible
    if (instance->commands == NULL) return _command;

    unsigned int i = 0;

    // loop through all commands available
    for (i = 0; i < instance->commands_size; i++) {
        // get command reference
        struct cterm_command *cmd_ptr = instance->commands + i;
        
        // check if string is not equal to the other
        if (strcmp(cmd_ptr->name, command) != 0) continue;

        // set copy index value
        copy_index = i;

        // return from the loop
        break;
    }

    if (copy_index == 0xFFFFFFFF) {
        // return NULL (no command found)
        return _command;
    }

    // allocate _command variable
    _command = (cterm_command_alloc)malloc(sizeof(struct cterm_command));

    // get command reference
    struct cterm_command *cmd_ptr = instance->commands + copy_index;

    // copy command
    _command->argc = cmd_ptr->argc;
    _command->argv = cmd_ptr->argv;
    _command->description = cmd_ptr->description;
    _command->execute = cmd_ptr->execute;
    _command->name = cmd_ptr->name;
    _command->linked_instance = instance;

    return _command;
}