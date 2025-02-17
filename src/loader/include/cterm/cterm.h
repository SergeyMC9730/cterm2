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
#include <cterm/cterm_general_macros.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CTERM_INIT_EXTRA_LOG_IO 1

#include <stdio.h>

#pragma pack(push, 1)

struct cterm_native_module {
    void *(*acquire_function)(struct cterm_native_module module, const char *func_name);

    const char *path;

#ifdef __unix__
    void *native_handle;
#endif
#ifdef _WIN32
    void* native_handle;
#endif

    bool load_successful;
    char *load_error;
};

struct cterm_instance;
struct cterm_execute_result;

struct cterm_module {
    struct cterm_native_module native_representation;
    struct cterm_instance *cterm_instance;

    const char *name;
    const char *version;
    const char *description;

    bool load_successful;

    CTERM_FPTR(on_init, void, struct cterm_module *module);
    CTERM_FPTR(on_close, void, struct cterm_module *module);
};

#define CTERM_COMMAND_EXECUTE CTERM_FPTR(execute, bool, struct cterm_command *command)

struct cterm_command {
    const char *name;
    const char *description;

    int argc;
    char **argv;
    
    struct cterm_instance *linked_instance;

    CTERM_COMMAND_EXECUTE;
};

typedef struct cterm_command* cterm_command_alloc;

struct cterm_pipe_io {
    FILE *input;
    FILE *output;
};

struct cterm_instance {
    struct cterm_command *commands;
    unsigned int commands_size;

    struct cterm_module *modules;
    unsigned int modules_size;

    struct cterm_internal {
        void (*log)(struct cterm_instance *instance, const char *file, const char *format, ...);
        void (*cprintf)(struct cterm_instance *instance, const char *format, ...);
    } internal_funcs;

    struct cterm_pipe_io command_line;

    const char *log_file_path;
    struct cterm_pipe_io log_io_extra;

    struct cterm_command *processed_command;

    bool ready;
};

#pragma pack(pop)

struct cterm_instance _ctermInit(bool log_into_stdout);

struct cterm_module _ctermLoadModule(struct cterm_instance *instance, const char *module_path);

#ifndef _NO_PLATFORM_CODE_
struct cterm_native_module _ctermLoadNativeLibrary(const char *library_path);
#endif

// inits command line for this instance
// this is a synchronous function! this function would exit only on an according command
void _ctermInitCommandLine(struct cterm_instance *instance, FILE *input, FILE *output);

// load libraries into the cterm instance
void _ctermLoadLibrariesFromFolder(struct cterm_instance *instance, const char *path);

// tries to find command by its name and make a copy of it
// you should free this command after use!
cterm_command_alloc _ctermGetCommand(struct cterm_instance *instance, const char *command);

// get cterm version
const char *_ctermGetVersion();

// register command
void _ctermRegisterCommand(struct cterm_instance *instance, const char *command, const char *description, CTERM_COMMAND_EXECUTE);

// executes command
void _ctermExecute(struct cterm_instance *instance, const char *input, struct cterm_execute_result *result);

// closes this instance
void _ctermClose(struct cterm_instance *instance);

#define CTERM_INIT_MODULE(name, description, version) const char *get_module_name() { return name; } const char *get_module_description() { return description; } const char *get_module_version() { return version; }

#ifdef __cplusplus
}
#endif