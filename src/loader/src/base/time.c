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
#include <time.h>
#include <stdlib.h>

char *_ctermInternalGetTime() {
    // get current time (unix timestamp)
    time_t current_time = time(NULL);

    // format string used for convertion unix timestamp into the string
    const char *format = "%a %b %d %Y at %H:%M";

    struct tm lt;

    // allocate the resulting string
    char *result = (char *)malloc(64);

    // make timestamp localized, put time info into lt
    localtime_r(&current_time, &lt);

    // format string
    strftime(result, 64, format, &lt);
    
    return result;
}