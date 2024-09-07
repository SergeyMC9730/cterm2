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
#include <cterm/cterm_general_tools.h>
#include <stddef.h>
#include <stdio.h>
#ifdef __unix__
#include <dirent.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif
#include <string.h>
#include <stdlib.h>

// make private functions accessible inside this file
extern void _ctermInternalPushModule(struct cterm_instance *instance, struct cterm_module module);

bool _ctermInternalVerifyExt(const char *str, const char *ext) {
    size_t str_len = strlen(str);
    size_t ext_len = strlen(ext);

    size_t offset = str_len - ext_len;

    const char *ptr = str + offset;

    if (strncmp(ptr, ext, ext_len)) return false;
    return true;
}

// load libraries into the cterm instance
void _ctermLoadLibrariesFromFolder(struct cterm_instance *instance, const char *_path) {
    if (_path == NULL) {
        instance->internal_funcs.log(instance, instance->log_file_path, "* loading libraries from   ... : FAILED (1)\n");
    
        return;
    }

    instance->internal_funcs.log(instance, instance->log_file_path, "* loading libraries from %s ... : ", _path);
    
#ifdef _NO_PLATFORM_CODE_
    instance->internal_funcs.log(instance, instance->log_file_path, "FAILED (DISABLED)\n");

    return;
#else

    // try to open specified directory
#ifdef __unix__
    char* path = _ctermCopyString(_path);
    DIR *current_dir = opendir(path); 

    // check if directory exists
    if (!current_dir) {
        instance->internal_funcs.log(instance, instance->log_file_path, "FAILED (2)\n");

        return;
    }
#endif
#ifdef _WIN32
    size_t path_len = strlen(_path);

    char* path = (char*)(malloc(path_len + 3));
    
    snprintf(path, path_len + 3, "%s\\*", _path);

    WIN32_FIND_DATA ffd;

    // check if directory is not empty
    HANDLE hFind = FindFirstFile(path, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
        instance->internal_funcs.log(instance, instance->log_file_path, "FAILED (3)\n");

        return;
    }
#endif

    instance->internal_funcs.log(instance, instance->log_file_path, "PROCESSING\n");

#ifdef __unix__
    struct dirent *de;

    // loop through all files inside directory
    while ((de = readdir(current_dir)) != NULL) {
        const char* file_name = de->d_name;
        char dir_separator = '/';
#endif
#ifdef _WIN32
    while (FindNextFile(hFind, &ffd) != 0) {
        const char* file_name = ffd.cFileName;
        char dir_separator = '\\';
#endif
        // calculate size of file_path string
        size_t file_path_size = strlen(_path) + 1 + strlen(file_name) + 1;

        // allocate string to put file path into it
        char *file_path = (char *)malloc(file_path_size);

        // format string (put filepath into this string)
        snprintf(file_path, file_path_size, "%s%c%s", _path, dir_separator, file_name);  

        // check if file_path contains "cterm_ext". if not, dont do anything
        if (!strstr(file_path, "cterm_ext")) {
            // print error
            // instance->internal_funcs.log(instance, instance->log_file_path, "FAILED (3)\n");

            continue;
        }
        // check if extension is valid
        if (!_ctermInternalVerifyExt(file_name, "so") && !_ctermInternalVerifyExt(file_name, "dll")) {
            // print error
            // instance->internal_funcs.log(instance, instance->log_file_path, "FAILED (4)\n");

            continue;
        }

        instance->internal_funcs.log(instance, instance->log_file_path, "  * loading %s : ", file_path);

        // try to load module
        struct cterm_module module = _ctermLoadModule(instance, file_path);
        
        // check if module did not load
        if (!module.load_successful) {
            int reason = 2;

            // check if os failed to load this library
            if (!module.native_representation.load_successful) reason = 1;

            // print error
            instance->internal_funcs.log(instance, instance->log_file_path, "FAILED (%d", reason);

            // check if error is os-related
            if (reason == 1) {
                // print output from native library loader error
                instance->internal_funcs.log(instance, instance->log_file_path, ": %s)\n", module.native_representation.load_error);
            
                // free error
                free(module.native_representation.load_error);
            } else {
                instance->internal_funcs.log(instance, instance->log_file_path, ")\n");
            }

            // free file path string
            free(file_path);

            continue;
        }

        // push this module
        _ctermInternalPushModule(instance, module);
            
        instance->internal_funcs.log(instance, instance->log_file_path, "SUCCESS\n");
        instance->internal_funcs.log(instance, instance->log_file_path, "    * \"%s\" - %s\n", module.name, module.version);

        // we should not free file_path because its used in module.native_representation.path
    }

#ifdef __unix__
    closedir(current_dir);
#endif
#ifdef _WIN32
    FindClose(hFind);
#endif

    free(path); 
#endif
}