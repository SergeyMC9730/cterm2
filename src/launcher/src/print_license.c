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

#include <cterm/launcher.h>
#include <stdio.h>

static const char *_ctLauncherLicense =
    "CTerm  Copyright (C) 2024  Sergei Baigerov\n"
    "This program comes with ABSOLUTELY NO WARRANTY; for details type `license'.\n"
    "This is free software, and you are welcome to redistribute it\n"
    "under certain conditions; type `license c' for details.";

void _ctLauncherPrintLicense() {
    printf("%s\n", _ctLauncherLicense);
}