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

#include <cterm/cterm_command_line.h>
#include <cterm/cterm_general_macros.h>
#include <cterm/cterm_general_tools.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool _ctermInternalContainsOnlySymbol(const char *str, char c) {
    unsigned int len = strlen(str);

    for (unsigned int i = 0; i < len; i++) {
        if (str[i] != c) return false;
    }

    return true;
}

struct cterm_command_line_input _ctermInternalParseInput(const char *_user_input) {
    unsigned int max_argument_size = 1024;
    char *current_string = (char *)malloc(max_argument_size);
    unsigned int local_pointer = 0;

    unsigned int len = strlen(_user_input);

    struct cterm_command_line_input list = {NULL, 0};

    if (len == 0) return list;

    bool had_space_before = false;

    unsigned char ptr_size = sizeof(char *);

    char delim = ' ';
    char delim2 = '\n';

    unsigned int offset = 0;

    unsigned char __c = _user_input[0];

    bool cannot_parse = false;

    while (__c == delim || __c == delim2) {
        if (offset == len) {
            cannot_parse = true;
            break;
        }

        __c = _user_input[offset++];
    }

    const char *user_input = _user_input;

    if (offset != 0) {
        user_input += offset - 1;
    }

    if (cannot_parse) {
        return list;
    }

    bool inside_string = false;

    for (unsigned int i = 0; i < len; i++) {
        bool should_split = false;
        bool should_split_max = false;
        char current_char = user_input[i];

        if (local_pointer >= max_argument_size) {
            should_split = true;
            should_split_max = true;
        } else {
            if (current_char == '"') {
                inside_string = !inside_string;
            }

            if (current_char == delim || current_char == delim2) {
                if (!inside_string) {
                    if (had_space_before) {
                        continue;
                    }

                    should_split = true;
                    had_space_before = true;
                }
            } else {
                had_space_before = false;
            }
            
            if (!should_split) {
                current_string[local_pointer++] = current_char;
                current_string[local_pointer] = 0;   
            }
        }

        if (should_split) {
            list.argument_list_size++;
            CTERM_SAFE_REALLOC(list.argument_list, ptr_size * list.argument_list_size, char *);
            
            list.argument_list[list.argument_list_size - 1] = _ctermCopyString(current_string);

            memset(current_string, 0, max_argument_size);
            local_pointer = 0;
        }
    }

    if (!_ctermInternalContainsOnlySymbol(current_string, ' ')) {
        list.argument_list_size++;
        CTERM_SAFE_REALLOC(list.argument_list, ptr_size * list.argument_list_size, char *);
        list.argument_list[list.argument_list_size - 1] = _ctermCopyString(current_string);
    }

    free(current_string);

    return list;
}