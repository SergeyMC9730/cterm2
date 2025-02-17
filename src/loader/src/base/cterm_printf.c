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
#include <stdarg.h>
#include <stdio.h>

void _ctermInternalPrintf(struct cterm_instance *instance, const char *format, ...) {
    // check if printing is not possible
    if (instance->command_line.output == NULL) return;
    
    // init variable arguments
    va_list arglist;
    va_start(arglist, format);

    // print formatted data into stdout or something else
    // defined in the instance->command_line.output
    vfprintf(instance->command_line.output, format, arglist);

    // unload arguments
    va_end(arglist);
}
