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

#ifdef _WIN32

#include <cterm/cterm.h>
#include <cterm/cterm_general_tools.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>

// make unlisted function accessible inside this file

extern void* _ctermInternalNAcquireFunction(struct cterm_native_module module, const char* func_name);

struct cterm_native_module _ctermLoadNativeLibrary(const char* library_path) {
    struct cterm_native_module nmod;

    nmod.path = library_path;
    nmod.acquire_function = _ctermInternalNAcquireFunction;

    // load the library itself
    nmod.native_handle = LoadLibrary(library_path);

    nmod.load_successful = nmod.native_handle != NULL;

    // check if loading failed
    if (!nmod.load_successful) {
        DWORD value = GetLastError();

        char* buffer = (char*)malloc(64);
        snprintf(buffer, 64, "LoadLibrary: %ld", value);

        nmod.load_error = buffer;
    }

    return nmod;
}

#endif