# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/clion-2020.2/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /opt/clion-2020.2/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/server_c.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/server_c.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/server_c.dir/flags.make

CMakeFiles/server_c.dir/main.c.o: CMakeFiles/server_c.dir/flags.make
CMakeFiles/server_c.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/server_c.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/server_c.dir/main.c.o   -c /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/main.c

CMakeFiles/server_c.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/server_c.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/main.c > CMakeFiles/server_c.dir/main.c.i

CMakeFiles/server_c.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/server_c.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/main.c -o CMakeFiles/server_c.dir/main.c.s

# Object files for target server_c
server_c_OBJECTS = \
"CMakeFiles/server_c.dir/main.c.o"

# External object files for target server_c
server_c_EXTERNAL_OBJECTS =

server_c: CMakeFiles/server_c.dir/main.c.o
server_c: CMakeFiles/server_c.dir/build.make
server_c: CMakeFiles/server_c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable server_c"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/server_c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/server_c.dir/build: server_c

.PHONY : CMakeFiles/server_c.dir/build

CMakeFiles/server_c.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/server_c.dir/cmake_clean.cmake
.PHONY : CMakeFiles/server_c.dir/clean

CMakeFiles/server_c.dir/depend:
	cd /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/cmake-build-debug /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/cmake-build-debug /home/CS/users/jmeleski/.linux/rwhite-jmeleski/server.c/cmake-build-debug/CMakeFiles/server_c.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/server_c.dir/depend

