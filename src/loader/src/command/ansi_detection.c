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
#include <cterm/cterm_general_macros.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// try to detect ansi command inside the input string
//
// the last element inside this array is CTEnd
//
// array is being allocated inside this function and
TERMCMD_TYPE *_ctermDetectAnsiCommands(const char *input) {
    unsigned int commands = 0;
    unsigned int command_size = sizeof(TERMCMD_TYPE);

    TERMCMD_TYPE *result = NULL;

    // char pattern_arrow_up
    return result;
}