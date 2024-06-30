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

//
// make internal functions accessible inside this file
//

// log into file
extern void _ctermInternalLog(struct cterm_instance *instance, const char *file, const char *format, ...);

// get current time in a string representation. free string after use!
extern char *_ctermInternalGetTime();

// load libraries into the cterm instance
extern void _ctermInternalLoadLibraries(struct cterm_instance *instance);







struct cterm_instance _ctermInit() {
    // create instance inside the stack
    // also set everything to zero or NULL
    struct cterm_instance i = {};

    // load log function into the instance
    i.internal_funcs.log = _ctermInternalLog;
    i.log_file_path = "cterm_log.log";

#if CTERM_INIT_EXTRA_LOG_IO == 1
    i.log_io_extra.input = stdin;
    i.log_io_extra.output = stdout;
#endif

    // print early welcome message
    i.internal_funcs.log(&i, i.log_file_path, "\n--- INITIALIZING CTERM INSTANCE---\n");

    // get current time
    char *current_time = _ctermInternalGetTime();

    i.internal_funcs.log(&i, i.log_file_path, "* initialization began on %s\n", current_time);

    // free string
    free(current_time);

    // load libraries
    i.internal_funcs.log(&i, i.log_file_path, "* loading libraries into the cterm instance\n");
    _ctermInternalLoadLibraries(&i);

    // complete initializing
    i.ready = true;

    return i;
}