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
#include <stdlib.h>

void _ctermInternalPushModule(struct cterm_instance *instance, struct cterm_module module) {
    // increase size of the modules array
    instance->modules_size++;

    // calculate array size to be allocated
    unsigned int struct_size = sizeof(struct cterm_module);
    unsigned int alloc_size = struct_size * instance->modules_size;

    // reallocate modules array
    CTERM_SAFE_REALLOC(instance->modules, alloc_size, struct cterm_module);

    // add new module
    instance->modules[instance->modules_size - 1] = module;
}