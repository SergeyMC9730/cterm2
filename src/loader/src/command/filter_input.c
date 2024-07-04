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

char *_ctermInternalFilterInput(const char *v, char f) {
    char *str = _ctermCopyString(v);
    unsigned int len = strlen(str);

    for (unsigned int i = 0; i < len; i++) {
        if (str[i] == f) {
            str[i] = 0;
        }
    }

    return str;
}