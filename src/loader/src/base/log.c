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

#include <stdio.h>
#include <stdarg.h>
#include <cterm/cterm.h>

void _ctermInternalLog(struct cterm_instance *instance, const char *file, const char *format, ...) {
    // open file in "a" mode (append)
    FILE *log_file = fopen(file, "a");

    // init variable arguments
    va_list arglist;
    va_start(arglist, format);

    // print formatted data into file
    vfprintf(log_file, format, arglist);

    // unload arguments
    va_end(arglist);

    // close file
    fclose(log_file);

    if (instance->log_io_extra.output == NULL) return;

    // load variadic arguments again
    va_start(arglist, format);

    // print formatted data into stdout or something else
    // defined in the instance->log_io_extra.output
    vfprintf(instance->log_io_extra.output, format, arglist);

    // unload arguments
    va_end(arglist);
}