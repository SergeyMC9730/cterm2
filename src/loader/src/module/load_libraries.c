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
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>

bool _ctermInternalVerifyExt(const char *str, const char *ext) {
    size_t str_len = strlen(str);
    size_t ext_len = strlen(ext);

    size_t offset = str_len - ext_len;

    const char *ptr = str + offset;

    if (strncmp(ptr, ext, ext_len)) return false;
    return true;
}

// load libraries into the cterm instance
void _ctermLoadLibrariesFromFolder(struct cterm_instance *instance, const char *path) {
    instance->ready = false;

    if (path == NULL) {
        instance->internal_funcs.log(instance, instance->log_file_path, "* loading libraries from   ... : FAILED (1)\n");
    
        return;
    }

    instance->internal_funcs.log(instance, instance->log_file_path, "* loading libraries from %s ... : ", path);
    
    // try to open specified directory
    DIR *current_dir = opendir(path); 

    if (!current_dir) { 
        instance->internal_funcs.log(instance, instance->log_file_path, "FAILED (2)\n");

        return;
    }

    instance->internal_funcs.log(instance, instance->log_file_path, "PROCESSING\n");

    struct dirent *de;
    // loop through all files inside directory
    while ((de = readdir(current_dir)) != NULL) {
        // calculate size of file_path string
        size_t file_path_size = strlen(path) + 1 + strlen(de->d_name) + 1;

        // allocate string to put file path into it
        char *file_path = (char *)malloc(file_path_size);

        // format string (put filepath into this string)
        snprintf(file_path, file_path_size, "%s/%s", path, de->d_name);

        // check if file_path contains "cterm_ext". if not, dont do anything
        if(!strstr(file_path, "cterm_ext")) continue;
        // check if extension is valid
        if (!_ctermInternalVerifyExt(de->d_name, "so") && !_ctermInternalVerifyExt(de->d_name, "dll")) continue;

        instance->internal_funcs.log(instance, instance->log_file_path, "  * loading %s : ", file_path);

        struct cterm_module module = _ctermLoadModule(instance, file_path);
        if (!module.load_successful) {
            int reason = 2;

            if (!module.native_representation.load_successful) reason = 1;

            instance->internal_funcs.log(instance, instance->log_file_path, "FAILED (%d)\n", reason);

            free(file_path);

            continue;
        }
            
        instance->internal_funcs.log(instance, instance->log_file_path, "SUCCESS\n");
        instance->internal_funcs.log(instance, instance->log_file_path, "    * \"%s\" - %s\n", module.name, module.version);

        // we should not free file_path because its used in module.native_representation.path
    }

    closedir(current_dir);

    instance->ready = true;
}