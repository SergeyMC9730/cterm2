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

#define CTERM_SAFE_REALLOC(value, size, ...) if (value == NULL) { value = (__VA_ARGS__ *)(malloc(size)); } else { value = (__VA_ARGS__ *)(realloc(value, size)); }
#define CTERM_FPTR_CAST(ret, ...) ret (*)(__VA_ARGS__)
#define CTERM_FPTR(name, ret, ...) ret (*name)(__VA_ARGS__)