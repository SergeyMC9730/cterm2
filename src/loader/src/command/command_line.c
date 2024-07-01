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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define CPRINTF(format, ...) instance->internal_funcs.cprintf(instance, format, __VA_ARGS__)

// inits command line for this instance
// this is a synchronous function! this function would exit only on an according command
void _ctermInitCommandLine(struct cterm_instance *instance, FILE *input, FILE *output) {
    instance->command_line.input = input;
    instance->command_line.output = output;
    
    bool should_exit = false;

    CPRINTF("\n--- Welcome to CTerm %s\n\b", _ctermGetVersion());

    // allocate string buffer
    char *string_buffer = (char *)malloc(1024);

    while (should_exit == false) {
        // clear string buffer
        memset(string_buffer, 0, 1024);

        CPRINTF("> ", 0);

        fgets(string_buffer, 1024, instance->command_line.input);
    
        // CPRINTF("%s", string_buffer);
    }
}