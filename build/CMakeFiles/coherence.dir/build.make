# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/lujunchen/.local/lib/python3.10/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/lujunchen/.local/lib/python3.10/site-packages/cmake/data/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lujunchen/cs4223/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lujunchen/cs4223/src/build

# Include any dependencies generated for this target.
include CMakeFiles/coherence.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/coherence.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/coherence.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/coherence.dir/flags.make

CMakeFiles/coherence.dir/main.cpp.o: CMakeFiles/coherence.dir/flags.make
CMakeFiles/coherence.dir/main.cpp.o: /home/lujunchen/cs4223/src/main.cpp
CMakeFiles/coherence.dir/main.cpp.o: CMakeFiles/coherence.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lujunchen/cs4223/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/coherence.dir/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/coherence.dir/main.cpp.o -MF CMakeFiles/coherence.dir/main.cpp.o.d -o CMakeFiles/coherence.dir/main.cpp.o -c /home/lujunchen/cs4223/src/main.cpp

CMakeFiles/coherence.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/coherence.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lujunchen/cs4223/src/main.cpp > CMakeFiles/coherence.dir/main.cpp.i

CMakeFiles/coherence.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/coherence.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lujunchen/cs4223/src/main.cpp -o CMakeFiles/coherence.dir/main.cpp.s

CMakeFiles/coherence.dir/timer/Timer.cpp.o: CMakeFiles/coherence.dir/flags.make
CMakeFiles/coherence.dir/timer/Timer.cpp.o: /home/lujunchen/cs4223/src/timer/Timer.cpp
CMakeFiles/coherence.dir/timer/Timer.cpp.o: CMakeFiles/coherence.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/home/lujunchen/cs4223/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/coherence.dir/timer/Timer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/coherence.dir/timer/Timer.cpp.o -MF CMakeFiles/coherence.dir/timer/Timer.cpp.o.d -o CMakeFiles/coherence.dir/timer/Timer.cpp.o -c /home/lujunchen/cs4223/src/timer/Timer.cpp

CMakeFiles/coherence.dir/timer/Timer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/coherence.dir/timer/Timer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lujunchen/cs4223/src/timer/Timer.cpp > CMakeFiles/coherence.dir/timer/Timer.cpp.i

CMakeFiles/coherence.dir/timer/Timer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/coherence.dir/timer/Timer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lujunchen/cs4223/src/timer/Timer.cpp -o CMakeFiles/coherence.dir/timer/Timer.cpp.s

# Object files for target coherence
coherence_OBJECTS = \
"CMakeFiles/coherence.dir/main.cpp.o" \
"CMakeFiles/coherence.dir/timer/Timer.cpp.o"

# External object files for target coherence
coherence_EXTERNAL_OBJECTS =

coherence: CMakeFiles/coherence.dir/main.cpp.o
coherence: CMakeFiles/coherence.dir/timer/Timer.cpp.o
coherence: CMakeFiles/coherence.dir/build.make
coherence: CMakeFiles/coherence.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/home/lujunchen/cs4223/src/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable coherence"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/coherence.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/coherence.dir/build: coherence
.PHONY : CMakeFiles/coherence.dir/build

CMakeFiles/coherence.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/coherence.dir/cmake_clean.cmake
.PHONY : CMakeFiles/coherence.dir/clean

CMakeFiles/coherence.dir/depend:
	cd /home/lujunchen/cs4223/src/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lujunchen/cs4223/src /home/lujunchen/cs4223/src /home/lujunchen/cs4223/src/build /home/lujunchen/cs4223/src/build /home/lujunchen/cs4223/src/build/CMakeFiles/coherence.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/coherence.dir/depend

