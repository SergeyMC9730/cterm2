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

unsigned int _ctermInternalGetCommandIndex(struct cterm_instance *instance, const char *name) {
    if (instance->commands == NULL) {
        return 0xFFFFFFFF;
    }

    for (unsigned int i = 0; i < instance->commands_size; i++) {
        struct cterm_command *ref = instance->commands + i;

        if (strcmp(ref->name, name)) continue;

        return i;
    }

    return 0xFFFFFFFF;
}

void _ctermInternalPushCommand(struct cterm_instance *instance, struct cterm_command command) {
    // increase size of the commands array
    instance->commands_size++;

    // calculate array size to be allocated
    unsigned int struct_size = sizeof(struct cterm_command);
    unsigned int alloc_size = struct_size * instance->commands_size;

    // get index from current command
    unsigned int old_cmd_index = 0xFFFFFFFF;
    if (instance->processed_command != NULL) {
        _ctermInternalGetCommandIndex(instance, instance->processed_command->name);
    }

    // reallocate commands array
    CTERM_SAFE_REALLOC(instance->commands, alloc_size, struct cterm_command);

    // add new command
    instance->commands[instance->commands_size - 1] = command;

    if (old_cmd_index != 0xFFFFFFFF) {
        instance->processed_command = instance->commands + old_cmd_index;
    }
}