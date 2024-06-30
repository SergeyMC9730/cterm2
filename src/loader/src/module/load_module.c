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

struct cterm_module _ctermLoadModule(struct cterm_instance *instance, const char *module_path) {
    struct cterm_module mod = {};
    mod.native_representation = _ctermLoadNativeLibrary(module_path);

    if (!mod.native_representation.load_successful) {
        return mod;
    }

    // get basic cterm module functions

    void *get_module_name_ptr = mod.native_representation.acquire_function(mod.native_representation, "get_module_name");
    void *get_module_description_ptr = mod.native_representation.acquire_function(mod.native_representation, "get_module_description");
    void *get_module_version_ptr = mod.native_representation.acquire_function(mod.native_representation, "get_module_version");
    void *on_init_ptr = mod.native_representation.acquire_function(mod.native_representation, "on_init");
    void *on_close_ptr = mod.native_representation.acquire_function(mod.native_representation, "on_close");

    int failed_functions = 0;
    int max_failed_functions = 3;

    // get module name if possible
    if (get_module_name_ptr != NULL) {
        // cast raw ptr into the function pointer using these macros
        CTERM_FPTR(get_module_name, const char *) = (CTERM_FPTR_CAST(const char *))get_module_name_ptr;
    
        mod.name = get_module_name();
    } else {
        failed_functions++;
    }

    // get module description if possible
    if (get_module_description_ptr != NULL) {
        // cast raw ptr into the function pointer using these macros
        CTERM_FPTR(get_module_description, const char *) = (CTERM_FPTR_CAST(const char *))get_module_description_ptr;

        mod.description = get_module_description();
    } else {
        failed_functions++;
    }

    // get module version if possible
    if (get_module_version_ptr != NULL) {
        // cast raw ptr into the function pointer using these macros
        CTERM_FPTR(get_module_version, const char *) = (CTERM_FPTR_CAST(const char *))get_module_version_ptr;

        mod.version = get_module_version();
    } else {
        failed_functions++;
    }

    // check if module loading failed
    if (failed_functions >= max_failed_functions) {
        // we couldn't load this module
        return mod;
    }

    // check if on_init exists. if not, return failure
    if (on_init_ptr == NULL) {
        // we couldn't load this module
        return mod;
    }

    CTERM_FPTR(on_init, void, struct cterm_module *) = (CTERM_FPTR_CAST(void, struct cterm_module *))on_init_ptr;

    mod.on_init = on_init;

    // check if on_close exists (OPTIONAL)
    if (on_close_ptr != NULL) {
        CTERM_FPTR(on_close, void, struct cterm_module *) = (CTERM_FPTR_CAST(void, struct cterm_module *))on_close_ptr;
    
        mod.on_close = on_close;
    }

    // setup values
    mod.cterm_instance = instance;    

    // init module
    mod.on_init(&mod);

    mod.load_successful = true;

    return mod;
}