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

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CTERM_INIT_EXTRA_LOG_IO 1

struct _IO_FILE;
typedef struct _IO_FILE FILE;

struct cterm_native_module {
    void *(*acquire_function)(struct cterm_native_module module, const char *func_name);

    const char *path;

#ifdef __unix__
    void *native_handle;
#endif

    bool load_successful;
};

#define CTERM_FPTR_CAST(ret, ...) ret (*)(__VA_ARGS__)
#define CTERM_FPTR(name, ret, ...) ret (*name)(__VA_ARGS__)

struct cterm_instance;

struct cterm_module {
    struct cterm_native_module native_representation;
    struct cterm_instance *cterm_instance;

    const char *name;
    const char *version;
    const char *description;

    bool load_successful;

    void (*on_init)(struct cterm_module *module);
    void (*on_close)(struct cterm_module *module);
};

struct cterm_command {
    struct cterm_module *linked_module;

    const char *name;
    const char *description;
};

struct cterm_pipe_io {
    FILE *input;
    FILE *output;
};

struct cterm_instance {
    struct cterm_command *commands;
    unsigned int commands_size;

    struct cterm_internal {
        void (*log)(struct cterm_instance *instance, const char *file, const char *format, ...);
    } internal_funcs;

    struct cterm_pipe_io command_line;

    const char *log_file_path;
    struct cterm_pipe_io log_io_extra;

    bool ready;
};

struct cterm_instance _ctermInit();

struct cterm_module _ctermLoadModule(struct cterm_instance *instance, const char *module_path);
struct cterm_native_module _ctermLoadNativeLibrary(const char *library_path);

void _ctermParseModule(struct cterm_instance *instance, struct cterm_module module);

// inits command line for this instance
// this is a synchronous function! this function would exit only on an according command
void _ctermInitCommandLine(struct cterm_instance *instance, FILE *input, FILE *output);

// load libraries into the cterm instance
void _ctermLoadLibrariesFromFolder(struct cterm_instance *instance, const char *path);

#define CTERM_INIT_MODULE(name, description, version) const char *get_module_name() { return name; } const char *get_module_description() { return description; } const char *get_module_version() { return version; }

#ifdef __cplusplus
}
#endif