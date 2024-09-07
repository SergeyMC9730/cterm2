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

#define CPRINTF(format, ...) instance->internal_funcs.cprintf(instance, format, __VA_ARGS__)

bool _ctermInternalHelloCommand(struct cterm_command *command) {
    struct cterm_instance *instance = command->linked_instance;

    CPRINTF("Hello, World!\n", 0);
    instance->internal_funcs.log(instance, instance->log_file_path, "Hello, World!\n");

    return true;
}