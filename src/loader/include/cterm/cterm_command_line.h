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

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>

struct cterm_command_line_input {
    char **argument_list;
    int argument_list_size;
};

enum cterm_internal_commands {
    ICommandNotACommand,
    ICommandLine,
    ICommandExit
};
enum cterm_terminal_command {
    CNone,
    CArrowUp,
    CArrowDown,
    CTEnd
};

struct cterm_execute_result {
    bool execute_successful;
    bool command_not_found;
    bool empty;

    enum cterm_internal_commands internal_command;

    char *executed_command;
};

// used type for _ctermDetectAnsiCommands could be
// changed in the future
#define TERMCMD_TYPE enum cterm_terminal_command

// try to detect ansi command inside the input string
//
// the last element inside this array is CTEnd
//
// array is being allocated inside this function and
// should be freed after use
TERMCMD_TYPE *_ctermDetectAnsiCommands(const char *input);

#ifdef __cplusplus
}
#endif