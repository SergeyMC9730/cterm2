cmake_minimum_required(VERSION 3.5)

project(cterm LANGUAGES C)

set(CTERM_LAUNCHER_NAME ${PROJECT_NAME})
set(CTERM_LIBRARY_NAME ${PROJECT_NAME}l)

set(CTERM_API_INCLUDES
	src/loader/include
)

if (UNIX)
    set (CTERM_NATIVE_SRC 
		src/loader/src/platform/linux/load_native_lib.c
		src/loader/src/platform/linux/acquire_function.c
	)

	message(STATUS "Compiling CTerm for Unix-based systems")
endif (UNIX)
if (WIN32)
    set (CTERM_NATIVE_SRC
		src/loader/src/platform/windows/load_native_lib.c
	)

	message(STATUS "Compiling CTerm for NT-based systems")
endif (WIN32)

add_library(${CTERM_LIBRARY_NAME} SHARED
	src/loader/src/base/init.c
	src/loader/src/base/log.c
	src/loader/src/base/time.c
	src/loader/src/library/load_libraries.c
	src/loader/src/command/command_line.c
	src/loader/src/module/load_module.c
	src/loader/src/library/load_libraries_p.c
	src/loader/src/module/push_module_p.c
	src/loader/src/command/push_command_p.c
	src/loader/src/command/get_command.c
	src/loader/src/base/version.c
	src/loader/src/command/register_command.c
	src/loader/src/base/cterm_printf.c
	src/loader/src/command/free_input.c
	src/loader/src/command/parse_input.c
	src/loader/src/tools/copy_string.c
	src/loader/src/command/help.c

	${CTERM_NATIVE_SRC}
)
target_include_directories(${CTERM_LIBRARY_NAME} PRIVATE
	${CTERM_API_INCLUDES}
)
target_link_libraries(${CTERM_LIBRARY_NAME}
	dl
)

add_executable(${CTERM_LAUNCHER_NAME}
	src/launcher/src/main.c
	src/launcher/src/print_license.c
)
target_include_directories(${CTERM_LAUNCHER_NAME} PRIVATE 
	src/launcher/include
	${CTERM_API_INCLUDES}
)
target_link_libraries(${CTERM_LAUNCHER_NAME} ${CTERM_LIBRARY_NAME})

# -------------------------------------------------

add_library(cterm_ext_test SHARED src/applications/helloworld/src/main.c)
target_include_directories(cterm_ext_test PRIVATE
	${CTERM_API_INCLUDES}
)
target_link_libraries(cterm_ext_test ${CTERM_LIBRARY_NAME})