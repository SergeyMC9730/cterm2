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

const char *_ctermInternalGetWarranty() {
    return "THERE IS NO WARRANTY FOR THE PROGRAM, TO THE EXTENT PERMITTED BY\n"
"APPLICABLE LAW.  EXCEPT WHEN OTHERWISE STATED IN WRITING THE COPYRIGHT\n"
"HOLDERS AND/OR OTHER PARTIES PROVIDE THE PROGRAM \"AS IS\" WITHOUT WARRANTY\n"
"OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING, BUT NOT LIMITED TO,\n"
"THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR\n"
"PURPOSE.  THE ENTIRE RISK AS TO THE QUALITY AND PERFORMANCE OF THE PROGRAM\n"
"IS WITH YOU.  SHOULD THE PROGRAM PROVE DEFECTIVE, YOU ASSUME THE COST OF\n"
"ALL NECESSARY SERVICING, REPAIR OR CORRECTION.";
}

const char *_ctermInternalGetConditions() {
    return "GPL V3 REQUIREMENTS:\n"
" - Include a copy of the full license text.\n"
" - State all significant changes made to the original software.\n"
" - Make available the original source code when you distribute any binaries based on the licensed work.\n"
" - Include a copy of the original copyright notice.";
}

bool _ctermInternalLicenseCommand(struct cterm_command *command) {
    struct cterm_instance *instance = command->linked_instance;

    const char *available_commands[2] = {"w", "c"};
    const char *available_descriptions[2] = {"show warranty", "show conditions"};

    unsigned char len = sizeof(available_commands) / sizeof(const char *);

    if (command->argc <= 1) {
        CPRINTF("License command is not specified. ", 0);

        if (len >= 2) {
            CPRINTF("You can choose %d of them to continue:\n", len);
        } else {
            CPRINTF("Select this command to continue:\n", 0);
        }

        for (unsigned char i = 0; i < len; i++) {
            CPRINTF("   - \"%s\" -> %s\n", available_commands[i], available_descriptions[i]);
        }

        return false;
    }

    char mode = command->argv[1][0];

    switch (mode) {
        case 'w': {
            CPRINTF("%s\n", _ctermInternalGetWarranty());
            return true;
        }
        case 'c': {
            CPRINTF("%s\n", _ctermInternalGetConditions());
            return true;
        }
        default: {
            CPRINTF("License command is invalid. ", 0);

            if (len >= 2) {
                CPRINTF("You can choose %d of them to continue:\n", len);
            } else {
                CPRINTF("Select this command to continue:\n", 0);
            }

            for (unsigned char i = 0; i < len; i++) {
                CPRINTF("   - \"%s\" -> %s\n", available_commands[i], available_descriptions[i]);
            }

            return true;
        }
    }
}